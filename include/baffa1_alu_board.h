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

// FLAG - msw-h - bits
#define MSWh_ZF 0x00 // ZeroFlag
#define MSWh_CF 0x01 // Carry Flag
#define MSWh_SF 0x02 // Overflow Flag
#define MSWh_OF 0x03 // Sign Flag
#define MSWh_12 0x04
#define MSWh_13 0x05
#define MSWh_14 0x06
#define MSWh_15 0x07

// STATUS FLAGS - msw-l bits
#define MSWl_DMA_ACK 0x00
#define MSWl_INTERRUPT_ENABLE 0x01
#define MSWl_CPU_MODE 0x02
#define MSWl_PAGING_EN 0x03
#define MSWl_HALT 0x04
#define MSWl_DISPLAY_REG_LOAD 0x05
#define MSWl_14 0x06
#define MSWl_DIR 0x07

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


	void baffa1_alu_init();
	void baffa1_alu_display_registers(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS &alu_bus, HW_TTY& hw_tty);


	BAFFA1_BYTE ALU_EXEC(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE w_bus, BAFFA1_BYTE k_bus, 
		BAFFA1_BYTE u_cf, BAFFA1_BYTE msw_cf, BAFFA1_CONFIG& config, HW_TTY& hw_tty, FILE *fa);

	void u_flags_refresh(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG& config, HW_TTY& hw_tty);
	   
	BAFFA1_REGISTER_8BIT Gh; // GX (16bit)	Gh/Gl	General Register(For scratch)
	BAFFA1_REGISTER_8BIT Gl; 

	BAFFA1_REGISTER_8BIT MSWh; // MSW (16bit) FLAGS
	BAFFA1_REGISTER_8BIT MSWl; // STATUS - flags de controle

	void mswh_flags_desc(HW_TTY& hw_tty);
	void mswl_status_desc(HW_TTY& hw_tty);

	void refresh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE data_bus, BAFFA1_BYTE u_sf, BAFFA1_CONFIG& config, FILE *fa);
	void refresh_reg_flags_MSWh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);


private:

	

	BAFFA1_BYTE get_MSWh_ZF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_CF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_SF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_OF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);

	void baffa1_alu_reset();
	
	void display_debug_alu(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_BYTE u_cf, BAFFA1_BYTE msw_cf, HW_TTY& hw_tty);
	void display_u_flags(struct baffa1_controller_rom *controller_bus, HW_TTY& hw_tty);
	void display_u_flags_lite(struct baffa1_controller_rom *controller_bus, HW_TTY& hw_tty);
	
	BAFFA1_BYTE calc_final_condition(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value);

	BAFFA1_BYTE int_pending(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value);


	BAFFA1_BYTE x_bus_refresh(BAFFA1_BYTE w_bus, BAFFA1_BYTE alu_a_src);

	BAFFA1_BYTE y_bus_refresh(BAFFA1_BYTE k_bus, BAFFA1_BYTE imm,
		BAFFA1_BYTE alu_b_src);
};
#endif