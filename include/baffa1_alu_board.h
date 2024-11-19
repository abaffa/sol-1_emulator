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
#include "baffa1_register_8bit.h"
#include "config.h"
#include "hw_tty.h"


class BAFFA1_ALU
{
public:
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
	BAFFA1_BYTE u_esc;
	BAFFA1_REGISTER_8BIT U_FLAGS;


	void baffa1_alu_init();
	void baffa1_alu_display_registers(struct baffa1_controller_bus *controller_bus, BAFFA1_ALU_BUS &alu_bus, HW_TTY& hw_tty);


	BAFFA1_BYTE ALU_EXEC(struct baffa1_controller_bus *controller_bus, BAFFA1_ALU_BUS &alu_bus,
		BAFFA1_BYTE u_cf, BAFFA1_BYTE msw_cf, BAFFA1_CONFIG& config, HW_TTY& hw_tty);

	void u_flags_refresh(struct baffa1_controller_bus *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG& config, HW_TTY& hw_tty);

private:
	void baffa1_alu_reset();
	
	void display_u_flags(struct baffa1_controller_bus *controller_bus, HW_TTY& hw_tty);
	void display_u_flags_lite(struct baffa1_controller_bus *controller_bus, HW_TTY& hw_tty);
	
	void update_final_condition(struct baffa1_controller_bus *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value);

	BAFFA1_BYTE int_pending(struct baffa1_controller_bus *controller_bus, BAFFA1_BYTE reg_status_value);
};
#endif