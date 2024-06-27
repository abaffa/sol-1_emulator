//
// baffa1_cpu.h
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
#ifndef BAFFA1CPU_H
#define BAFFA1CPU_H

#include "config.h"
#include "baffa1_memory_board.h"
#include "baffa1_microcode.h"
#include "baffa1_registers.h"
#include "baffa1_alu_board.h"
#include "hw_tty.h"
#include <stddef.h>

class BAFFA1_CPU
{

public:

	BAFFA1_MEMORY memory;
	BAFFA1_REGISTERS registers;
	BAFFA1_MICROCODE microcode;

	BAFFA1_ALU alu;

	char last_op_desc[512];

	BAFFA1_MWORD BKPT; //breakpoint (FFFF = disable)

	BAFFA1_CONFIG config;


	int display_reg_load;


	void init(HW_TTY& hw_tty);

	void reset();
	void memory_display(HW_TTY& hw_tty);


	void display_registers(HW_TTY& hw_tty);
	void display_registers_lite(HW_TTY& hw_tty);


	Memory get_current_memory();

	unsigned int get_current_memory_size();




};
#endif