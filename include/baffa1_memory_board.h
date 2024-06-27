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
#include "baffa1_registers.h"
#include "baffa1_microcode.h"
#include "config.h"
#include "hw_tty.h"
#include "memory.h"

class BAFFA1_MEMORY
{
public:
	Memory rom_bios;

	Memory pagtbl_ram_l;
	Memory pagtbl_ram_h;

	Memory low_memory;

	Memory high_memory0;
	Memory high_memory1;
	Memory high_memory2;
	Memory high_memory3;
	Memory high_memory4;
	Memory high_memory5;
	Memory high_memory6;
	Memory high_memory7;

	/*
	BAFFA1_REGISTER_8BIT *MARh; // memory address register
	BAFFA1_REGISTER_8BIT *MARl;
	BAFFA1_REGISTER_8BIT *MDRh; // memory data register
	BAFFA1_REGISTER_8BIT *MDRl;
	*/

	unsigned int debug_mem_offset;
	unsigned int debug_manual_offset;

	char mem_bank[5];


	BAFFA1_MEMORY();
	void reset();

	int load_bios();

	unsigned long read_address_bus(BAFFA1_BYTE bus_tristate, BAFFA1_MICROCODE& microcode, BAFFA1_REGISTERS& registers);
	BAFFA1_BYTE read_memory(BAFFA1_MWORD addr, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_rd);
	void write_memory(BAFFA1_MWORD addr, BAFFA1_BYTE value, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_wr);
	void refresh_pagetable_mem(BAFFA1_MICROCODE& microcode, BAFFA1_REGISTERS& registers);

	void refresh_int_pending(BAFFA1_MICROCODE& microcode, BAFFA1_REGISTERS& registers);
	void refresh_any_interruption(BAFFA1_MICROCODE& microcode);

	void displayMainMemory(BAFFA1_REGISTERS& registers, HW_TTY& hw_tty);

	void display_test(BAFFA1_REGISTERS& registers, HW_TTY& hw_tty);

private:

	BAFFA1_BYTE page_table_addr_src(BAFFA1_BYTE force_user_ptb, BAFFA1_BYTE reg_status_value);

	void displayMemory(BAFFA1_BYTE* mem_bytes, long offset, long current, HW_TTY& hw_tty);

};
#endif