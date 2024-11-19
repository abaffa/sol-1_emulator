//
// hw_tty.cpp
//
////// BEGIN LICENSE NOTICE//////
//
//Baffa-1 HomebrewCPU Minicomputer System Emulator
//
//Copyright(C) 2021 Augusto Baffa, (sol-1.baffasoft.com.br)
//
//This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301, USA.
//
////// END LICENSE NOTICE//////
//
#include "hw_tty.h"
#include "baffa1_computer.h"
#include <stdio.h>
#include <iostream>


#if defined(_MSC_VER) || defined(__MINGW32__)    
#include <windows.h>
#include <conio.h>
#else
//#include <pthread.h> 
#endif



//TELNET PROTOCOL
//
//https://datatracker.ietf.org/doc/html/rfc1116
//https://tools.ietf.org/html/rfc854
//https://datatracker.ietf.org/doc/html/rfc854#page-14
//https://tools.ietf.org/html/rfc855
//https://datatracker.ietf.org/doc/html/rfc858
//https://datatracker.ietf.org/doc/rfc930/
//https://en.wikipedia.org/wiki/Telnet#Related_RFCs
//http://www.iana.org/assignments/telnet-options/telnet-options.xhtml
//
#define T_SE      (uint8_t)240 // 0xf0
#define T_NOP     (uint8_t)241 // 0xf1
#define T_BREAK   (uint8_t)243 // 0xf3
#define T_GOAHEAD (uint8_t)249 // 0xf9
#define T_SB      (uint8_t)250 // 0xfa
#define T_WILL    (uint8_t)251 // 0xfb
#define T_WONT    (uint8_t)252 // 0xfc
#define T_DO      (uint8_t)253 // 0xfd
#define T_DONT    (uint8_t)254 // 0xfe
#define T_IAC     (uint8_t)255 // 0xff

#define TO_SEND_BINARY        (uint8_t)0
#define TO_ECHO               (uint8_t)1
#define TO_SUPPRESS_GO_AHEAD  (uint8_t)3
#define TO_TERMINAL_TYPE      (uint8_t)24
#define TO_LINEMODE           (uint8_t)34


void transmit_welcome(HW_TTY_CLIENT *current_client) {

#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client = (current_client)->client;
#else
	int client = (current_client)->client;
#endif

	uint8_t  welcome_buf[10] = { T_IAC, T_DO, TO_LINEMODE, T_IAC, T_WILL, TO_ECHO, 0x0 };
	send(client, reinterpret_cast<const char*>(welcome_buf), 7, 0);

	char welm[512];
	sprintf(welm, "Baffa-1 74HC HomebrewCPU MiniComputer.\r\n");
	sprintf(welm + strlen(welm), "Emulator Version 1.0\r\n");
	sprintf(welm + strlen(welm), "\r\n");
	sprintf(welm + strlen(welm), "terminal-%d initialized\r\n", (current_client->index + 1));
	send(client, welm, (int)strlen(welm), 0);
	
	char *buf_send = (char *)"\r\nPassword: ";
	send(client, buf_send, (int)strlen(buf_send), 0);
}

uint8_t transmit_bytes(HW_TTY_CLIENT *current_client) {

#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client = (current_client)->client;
#else
	int client = (current_client)->client;
#endif

	if (!current_client->tty_out.empty()) {

#if defined(_MSC_VER) || defined(__MINGW32__)    
		std::unique_lock<std::mutex> lock(current_client->mtx_out);
#else
		//pthread_mutex_lock(&current_client->mtx_out);
#endif

		BAFFA1_BYTE data = current_client->tty_out.front(); current_client->tty_out.pop();

#if defined(_MSC_VER) || defined(__MINGW32__)    
		current_client->cv_out.notify_all();
#else
		//pthread_mutex_unlock(&current_client->mtx_out);
#endif

		char buf_send[4];
		buf_send[0] = data;
		buf_send[1] = '\0';
		buf_send[2] = '\0';
		buf_send[3] = '\0';

		if (buf_send[0] == 0x08) { //CONVERT BS TO DEL
			buf_send[1] = 0x20;
			buf_send[2] = 0x08;
		}

		if (current_client->lastSentChar == '\r' && data != '\n')
			send(client, "\n", 1, 0);

		int send_status = send(client, buf_send, (int)strlen(buf_send), 0);

		if (current_client->lastSentChar == '\n' &&  data != '\n')
			current_client->lastSentChar = '\0';
		else
			current_client->lastSentChar = data;

		if (send_status <= 0)
			return 0;

		
	}

	return 1;
}

void process_received_byte(HW_TTY_CLIENT *current_client, uint8_t b) {
	if (*(current_client->console) == 0) {
		if (b == 0x04)
		{
			*(current_client->debug_call) = 1;
		}
		else {
			current_client->_hw_uart->receive(b);
			//current_client->baffa1_cpu.microcode.mccycle.int_request = 0x01;
		}
	}
	else if (*(current_client->console) == 1) {
		//HW_TTY::net_data *net_data = (HW_TTY::net_data*)malloc(sizeof(HW_TTY::net_data));
		current_client->tty_in->push(b);
	}
}


uint8_t process_special_key(HW_TTY_CLIENT *current_client, uint8_t *local_buff) {

	if (*local_buff == 0x7F) //CONVERT DEL TO BS
		*local_buff = 0x08;

	if (current_client->receivedCR == 0 && *local_buff == 0x0A)		current_client->receivedCR = 0x0A;
	else if (current_client->receivedCR == 0 && *local_buff == 0x0D)	current_client->receivedCR = 0x0D;
	else if ((current_client->receivedCR == 0x0A && *local_buff == 0x0D) || (current_client->receivedCR == 0x0D && *local_buff == 0x0)) return 2;
	else return 1;

	return 0;
}

uint8_t receive_bytes(HW_TTY_CLIENT *current_client, BAFFA1_BYTE *receiv_buff) {

#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client = (current_client)->client;
#else
	int client = (current_client)->client;
#endif

	if (current_client->proc_receiv_count == 0) {
		current_client->receiv_total = recv(client, (char *)receiv_buff, 1, 0);// MSG_WAITALL);
	}

	if (current_client->receiv_total > 0) {

		if (current_client->proc_receiv_count < current_client->receiv_total) {
			uint8_t local_buff = receiv_buff[current_client->proc_receiv_count++];

			if (current_client->subnegotiation)
			{
				if (current_client->handleTelnetProtocol) { printf("<%02x", local_buff); }

				if (current_client->cmdLen == 0 && local_buff == T_IAC) current_client->cmdLen = 1;
				else if (current_client->cmdLen == 1 && local_buff == T_SE)
				{
					current_client->subnegotiation = false;
					current_client->cmdLen = 0;
				}
				else current_client->cmdLen = 0;



			}
			// ---- handle IAC sequences

			else if (current_client->cmdLen == 0)
			{
				// waiting for IAC uint8_t
				if (local_buff == T_IAC)
				{
					current_client->cmdLen = 1;
					current_client->cmd[0] = T_IAC;
					if (current_client->handleTelnetProtocol) { printf("<%02x", local_buff); }
				}
				else {
					// Process received bytes from Telnet

					uint8_t _do = process_special_key(current_client, &local_buff);


					if (current_client->user_validated == 1) {
						if (_do == 2) {
							current_client->receivedCR = 0;
							process_received_byte(current_client, 0x0D);
							process_received_byte(current_client, 0x0A);
						}
						else if (_do == 1) {
							if (current_client->receivedCR != 0) process_received_byte(current_client, current_client->receivedCR);

							current_client->receivedCR = 0;
							process_received_byte(current_client, local_buff);
						}
					}
					else {
						if (_do == 2) {
							current_client->receivedCR = 0;
							if (strcmp(current_client->passBuffer, tty_password) == 0) {
								current_client->user_validated = 1;

								process_received_byte(current_client, 0x0D);
								process_received_byte(current_client, 0x0A);
							}
							else {
								current_client->passBuffer[0] = 0;
								char *buf_send = (char *)"\r\nPassword: ";
								send(client, buf_send, (int)strlen(buf_send), 0);
							}
						}
						else if (_do == 1) {
							current_client->receivedCR = 0;

							uint8_t s = (uint8_t)strlen(current_client->passBuffer);

							if (s <= 20) {
								current_client->passBuffer[s] = local_buff;
								current_client->passBuffer[s+1] = 0;
							}
						}
					}

				}
			}
			else if (current_client->cmdLen == 1)
			{
				if (current_client->handleTelnetProtocol) { printf("<%02x", local_buff); }
				// received second uint8_t of IAC sequence
				if (local_buff == T_IAC)
				{
					// IAC->IAC sequence means regular 0xff data value
					current_client->cmdLen = 0;

					// we already skipped the first IAC (0xff), so just return 'false' to pass this one through

				}
				else if (local_buff == T_NOP || local_buff == T_BREAK || local_buff == T_GOAHEAD)
				{
					// NOP, BREAK, GOAHEAD => do nothing and skip
					current_client->cmdLen = 0;

				}
				else if (local_buff == T_SB)
				{
					// start of sub-negotiation
					current_client->subnegotiation = true;
					current_client->cmdLen = 0;

				}
				else
				{
					// record second uint8_t of sequence
					current_client->cmdLen = 2;
					current_client->cmd[1] = local_buff;

				}
			}
			else if (current_client->cmdLen == 2)
			{
				// received third (i.e. last) uint8_t of IAC sequence
				if (current_client->handleTelnetProtocol) { printf("<%02x", local_buff); }
				current_client->cmd[2] = local_buff;

				bool reply = true;
				if (current_client->cmd[1] == (char)T_WILL)
				{
					switch (current_client->cmd[2])
					{
					case TO_SEND_BINARY:          current_client->cmd[1] = T_DO;   break;
					case TO_ECHO:                 current_client->cmd[1] = current_client->telnetDisableLocalEcho ? T_DONT : T_DO; break;
					case TO_SUPPRESS_GO_AHEAD:    current_client->cmd[1] = T_DO; break;
					default:   current_client->cmd[1] = T_DONT; break;
					}
				}
				else if (current_client->cmd[1] == (char)T_WONT)
				{
					switch (current_client->cmd[2])
					{
					case TO_SEND_BINARY:   current_client->cmd[1] = T_DO; break;
					default:			   current_client->cmd[1] = T_DONT; break;
					}

				}
				else if (current_client->cmd[1] == T_DO)
				{
					switch (current_client->cmd[2])
					{
					case TO_SEND_BINARY:         current_client->cmd[1] = T_WILL;  break;
					case TO_SUPPRESS_GO_AHEAD:   current_client->cmd[1] = T_WILL; break;
					case TO_TERMINAL_TYPE:       current_client->cmd[1] = current_client->telnetTerminalType == 0 ? T_WONT : T_WILL; break;
					case TO_ECHO:                current_client->cmd[1] = current_client->telnetDisableLocalEcho ? T_WILL : T_WONT; break;
					default:   current_client->cmd[1] = T_WONT; break;
					}
				}
				else if (current_client->cmd[1] == T_DONT)
				{
					switch (current_client->cmd[2])
					{
					case TO_SEND_BINARY:   current_client->cmd[1] = T_WILL;   break;
					default:   current_client->cmd[1] = T_WONT; break;
					}

				}
				else
					reply = false;

				// send reply if necessary
				if (reply)
				{
					if (current_client->handleTelnetProtocol)
						for (int k = 0; k < 3; k++)
						{
							printf(">%02x", (uint8_t)current_client->cmd[k]);
						}

					send(client, reinterpret_cast<const char*>(current_client->cmd), 3, 0);

					if (current_client->cmd[1] == T_WILL && current_client->cmd[2] == TO_TERMINAL_TYPE)
					{
						// send terminal-type subnegoatiation sequence
						uint8_t buf[110], i, n = 0;
						buf[n++] = T_IAC;
						buf[n++] = T_SB;
						buf[n++] = TO_TERMINAL_TYPE;
						buf[n++] = 0; // IS
						for (i = 0; i < 100 && current_client->telnetTerminalType[i] >= 32 && current_client->telnetTerminalType[i] < 127; i++)
							buf[n++] = current_client->telnetTerminalType[i];
						buf[n++] = 0;
						buf[n++] = T_IAC;
						buf[n++] = T_SE;

						send(client, reinterpret_cast<const char*>(buf), n, 0);

						if (current_client->handleTelnetProtocol)
							for (int k = 0; k < n; k++) { printf(">%02x", (uint8_t)buf[k]); }
					}
				}

				// start over
				current_client->cmdLen = 0;

			}
			else
			{
				// invalid state (should never happen) => just reset
				current_client->cmdLen = 0;

			}
		}
		else {
			current_client->receiv_total = 0;
			current_client->proc_receiv_count = 0;
			memset(receiv_buff, 0, 512 * (sizeof(receiv_buff[0])));
		}

	}
	else if (current_client->receiv_total == 0) 
		return 0;

	return 1;
}


#if defined(_MSC_VER) || defined(__MINGW32__)    
DWORD WINAPI TelnetClientThread(LPVOID pParam)
#else
void *TelnetClientThread(void *pParam)
#endif
{
	HW_TTY_CLIENT *current_client = (HW_TTY_CLIENT*)pParam;

	BAFFA1_BYTE receiv_buff[0x200];// FFFF];
	   
#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client = (current_client)->client;
#else
	int client = (current_client)->client;
#endif

	transmit_welcome(current_client);

	while (true)
	{

		if (current_client->user_validated == 1)
			if (!transmit_bytes(current_client)) break;

		if (!receive_bytes(current_client, receiv_buff)) break;
	}

#if defined(_MSC_VER) || defined(__MINGW32__)     
	closesocket(client);
#else
	shutdown(client, 2);
	close(client);
#endif
	current_client->reset();
	current_client->running = 0;
	current_client->client = -1;


	return 0;

}



#if defined(_MSC_VER) || defined(__MINGW32__)    
DWORD WINAPI TelnetServerThread(LPVOID pParam)
#else
void *TelnetServerThread(void *pParam)
#endif
{

	HW_TTY_CLIENT* clients = (HW_TTY_CLIENT*)pParam;

	sockaddr_in local;
#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET server;
	WSADATA wsaData;
	int wsaret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaret != 0)
	{
		return 0;
	}
#else
	int server;
#endif
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons((u_short)SERVER_PORT);
#if defined(_MSC_VER) || defined(__MINGW32__)   
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		return 0;
	}
#else
	if ((server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		return 0;
	}
#endif
	if (::bind(server, (sockaddr*)&local, sizeof(local)) != 0)
	{
		return 0;
	}
	if (listen(server, pool_size) != 0)
	{
		return 0;
	}

	sockaddr_in from;
#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client;
	int fromlen = sizeof(from);
#else
	int client;
	socklen_t fromlen = sizeof(from);
#endif

	while (true)
	{
		client = accept(server,
			(struct sockaddr*)&from, &fromlen);

#if defined(_MSC_VER) || defined(__MINGW32__)    
		u_long mode = 1;  // 1 to enable non-blocking socket
		ioctlsocket(client, FIONBIO, &mode);
#else
		fcntl(client, F_SETFL, O_NONBLOCK);
#endif

		HW_TTY_CLIENT *new_client = NULL;
		int i = 0;
		for (i = 0; i < pool_size; i++) {
			if (clients[i].running == 0) {
				clients[i].reset();
				clients[i].running = 1;
				clients[i].client = client;
				new_client = &clients[i];
				break;
			}
		}
		if (new_client != NULL) {


#if defined(_MSC_VER) || defined(__MINGW32__)    
			DWORD tid = 100 + new_client->index;
			HANDLE myHandle = CreateThread(0, 0, TelnetClientThread, new_client, 0, &tid);
#else
			pthread_t tid = 100 + new_client->index;
			pthread_create(&tid, NULL, TelnetClientThread, (void *)new_client);
#endif


		}
		else {

			char *buf_send = (char *)"No pool available.\n";
			send(client, buf_send, (int)strlen(buf_send), 0);
#if defined(_MSC_VER) || defined(__MINGW32__)     
			closesocket(client);
#else
			shutdown(client, 2);
			close(client);
#endif

		}
	}

	return 0;
	}



void HW_TTY::start_server(hw_uart* hw_uart) {


	int i;
	//this->tty_in = queue_create();

	for (i = 0; i < pool_size; i++) {
		this->clients[i].reset();
		this->clients[i].index = i;

		//this->clients[i].baffa1_cpu = baffa1_cpu;
		this->clients[i]._hw_uart = hw_uart;
		//this->clients[i].tty_out = queue_create();
		//this->clients[i].mtx = &this->mtx;
		this->clients[i].tty_in = &this->tty_in;//queue_create();
		this->clients[i].console = &this->console;
		this->clients[i].debug_call = &this->debug_call;
	}

#if defined(_MSC_VER) || defined(__MINGW32__)        
	DWORD tid;
	HANDLE myHandle = CreateThread(0, 0, TelnetServerThread, &this->clients, 0, &tid);
#else
	pthread_t tid;
	pthread_create(&tid, NULL, TelnetServerThread, (void*)&this->clients);
#endif

	this->started = 1;
}

