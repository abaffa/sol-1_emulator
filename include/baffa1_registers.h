//
// baffa1_registers.h
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
#ifndef BAFFA1REGISTERS_H
#define BAFFA1REGISTERS_H

#include "config.h"
#include "baffa1_register_8bit.h"
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
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

class BAFFA1_REGISTERS
{

public:
	//General Purpose Registers
	//DATA REGISTERS
	BAFFA1_REGISTER_8BIT Ah; // AX (16bit) Accumulator	(Ah/Al)
	BAFFA1_REGISTER_8BIT Al;
	BAFFA1_REGISTER_8BIT Bh; // BX (16bit) Base		(Bh/Bl)
	BAFFA1_REGISTER_8BIT Bl;
	BAFFA1_REGISTER_8BIT Ch; // CX (16bit) Counter		(Ch/Cl)
	BAFFA1_REGISTER_8BIT Cl;
	BAFFA1_REGISTER_8BIT Dh; // DX (16bit) Data		(Dh/Dl)
	BAFFA1_REGISTER_8BIT Dl;
	BAFFA1_REGISTER_8BIT Gh; // GX (16bit)	Gh/Gl	General Register(For scratch)
	BAFFA1_REGISTER_8BIT Gl;

	//Pointer Registers
	BAFFA1_REGISTER_8BIT BPh; // BP (16bit) Base Pointer  (Used to manage stack frames)
	BAFFA1_REGISTER_8BIT BPl;
	BAFFA1_REGISTER_8BIT SPh; // SP (16bit) Stack Pointer
	BAFFA1_REGISTER_8BIT SPl;
	BAFFA1_REGISTER_8BIT SSPh; // SSP (16bit) Supervisor Stack Pointer
	BAFFA1_REGISTER_8BIT SSPl;

	//Index Registers
	BAFFA1_REGISTER_8BIT SIh; // SI (16bit) Source index (Source address for string operations)
	BAFFA1_REGISTER_8BIT SIl;
	BAFFA1_REGISTER_8BIT DIh; // DI (16bit) Destination Index (Destination address for string operations)
	BAFFA1_REGISTER_8BIT DIl;
	BAFFA1_REGISTER_8BIT PCh; // PC (16bit) Program Counter
	BAFFA1_REGISTER_8BIT PCl;

	BAFFA1_REGISTER_8BIT TDRh; // TDR (16bit) Temporary Data Register
	BAFFA1_REGISTER_8BIT TDRl;
	BAFFA1_REGISTER_8BIT PTB;  // PTB (8bit) = Page table base



	BAFFA1_REGISTER_8BIT MSWh; // MSW (16bit) FLAGS
	BAFFA1_REGISTER_8BIT MSWl; // STATUS - flags de controle


	BAFFA1_REGISTER_8BIT INT_MASKS; // INT FLAGS
	


	BAFFA1_REGISTER_8BIT MARh; // memory address register
	BAFFA1_REGISTER_8BIT MARl;
	BAFFA1_REGISTER_8BIT MDRh; // memory data register
	BAFFA1_REGISTER_8BIT MDRl;

	//Control Registers (FLAGS)
		//ZF // Zero Flag
		//CF // Carry Flag
		//SF // Sign Flag
		//OF // Overflow Flag

	//////////////////////////////////////////////////
    //unsigned char S;       /*flag S     Result negative         */
    //unsigned char Z;       /*flag Z     Result is zero          */
    //unsigned char H;       /*flag H     Halfcarry/Halfborrow    */
    //unsigned char P;       /*flag P     Result is even          */
    //unsigned char V;       /*flag V     Overflow occured        */
    //unsigned char N;       /*flag N     Subtraction occured     */
    //unsigned char C;       /*flag C     Carry/Borrow occured    */


	BAFFA1_REGISTERS();

	static BAFFA1_DWORD value(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h);
	static void set(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h, BAFFA1_DWORD v);
	static void reset(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h);
	
	void mswh_flags_desc(HW_TTY& hw_tty);
	void mswl_status_desc(HW_TTY& hw_tty);

	void refresh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE data_bus, BAFFA1_BYTE u_sf, BAFFA1_CONFIG& config, FILE *fa);
	void refresh_reg_flags_MSWh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);


private:

	BAFFA1_BYTE get_MSWh_ZF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_CF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_SF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus);
	BAFFA1_BYTE get_MSWh_OF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf);

};
#endif