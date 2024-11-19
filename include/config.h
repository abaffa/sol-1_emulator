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
#include <ctype.h>

#define EMULATOR_WINDOW_TITLE "SOL-1 Emulator"

#define BAFFA1_BIOS_MEMORY_SIZE (0xFFFF + 1)
#define BAFFA1_PAGING_MEMORY_SIZE (0x7FFF + 1)
#define BAFFA1_MAIN_MEMORY_SIZE	(0x3FFFFF + 1)


#define BAFFA1_IDE_MEMORY_SIZE	(0x3FFFFF + 1)

#define BAFFA1_ROM_SIZE		(0xFFFF + 1)
#define BAFFA1_TOTAL_KEYS		41


// ROMS
#define BAFFA1_ROM_NBR_ROMS 15
#define BAFFA1_ROM_TOTAL_CONTROL_BITS NBR_ROMS * 8
#define BAFFA1_ROM_CYCLES_PER_INSTR 64 
#define BAFFA1_ROM_NBR_INSTRUCTIONS 256
#define BAFFA1_ROM_DESC	0x410000

#define BAFFA1_BYTE				unsigned char // 8bit
#define BAFFA1_MWORD				unsigned int //Memory Word (int 32bit, short 16bit, char 8bit)
#define BAFFA1_DWORD				unsigned int //Data Word


#define SERVER_PORT 20248

#define PANEL_ENABLED 0

#define INI_SERVER 1
#define INI_WEB_SERVER 0

//#define INI_DEBUG_OPCODE 0

#define INI_DEBUG_MICROCODE 0
#define INI_DEBUG_UADDRESSER 0
#define INI_DEBUG_UADDER 0
#define INI_DEBUG_UFLAGS 0


#define INI_DEBUG_LITE 0
#define INI_DEBUG_LITE_CYCLES 0

#define INI_DEBUG_BUSES 0	
#define INI_DEBUG_ALU 0

#define INI_DEBUG_TRACE_RDREG 0
#define INI_DEBUG_TRACE_WRREG 0
#define INI_DEBUG_REGISTERS 0

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
	int DEBUG_MICROCODE;
	int DEBUG_UADDRESSER;
	int DEBUG_UADDER;
	int DEBUG_UFLAGS;

	int DEBUG_BUSES;
	int DEBUG_ALU;

	int DEBUG_TRACE_RDREG;
	int DEBUG_TRACE_WRREG;
	int DEBUG_REGISTERS;


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
};


/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//boot sequence: bios, boot, kernel, shell
/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif