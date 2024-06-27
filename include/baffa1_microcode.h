//
// baffa1_microcode.h
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
#ifndef BAFFA1MICROCODE_H
#define BAFFA1MICROCODE_H


#include "config.h"
#include "baffa1_bus.h"
#include "baffa1_registers.h"
#include "baffa1_rom.h"
#include "baffa1_alu_board.h"
#include "hw_tty.h"

#include "baffa1_controller_bus.h"

class BAFFA1_MICROCODE {

public:

	// ROM01
	BAFFA1_BYTE next;		//TYP
	BAFFA1_BYTE u_offset;

	//ROM 02
	BAFFA1_BYTE u_escape_0;

	//ROM03
	BAFFA1_BYTE ir_wrt;

	//ROM 11
	BAFFA1_BYTE u_escape_1;

	//ROM 12
	BAFFA1_BYTE u_esc_in_src;

	//BAFFA1_DWORD u_adder_b;
	BAFFA1_DWORD u_ad_bus; BAFFA1_DWORD old_u_ad_bus;
	BAFFA1_DWORD u_adder;
	/////

	baffa1_controller_rom controller_bus;

	BAFFA1_REGISTER_8BIT IR;  // Instruction Register

	BAFFA1_REGISTER_8BIT U_ADDRESSl;  // Address Register
	BAFFA1_REGISTER_8BIT U_ADDRESSh;  // Address Register

	BAFFA1_ROM rom;
	
	BAFFA1_BYTE mux;

	void init(HW_TTY& hw_tty);	
	void reset();

	void sequencer_start(BAFFA1_BYTE any_interruption, BAFFA1_BYTE final_condition, BAFFA1_BUS& bus);
	void sequencer_end();

	void debugger(BAFFA1_CONFIG& config, HW_TTY& hw_tty);

private:	

	BAFFA1_BYTE calc_MUX(BAFFA1_BYTE typ, BAFFA1_BYTE any_interruption);

	//microcode sequence
	BAFFA1_DWORD U_ADDER(BAFFA1_BYTE u_offset, BAFFA1_DWORD u_ad, bool type_branch); //Clock up
	BAFFA1_DWORD microcode_addresser(BAFFA1_BYTE mux, BAFFA1_DWORD u_adder, BAFFA1_BYTE escape, BAFFA1_BYTE ir); //Clock up
	void load_microcode_from_rom(BAFFA1_DWORD addr); //Clock down
};
#endif

