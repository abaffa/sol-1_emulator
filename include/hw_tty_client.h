//
// hw_tty_client.h
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
#ifndef HWTTYCLIENT_H
#define HWTTYCLIENT_H
#include "config.h"
#include <queue>

#if defined(__linux__)
#include <sys/socket.h>
#include <fcntl.h>

#else // || defined(__MINGW32__)
#include <mutex> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <condition_variable>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif

using namespace std;

class HW_TTY_CLIENT {

public:
	int running;
	int index;
#if defined(_MSC_VER) || defined(__MINGW32__)    
	SOCKET client;
	mutex mtx_out;
	condition_variable cv_out;

#else
	int client;
	pthread_mutex_t mtx_out;
#endif
	hw_uart* _hw_uart;
	queue<BAFFA1_BYTE> tty_out;
	queue<BAFFA1_BYTE>* tty_in;
	BAFFA1_BYTE* console;
	BAFFA1_BYTE* debug_call;

	uint8_t user_validated;

	//used during transmission
	char lastSentChar;

	//used during reception
	int receiv_total;
	int proc_receiv_count;
	uint8_t receivedCR;

	//used during IAC Commands - Telnet Protocol

	bool handleTelnetProtocol;
	bool subnegotiation;
	char telnetTerminalType[100];
	bool telnetDisableLocalEcho;
	int cmdLen;
	uint8_t cmd[4];

	char passBuffer[21];


	void reset() {

		running = 0;
		//index = -1;

		//_hw_uart = NULL;
		queue<BAFFA1_BYTE> empty; 	std::swap(tty_out, empty);
		//tty_in = NULL;
		//console = NULL;
		//debug_call = NULL;

		user_validated = 0;

		//used during transmission
		lastSentChar = '\0';

		//used during reception
		receiv_total = 0;
		proc_receiv_count = 0;
		receivedCR = 0x0;

		//used during IAC Commands - Telnet Protocol

		handleTelnetProtocol = false;
		subnegotiation = false;
		strcpy(telnetTerminalType, "vt100");
		telnetDisableLocalEcho = true;
		cmdLen = 0;
		cmd[0] = 0;		cmd[1] = 0;		cmd[2] = 0;		cmd[3] = 0;
		passBuffer[0] = '\0';
	}

	HW_TTY_CLIENT() {
		this->reset();

		index = -1;

		_hw_uart = NULL;

		tty_in = NULL;
		console = NULL;
		debug_call = NULL;

#if defined(_MSC_VER) || defined(__MINGW32__)    
		client = -1;
#else
		client = -1;
#endif

	}
};

#endif