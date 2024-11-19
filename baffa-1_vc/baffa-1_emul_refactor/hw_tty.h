//
// hw_tty.h
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
#ifndef HWTTY_H
#define HWTTY_H

#define pool_size 10

#define tty_password "caceta"

#include "baffa1_defs.h"
#include "hw_uart.h"
#include "hw_tty_client.h"
#include <queue>

#include "baffa1_utils.h"

#if defined(__linux__)
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h> 
#include <unistd.h>

#else //  || defined(__MINGW32__)
#include <mutex> 
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif


using namespace std;




class HW_TTY {


public:

	HW_TTY_CLIENT clients[10];

	struct net_data {
		BAFFA1_BYTE data;
	};

	BAFFA1_BYTE started;
	BAFFA1_BYTE debug_call;
	BAFFA1_BYTE console;

	//void init(hw_uart *hw_uart);
	void start_server(hw_uart* hw_uart);

	queue<BAFFA1_BYTE> tty_in;

	/*
	void send(BAFFA1_BYTE b);
	BAFFA1_BYTE receive();

	void print(const char* s);

	void set_input(BAFFA1_BYTE b);

	char* gets(int max_value);
	char* getline();
	char get_char();
	*/

	HW_TTY() {
		started = 1;
		debug_call = 0;
		console = 0;
	}



	void send(BAFFA1_BYTE b) {

		if (this->started == 1) {
			int i;
			for (i = 0; i < pool_size; i++) {
				if (this->clients[i].running == 1) {  // SEND TELNET
					BAFFA1_BYTE data = b;
#if defined(_MSC_VER) || defined(__MINGW32__)    
					std::unique_lock<std::mutex> lock(this->clients[i].mtx_out);
#else
					//pthread_mutex_lock(&this->clients[i].mtx_out);
#endif
					this->clients[i].tty_out.push(data);

#if defined(_MSC_VER) || defined(__MINGW32__)    
					this->clients[i].cv_out.notify_all();
#else
					//pthread_mutex_unlock(&this->clients[i].mtx_out);
#endif
				}
			}
		}
	}

	void print(const char* s) {

		if (this->started == 1) {
			int i = 0;
			while (s[i] != '\0') {
				if (s[i] == '\n') {
					send('\r');
					send(s[i]);
				}
				else
					send(s[i]);
				i++;
			}
		}
		printf("%s", s);
	}





	void set_input(BAFFA1_BYTE b) {

		/*
		if (this->started == 1) {
			int i;
			for (i = 0; i < pool_size; i++) {
				if (clients[i].running == 1) {  // SEND TELNET
					clients[i].console = b;
				}
			}
		}
		*/
		this->console = b;
	}

	char* gets(int max_value) {

		char str_out[255];
		char *input = (char*)malloc(sizeof(char) * 257);

		int i = 0;
		for (i = 0; i < 256 && i < max_value; ) {
			char cur_input = get_char();
			if (cur_input == (char)8) {
				if (i > 0) {
					sprintf(str_out, "%c", cur_input);
					sprintf(str_out, " ");
					sprintf(str_out, "%c", cur_input);
					print(str_out);
					i--;
				}
			}
			else if (cur_input != '\n' && cur_input != '\r') {
				cur_input = toupper(cur_input);
				sprintf(str_out, "%c", cur_input);
				print(str_out);
				input[i] = cur_input;
				i++;
			}
			else {
				print("\r\n");
				break;
			}
		}
		input[i] = '\0';

		return input;
	}

	char* getline() {
		char str_out[255];
		char *input = (char*)malloc(sizeof(char) * 257);

		int i = 0;
		for (i = 0; i < 256; ) {
			char cur_input = get_char();
			if (cur_input == (char)8) {
				if (i > 0) {
					sprintf(str_out, "%c", cur_input);
					sprintf(str_out, " ");
					sprintf(str_out, "%c", cur_input);
					print(str_out);
					i--;
				}
			}
			else if (cur_input != '\n' && cur_input != '\r') {
				cur_input = toupper(cur_input);
				sprintf(str_out, "%c", cur_input);
				print(str_out);
				input[i] = cur_input;
				i++;
			}
			else {
				print("\r\n");
				break;
			}
		}
		input[i] = '\0';

		return input;
	}


	BAFFA1_BYTE receive() {

		BAFFA1_BYTE ch = 0x00;
		set_input(1);
		while (1) {
#if defined(_MSC_VER) || defined(__MINGW32__)     
			if (_kbhit()) {
				ch = _getch();
#else
			if (kbhit()) {
				ch = getch();
#endif
				break;
			}
			else if (!this->tty_in.empty()) {

				BAFFA1_BYTE data = this->tty_in.front(); this->tty_in.pop();
				ch = data;
				break;
			}

#if defined(_MSC_VER) || defined(__MINGW32__)     
			Sleep(10);
#else
			int milliseconds = 10;
			struct timespec ts;
			ts.tv_sec = milliseconds / 1000;
			ts.tv_nsec = (milliseconds % 1000) * 1000000;
			nanosleep(&ts, NULL);

#endif
		}
		set_input(0);

		return ch;
	}


	char get_char() {

		char cur_input = receive();
		return cur_input;
	}
};
#endif