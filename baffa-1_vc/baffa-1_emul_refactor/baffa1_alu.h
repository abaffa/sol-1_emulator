//
// baffa1_alu.h
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
#ifndef BAFFA1ALU_H
#define BAFFA1ALU_H

#include "baffa1_alu_4bit.h"
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
#include "baffa1_interruptions_bus.h"
#include "baffa1_register_8bit.h"
#include "baffa1_defs.h"
#include "baffa1_config.h"

#include "hw_tty.h"

class BAFFA1_ALU
{
public:

	BAFFA1_ALU_BUS alu_bus;

	//BAFFA1_BYTE _A;
	//BAFFA1_BYTE _B;
	//BAFFA1_BYTE _C;

	BAFFA1_BYTE A;
	BAFFA1_BYTE B;

	BAFFA1_BYTE C;

	BAFFA1_BYTE CIN; //carry in

	BAFFA1_BYTE COUT; //carry out

	BAFFA1_BYTE EQ; //EQ
	BAFFA1_BYTE F; //Larger, equal, zero, carry out

	BAFFA1_BYTE u_zf;
	BAFFA1_BYTE u_cf;
	BAFFA1_BYTE u_sf;
	BAFFA1_BYTE u_of;
	//BAFFA1_BYTE u_esc;
	BAFFA1_REGISTER_8BIT U_FLAGS;


	void init();
	void reset();	

	BAFFA1_BYTE exec(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE w_bus, BAFFA1_BYTE k_bus, BAFFA1_CONFIG &cpu_config);

	void u_flags_refresh(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value, BAFFA1_ALU_BUS &alu_bus);
	   
	BAFFA1_REGISTER_8BIT Gh; // GX (16bit)	Gh/Gl	General Register(For scratch)
	BAFFA1_REGISTER_8BIT Gl; 

	BAFFA1_REGISTER_8BIT MSWh; // MSW (16bit) FLAGS
	BAFFA1_REGISTER_8BIT MSWl; // STATUS - flags de controle



	void refresh(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE data_bus, BAFFA1_BYTE u_sf, BAFFA1_CONFIG &cpu_config);
	void refresh_reg_flags_MSWh(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);

	void debugger(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty);
	void display_registers(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty);
	void mswh_flags_desc(HW_TTY &_hw_tty);
	void mswl_status_desc(HW_TTY &_hw_tty);

private:

	

	BAFFA1_BYTE get_MSWh_ZF(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_CF(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_SF(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_OF(BAFFA1_CONTROLLER_BUS &controller_bus,  BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);
	
	BAFFA1_BYTE calc_final_condition(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value);
	BAFFA1_BYTE int_pending(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE reg_status_value);

	BAFFA1_BYTE x_bus_refresh(BAFFA1_BYTE w_bus, BAFFA1_BYTE alu_a_src);
	BAFFA1_BYTE y_bus_refresh(BAFFA1_BYTE k_bus, BAFFA1_BYTE imm, BAFFA1_BYTE alu_b_src);

};
#endif