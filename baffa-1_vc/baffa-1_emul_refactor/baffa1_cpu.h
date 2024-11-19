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

#include "baffa1_config.h"

#include "baffa1_microcode.h"
#include "baffa1_alu.h"
#include "baffa1_registers.h"
#include "baffa1_memory_bios.h"
#include "baffa1_interruptions.h"

#include "hw_tty.h"

#include <stddef.h>

class BAFFA1_CPU
{

public:

	
	BAFFA1_MICROCODE microcode;
	BAFFA1_ALU alu;
	BAFFA1_REGISTERS registers;
	BAFFA1_MEMORY_BIOS memory_bios;
	BAFFA1_INTERRUPTIONS interruptions;

	BAFFA1_CONTROLLER_BUS controller_bus;
	//BAFFA1_ALU_BUS alu_bus;
	//BAFFA1_REGISTERS_BUS registers_bus;
	BAFFA1_DATA_BUS data_bus;
	BAFFA1_INTERRUPTIONS_BUS interruptions_bus;

	char last_op_desc[512];

	BAFFA1_MWORD BKPT; //breakpoint (FFFF = disable)	

	int display_reg_load;

	void init(BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty);
	void reset(HW_TTY &_hw_tty);

	BAFFA1_MEMORY get_current_memory();
	unsigned int get_current_memory_size();
};
#endif