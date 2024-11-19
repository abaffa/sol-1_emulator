//
// hw_web.h
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
#ifndef HWWEB_H
#define HWWEB_H

#include "baffa1_cpu.h"
#include "hw_uart.h"

#include "queue_ll.h"

#if defined(__linux__)
#include <sys/socket.h> 
#include <arpa/inet.h>

#else // || defined(__MINGW32__)
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#endif

#include <string>



struct hw_web_client {

	int running;
	int index;
#if defined(_MSC_VER) || defined(__MINGW32__)
	SOCKET *client;
#else
	int *client;
#endif
	hw_uart* _hw_uart;
	Queue* web_out;

};

class HW_WEB {

public:


	std::string currentline;

	struct hw_web_client clients[10];

	struct net_data {
		BAFFA1_BYTE data;
	};

	HW_WEB() {
		this->currentline = "";
	}
	//void init(BAFFA1_CPU& baffa1_cpu, hw_uart *hw_uart);
	void new_char(BAFFA1_BYTE c);
	void start_server(BAFFA1_CPU& baffa1_cpu, hw_uart* hw_uart);

};
#endif