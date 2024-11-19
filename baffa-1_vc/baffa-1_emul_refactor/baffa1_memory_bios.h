//
// baffa1_memory_board.h
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
#ifndef BAFFA1MEMORY_H
#define BAFFA1MEMORY_H

#include <stdio.h>
#include "baffa1_defs.h"
#include "baffa1_memory.h"
#include "baffa1_register_8bit.h"
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
#include "baffa1_interruptions_bus.h"

#include "baffa1_config.h"

#include "hw_tty.h"

class BAFFA1_MEMORY_BIOS
{
public:

	BAFFA1_REGISTER_8BIT PTB;  // PTB (8bit) = Page table base

	BAFFA1_MEMORY rom_bios;

	BAFFA1_MEMORY pagtbl_ram_l;
	BAFFA1_MEMORY pagtbl_ram_h;

	BAFFA1_MEMORY low_memory;

	BAFFA1_MEMORY high_memory0;
	BAFFA1_MEMORY high_memory1;
	BAFFA1_MEMORY high_memory2;
	BAFFA1_MEMORY high_memory3;
	BAFFA1_MEMORY high_memory4;
	BAFFA1_MEMORY high_memory5;
	BAFFA1_MEMORY high_memory6;
	BAFFA1_MEMORY high_memory7;

	unsigned int debug_mem_offset;
	unsigned int debug_manual_offset;

	char mem_bank[5];


	void init(HW_TTY &_hw_tty);
	void reset(HW_TTY &_hw_tty);

	int load_bios(HW_TTY &_hw_tty);

	unsigned long read_address_bus(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_BYTE MSWl_value, BAFFA1_BYTE MARl_value, BAFFA1_BYTE MARh_value, BAFFA1_BYTE MDRl_value, BAFFA1_BYTE MDRh_value);
	
	BAFFA1_BYTE read_memory(BAFFA1_MWORD addr, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_rd);
	void write_memory(BAFFA1_MWORD addr, BAFFA1_BYTE value, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_wr);
	void refresh_pagetable_mem(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE MSWl_value, BAFFA1_BYTE MARh_value, BAFFA1_BYTE MDRl_value, BAFFA1_BYTE MDRh_value, BAFFA1_CONFIG &cpu_config);

	void refresh_int_pending(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_INTERRUPTIONS_BUS& interruptions_bus, BAFFA1_BYTE MSWl_value);
	void refresh_any_interruption(BAFFA1_CONTROLLER_BUS& controller_bus);

	void debugger(BAFFA1_DWORD MAR, HW_TTY &_hw_tty);

	void display_test(BAFFA1_DWORD MAR, HW_TTY &_hw_tty);

private:

	BAFFA1_BYTE page_table_addr_src(BAFFA1_BYTE force_user_ptb, BAFFA1_BYTE MSWl_value);

	void displayMemory(BAFFA1_BYTE* mem_bytes, long offset, long current, HW_TTY &_hw_tty);

};
#endif