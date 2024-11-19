//
// config.h
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
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baffa1_defs.h"

// WINDOW
#define EMULATOR_WINDOW_TITLE "SOL-1 Emulator"


// MEMORY
#define BAFFA1_BIOS_MEMORY_SIZE (0xFFFF + 1)
#define BAFFA1_PAGING_MEMORY_SIZE (0x7FFF + 1)
#define BAFFA1_MAIN_MEMORY_SIZE	(0x3FFFFF + 1)
#define BAFFA1_IDE_MEMORY_SIZE	(0x3FFFFF + 1)

// KEYBOARD
#define BAFFA1_TOTAL_KEYS		41

// ROMS
#define BAFFA1_ROM_NBR_ROMS 15
#define BAFFA1_ROM_TOTAL_CONTROL_BITS NBR_ROMS * 8
#define BAFFA1_ROM_CYCLES_PER_INSTR 64 
#define BAFFA1_ROM_NBR_INSTRUCTIONS 256
#define BAFFA1_ROM_DESC	0x410000
#define BAFFA1_ROM_SIZE		(0xFFFF + 1)

// SERVER
#define SERVER_PORT 20248
#define INI_SERVER 1
#define INI_WEB_SERVER 0

// PANEL GUI
#define PANEL_ENABLED 1

// MICROCODE
//#define INI_DEBUG_OPCODE 0
#define INI_DEBUG_MICROCODE 0
#define INI_DEBUG_UADDRESSER 0
#define INI_DEBUG_UADDER 0

//BUSSES
#define INI_DEBUG_BUSES 0	

//ALU
#define INI_DEBUG_UFLAGS 0
#define INI_DEBUG_ALU 0
#define INI_DEBUG_ALU_REGISTERS 0
#define INI_DEBUG_ALU_STATUS_FLAGS 0
#define INI_DEBUG_ALU_MSWh_FLAGS 0 


//REGISTERS
#define INI_DEBUG_REG_READ 0
#define INI_DEBUG_REGISTERS 0

#define INI_DEBUG_LITE 0
#define INI_DEBUG_LITE_CYCLES 0

#define INI_DEBUG_TRACE_RDREG 0
#define INI_DEBUG_TRACE_WRREG 0


#define INI_DEBUG_TRACE_RDMEM 0
#define INI_DEBUG_TRACE_WRMEM 0
#define INI_DEBUG_MEMORY 0

#define INI_DEBUG_UART 0
#define INI_DEBUG_IDE 0
#define INI_DEBUG_RTC 0
#define INI_DEBUG_TIMER 0

#define INI_DEBUG_LOG_OPCODE 0

class BAFFA1_CONFIG
{

public:
	//int DEBUG_OPCODE;
	int DEBUG_MICROCODE;
	int DEBUG_UADDRESSER;
	int DEBUG_UADDER;	

	int DEBUG_BUSES;

	int DEBUG_UFLAGS;
	int DEBUG_ALU;
	int DEBUG_ALU_REGISTERS;
	int DEBUG_ALU_STATUS_FLAGS;
	int DEBUG_ALU_MSWh_FLAGS;

	int DEBUG_REG_READ;
	int DEBUG_REGISTERS;

	int DEBUG_TRACE_RDREG;
	int DEBUG_TRACE_WRREG;	

	int DEBUG_TRACE_RDMEM;
	int DEBUG_TRACE_WRMEM;
	int DEBUG_MEMORY;

	int DEBUG_UART;
	int DEBUG_IDE;
	int DEBUG_RTC;
	int DEBUG_TIMER;

	int DEBUG_LOG_OPCODE;
	int DEBUG_LITE;
	int DEBUG_LITE_CYCLES;
	int SERVER;
	int WEB_SERVER;

	void io_file(const char *dir, const char *reg, BAFFA1_BYTE value);

	BAFFA1_CONFIG() {		

		//MICROCODE
		// DEBUG_OPCODE = INI_DEBUG_OPCODE;
		
		 DEBUG_MICROCODE = INI_DEBUG_MICROCODE;
		 DEBUG_UADDRESSER = INI_DEBUG_UADDRESSER;
		 DEBUG_UADDER = INI_DEBUG_UADDER;
		
		//BUS
		 DEBUG_BUSES = INI_DEBUG_BUSES;

		//ALU
		 DEBUG_UFLAGS = INI_DEBUG_UFLAGS;		
		 DEBUG_ALU = INI_DEBUG_ALU;
		 DEBUG_ALU_REGISTERS = INI_DEBUG_ALU_REGISTERS;
		 DEBUG_ALU_STATUS_FLAGS = INI_DEBUG_ALU_STATUS_FLAGS;
		 DEBUG_ALU_MSWh_FLAGS = INI_DEBUG_ALU_MSWh_FLAGS;


		//REGISTERS
		 DEBUG_REG_READ = INI_DEBUG_REG_READ;
		 DEBUG_REGISTERS = INI_DEBUG_REGISTERS;

		 DEBUG_TRACE_RDREG = INI_DEBUG_TRACE_RDREG;
		 DEBUG_TRACE_WRREG = INI_DEBUG_TRACE_WRREG;
		


		 DEBUG_TRACE_RDMEM = INI_DEBUG_TRACE_RDMEM;
		 DEBUG_TRACE_WRMEM = INI_DEBUG_TRACE_WRMEM;
		 DEBUG_MEMORY = INI_DEBUG_MEMORY;

		 DEBUG_UART = INI_DEBUG_UART;
		 DEBUG_IDE = INI_DEBUG_IDE;
		 DEBUG_RTC = INI_DEBUG_RTC;
		 DEBUG_TIMER = INI_DEBUG_TIMER;

		 DEBUG_LOG_OPCODE = INI_DEBUG_LOG_OPCODE;
		 DEBUG_LITE = INI_DEBUG_LITE;
		 DEBUG_LITE_CYCLES = INI_DEBUG_LITE_CYCLES;
		 SERVER = INI_SERVER;
		 WEB_SERVER = INI_WEB_SERVER;

	}
};




/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//boot sequence: bios, boot, kernel, shell
/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif