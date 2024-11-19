//
// baffa1_rom.h
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
#ifndef BAFFA1ROM_H
#define BAFFA1ROM_H

#include <stdio.h>
#include "baffa1_defs.h"
#include "baffa1_config.h"

#include "hw_tty.h"

class BAFFA1_ROM
{

public:

	BAFFA1_BYTE *rom_desc;
	BAFFA1_BYTE **roms;

	BAFFA1_MWORD MAR; // memory address register

	int debug_mem_offset;

	BAFFA1_BYTE bkpt_opcode;
	BAFFA1_BYTE bkpt_cycle;

	void init(HW_TTY &_hw_tty); //reset

	void display_current_cycles(BAFFA1_BYTE opcode, BAFFA1_BYTE cycle, BAFFA1_BYTE debug_desc_type, HW_TTY &_hw_tty);
	void display_current_cycles_desc(BAFFA1_BYTE opcode, BAFFA1_BYTE cycle, HW_TTY &_hw_tty);
	void debug_cycles(HW_TTY &_hw_tty);

private:
	void is_rom_in_bounds(BAFFA1_MWORD index);
	int load_rom(const char* filename, BAFFA1_BYTE* rom, HW_TTY &_hw_tty);
	void menu(BAFFA1_BYTE debug_desc_type, HW_TTY &_hw_tty);

};



#endif