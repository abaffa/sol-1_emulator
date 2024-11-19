//
// main.cpp
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
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
#include "baffa1_interruptions.h"

#include "baffa1_utils.h"

int main(int argc, char** argv) {

	BAFFA1_CONFIG cpu_config;
	//cpu_config.DEBUG_OPCODE = 1;
	cpu_config.DEBUG_MICROCODE = 1;
	cpu_config.DEBUG_UADDRESSER = 1;
	cpu_config.DEBUG_UADDER = 1;

	cpu_config.DEBUG_BUSES = 1;

	cpu_config.DEBUG_UFLAGS = 1;
	cpu_config.DEBUG_ALU = 1;
	cpu_config.DEBUG_ALU_REGISTERS = 1;
	cpu_config.DEBUG_ALU_STATUS_FLAGS = 1;
	cpu_config.DEBUG_ALU_MSWh_FLAGS = 1;

	cpu_config.DEBUG_REG_READ = 1;
	cpu_config.DEBUG_REGISTERS = 1;

	cpu_config.DEBUG_TRACE_RDREG = 1;
	cpu_config.DEBUG_TRACE_WRREG = 1;

	cpu_config.DEBUG_TRACE_RDMEM = 1;
	cpu_config.DEBUG_TRACE_WRMEM = 1;
	cpu_config.DEBUG_MEMORY = 1;

	cpu_config.DEBUG_UART = 1;
	cpu_config.DEBUG_IDE = 1;
	cpu_config.DEBUG_RTC = 1;
	cpu_config.DEBUG_TIMER = 1;

	cpu_config.DEBUG_LOG_OPCODE = 1;
	cpu_config.DEBUG_LITE = 1;
	cpu_config.DEBUG_LITE_CYCLES = 1;


	BAFFA1_CONTROLLER_BUS controller_bus;
	BAFFA1_ALU_BUS alu_bus;

	BAFFA1_INTERRUPTIONS interruptions;
	interruptions.cpu_config = cpu_config;

	interruptions.init();
	controller_bus.reset();
	alu_bus.reset();



	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

	cout << "Press enter to continue or 'q' enter to quit..." << endl;
	char c = cin.get();

	//if (c == 'q') break;
//}


	return 0;
}
