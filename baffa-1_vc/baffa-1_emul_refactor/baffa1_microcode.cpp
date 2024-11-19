//
// baffa1_microcode.cpp
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
#include<stdio.h>

#include "baffa1_microcode.h"
#include "baffa1_utils.h"

void BAFFA1_MICROCODE::reset() {
	// ROM 0
	//controller_bus.reset();
	next = 0;
	//change MUX

	// ROM 1

	u_offset = 0;
	u_escape_0 = 0;

	// ROM 2
	ir_wrt = 0;

	// ROM 3

	// ROM 4

	// ROM 5

	// ROM 6

	// ROM 7

	// ROM 8

	// ROM 9

	// ROM 10
	u_escape_1 = 0;

	// ROM 11
	u_esc_in_src = 0;

	// ROM 12

	// ROM 13

	// ROM 14

	/////////////////////////

	mux = 0x00;

	this->u_ad_bus = 0x00;
	this->u_adder = 0x00;

	this->IR.reset();

	this->U_ADDRESSl.reset();
	this->U_ADDRESSh.reset();
}

void BAFFA1_MICROCODE::init(HW_TTY &_hw_tty) {

	this->reset();

	this->rom.init( _hw_tty);
}


BAFFA1_BYTE BAFFA1_MICROCODE::get_current_opcode() {
	return (this->u_ad_bus / 64);
}

BAFFA1_BYTE BAFFA1_MICROCODE::get_current_opcode_cycle() {
	return  (this->u_ad_bus % 64);
}





//////////////////////

BAFFA1_BYTE BAFFA1_MICROCODE::calc_MUX(BAFFA1_BYTE typ, BAFFA1_BYTE any_interruption) {

	BAFFA1_BYTE mux_A = (typ == 0b00000011) || (typ == 0b00000010 && any_interruption == 0x01);
	BAFFA1_BYTE mux_B = typ == 0b00000010;

	return set_byte_bit(mux_B, 1) | set_byte_bit(mux_A, 0);
}


// add number to internal opcode cycle address
BAFFA1_DWORD BAFFA1_MICROCODE::U_ADDER(BAFFA1_BYTE u_offset, BAFFA1_DWORD u_ad, bool type_branch) {

	BAFFA1_DWORD r_adder = 0;

	//branch  	Typ1=0	Typ0=1	Final_Condition=0
	//if type = branch, and condition = false, then next = +1
	if (type_branch)
		r_adder = u_ad + 1;								//0x0000 - 0x3FFFF


	else if (get_byte_bit(u_offset, 6))
		r_adder = u_ad + (u_offset | 0b11111111000000);	//0x3FC0 - 0x3FFF
	else
		r_adder = u_ad + u_offset;						//0x00 - 0x3F	

	return r_adder & 0x3FFF;
}

// Sets u-address
BAFFA1_DWORD BAFFA1_MICROCODE::microcode_addresser(BAFFA1_BYTE mux, BAFFA1_DWORD u_adder, BAFFA1_BYTE escape, BAFFA1_BYTE ir) {

	BAFFA1_DWORD u_address = 0x00;

	// set Opcode/cycle Rom Address
	if (mux == 0x00) { // from Adder
		u_address = u_adder;
	}
	else if (mux == 0x01) { //from IR  + Escape (if activated)
		//u_address = (get_byte_bit(escape, 0) << 4) | (get_byte_bit(escape, 1) << 5) | (ir << 6); //versao compativel com baffa-1

		// U_ESCAPE_1 não está ligado no SOL-1
		u_address = (get_byte_bit(escape, 0) << 4) | (ir << 6); //versao compativel com sol-1
	}
	else if (mux == 0x02) { // force FETCH
		u_address = 0x10;
	}
	else if (mux == 0x03) { // force TRAP
		u_address = 0x20; //0b00100000
	}


	return u_address;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

// Sets Microcode
void BAFFA1_MICROCODE::sequencer_start(BAFFA1_BYTE final_condition, BAFFA1_DATA_BUS& data_bus, BAFFA1_CONTROLLER_BUS& controller_bus) {

	//CLOCK LOW

	if (ir_wrt == 0x00) IR.set(data_bus.data);

	BAFFA1_BYTE typ = next;
	//Typ1 Typ0		Desc
	//0    0		offset
	//0    1		branch  
	//1    0		pre-fetch
	//1    1		post_fetch			
	//if type = branch, and condition = false, then next = +1
	bool type_branch = (typ == 0b01) && (final_condition == 0);
	this->u_adder = U_ADDER(u_offset, u_ad_bus, type_branch);

	mux = calc_MUX(typ, controller_bus.any_interruption);

	BAFFA1_DWORD u_address = microcode_addresser(mux, this->u_adder, this->u_escape_0 | (this->u_escape_1 << 1), this->IR.value());
	set_register(this->U_ADDRESSl, this->U_ADDRESSh, u_address); // U-Address Register

	/////////////

	this->old_u_ad_bus = this->u_ad_bus; // define old state for debuging

	if (controller_bus.cpu_reset == 0x00) // reset address - button reset
		this->u_ad_bus = u_address;
	else
		this->u_ad_bus = 0;

	//this->load_microcode_from_rom(); // Sets Microcode from Rom
}

void BAFFA1_MICROCODE::sequencer_end(BAFFA1_CONTROLLER_BUS& controller_bus) {
	if (controller_bus.cpu_reset == 0x00)
		this->load_microcode_from_rom(this->u_ad_bus, controller_bus); // Sets Microcode from Rom
	else
		this->reset();
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


// Sets Microcode from Rom
void BAFFA1_MICROCODE::load_microcode_from_rom(BAFFA1_DWORD addr, BAFFA1_CONTROLLER_BUS& controller_bus) {

	// ROM 0
	//controller_bus.next = this->rom.roms[0][addr] & 0b00000011;///////////////////////
	next = this->rom.roms[0][addr] & 0b00000011;///////////////////////
	// changes MUX

	// ROM 1
	//controller_bus.u_offset = ((this->rom.roms[1][addr] & 0b00000001) << 6) | ((this->rom.roms[0][addr] >> 2) & 0b00111111);////////////////////
	u_offset = ((this->rom.roms[1][addr] & 0b00000001) << 6) | ((this->rom.roms[0][addr] >> 2) & 0b00111111);////////////////////
	controller_bus.cond_inv = (this->rom.roms[1][addr] >> 1) & 0b00000001;
	controller_bus.cond_flags_src = (this->rom.roms[1][addr] >> 2) & 0b00000001;
	controller_bus.cond_sel = (this->rom.roms[1][addr] >> 3) & 0b00001111;
	//controller_bus.u_escape_0 = this->rom.roms[1][addr] >> 7 & 0b00000001; //ESCAPE
	u_escape_0 = this->rom.roms[1][addr] >> 7 & 0b00000001; //ESCAPE

	// ROM 2
	controller_bus.uzf_in_src = this->rom.roms[2][addr] & 0b00000011;/////////////////
	controller_bus.ucf_in_src = (this->rom.roms[2][addr] >> 2) & 0b00000011;//////////
	controller_bus.usf_in_src = (this->rom.roms[2][addr] >> 4) & 0b00000001;//////////
	controller_bus.uof_in_src = (this->rom.roms[2][addr] >> 5) & 0b00000001;//////////
	//controller_bus.ir_wrt = (this->rom.roms[2][addr] >> 6) & 0b00000001;/////////////////
	ir_wrt = (this->rom.roms[2][addr] >> 6) & 0b00000001;/////////////////
	controller_bus.status_wrt = (this->rom.roms[2][addr] >> 7) & 0b00000001;/////////////

	// ROM 3
	controller_bus.shift_src = this->rom.roms[3][addr] & 0b00000111;/////////////////////
	controller_bus.zbus_out_src = (this->rom.roms[3][addr] >> 3) & 0b00000011;///////////
	controller_bus.alu_a_src = ((this->rom.roms[3][addr] >> 5) & 0b00000111) | ((this->rom.roms[4][addr] & 0b00000111) << 3);/////////////////////

	// ROM 4
	controller_bus.alu_op = (this->rom.roms[4][addr] >> 3) & 0b00001111;//////////////
	controller_bus.alu_mode = (this->rom.roms[4][addr] >> 7) & 0b00000001;////////////

	// ROM 5
	controller_bus.alu_cf_in_src = (this->rom.roms[5][addr]) & 0b00000011;////////////
	controller_bus.alu_cf_in_inv = (this->rom.roms[5][addr] >> 2) & 0b00000001;///////
	controller_bus.zf_in_src = (this->rom.roms[5][addr] >> 3) & 0b00000011;///////////////
	controller_bus.alu_cf_out_inv = (this->rom.roms[5][addr] >> 5) & 0b00000001;//////
	controller_bus.cf_in_src = ((this->rom.roms[5][addr] >> 6) & 0b00000011) | ((this->rom.roms[6][addr] & 0b00000001) << 2);/////////////////////

	// ROM 6
	controller_bus.sf_in_src = (this->rom.roms[6][addr] >> 1) & 0b00000011;///////////////
	controller_bus.of_in_src = (this->rom.roms[6][addr] >> 3) & 0b00000111;///////////////
	controller_bus.rd = (this->rom.roms[6][addr] >> 6) & 0b00000001;//////////////////////
	controller_bus.wr = (this->rom.roms[6][addr] >> 7) & 0b00000001;//////////////////////

	// ROM 7
	controller_bus.alu_b_src = this->rom.roms[7][addr] & 0b00000111;//////////////////
	controller_bus.display_reg_load = (this->rom.roms[7][addr] >> 3) & 0b00000001;////
	controller_bus.dl_wrt = (this->rom.roms[7][addr] >> 4) & 0b00000001;//////////////
	controller_bus.dh_wrt = (this->rom.roms[7][addr] >> 5) & 0b00000001;//////////////
	controller_bus.cl_wrt = (this->rom.roms[7][addr] >> 6) & 0b00000001;//////////////
	controller_bus.ch_wrt = (this->rom.roms[7][addr] >> 7) & 0b00000001;//////////////

	// ROM 8
	controller_bus.bl_wrt = (this->rom.roms[8][addr] >> 0) & 0b00000001;//////////////
	controller_bus.bh_wrt = (this->rom.roms[8][addr] >> 1) & 0b00000001;//////////////
	controller_bus.al_wrt = (this->rom.roms[8][addr] >> 2) & 0b00000001;//////////////
	controller_bus.ah_wrt = (this->rom.roms[8][addr] >> 3) & 0b00000001;//////////////
	controller_bus.mdr_in_src = (this->rom.roms[8][addr] >> 4) & 0b00000001;//////////
	controller_bus.mdr_out_src = (this->rom.roms[8][addr] >> 5) & 0b00000001;/////////
	controller_bus.mdr_out_en = (this->rom.roms[8][addr] >> 6) & 0b00000001;//////////
	controller_bus.mdrl_wrt = (this->rom.roms[8][addr] >> 7) & 0b00000001;////////////

	// ROM 9
	controller_bus.mdrh_wrt = (this->rom.roms[9][addr] >> 0) & 0b00000001;////////////
	controller_bus.tdrl_wrt = (this->rom.roms[9][addr] >> 1) & 0b00000001;////////////
	controller_bus.tdrh_wrt = (this->rom.roms[9][addr] >> 2) & 0b00000001;////////////
	controller_bus.dil_wrt = (this->rom.roms[9][addr] >> 3) & 0b00000001;/////////////
	controller_bus.dih_wrt = (this->rom.roms[9][addr] >> 4) & 0b00000001;/////////////
	controller_bus.sil_wrt = (this->rom.roms[9][addr] >> 5) & 0b00000001;/////////////
	controller_bus.sih_wrt = (this->rom.roms[9][addr] >> 6) & 0b00000001;/////////////
	controller_bus.marl_wrt = (this->rom.roms[9][addr] >> 7) & 0b00000001;////////////

	// ROM 10
	controller_bus.marh_wrt = (this->rom.roms[10][addr] >> 0) & 0b00000001;///////////
	controller_bus.bpl_wrt = (this->rom.roms[10][addr] >> 1) & 0b00000001;////////////
	controller_bus.bph_wrt = (this->rom.roms[10][addr] >> 2) & 0b00000001;////////////
	controller_bus.pcl_wrt = (this->rom.roms[10][addr] >> 3) & 0b00000001;////////////
	controller_bus.pch_wrt = (this->rom.roms[10][addr] >> 4) & 0b00000001;////////////
	controller_bus.spl_wrt = (this->rom.roms[10][addr] >> 5) & 0b00000001;////////////
	controller_bus.sph_wrt = (this->rom.roms[10][addr] >> 6) & 0b00000001;////////////
	//controller_bus.u_escape_1 = (this->rom.roms[10][addr] >> 7) & 0b00000001;////////////
	u_escape_1 = (this->rom.roms[10][addr] >> 7) & 0b00000001;////////////

	// ROM 11
	//controller_bus.u_esc_in_src = (this->rom.roms[11][addr] >> 0) & 0b00000001;/////
	u_esc_in_src = (this->rom.roms[11][addr] >> 0) & 0b00000001;/////
	controller_bus.int_vector_wrt = (this->rom.roms[11][addr] >> 1) & 0b00000001;/////
	controller_bus.mask_flags_wrt = (this->rom.roms[11][addr] >> 2) & 0b00000001;/////
	controller_bus.mar_in_src = (this->rom.roms[11][addr] >> 3) & 0b00000001;/////////
	controller_bus.int_ack = (this->rom.roms[11][addr] >> 4) & 0b00000001;
	controller_bus.clear_all_ints = (this->rom.roms[11][addr] >> 5) & 0b00000001;
	controller_bus.ptb_wrt = (this->rom.roms[11][addr] >> 6) & 0b00000001;////////////
	controller_bus.pagtbl_ram_we = (this->rom.roms[11][addr] >> 7) & 0b00000001;//////

	// ROM 12
	controller_bus.mdr_to_pagtbl_en = (this->rom.roms[12][addr] >> 0) & 0b00000001;///
	controller_bus.force_user_ptb = (this->rom.roms[12][addr] >> 1) & 0b00000001;
	// empty bit 3
	// empty bit 4
	// empty bit 5
	// empty bit 6
	controller_bus.gl_wrt = (this->rom.roms[12][addr] >> 6) & 0b00000001;/////////////
	controller_bus.gh_wrt = (this->rom.roms[12][addr] >> 7) & 0b00000001;/////////////

	// ROM 13
	controller_bus.imm = this->rom.roms[13][addr]; ///////////////////////////////////

	// ROM 14
	// empty bit 0
	// empty bit 1
	// empty bit 2
	// empty bit 3
	// empty bit 4
	// empty bit 5
	// empty bit 6
	// empty bit 7
}

void BAFFA1_MICROCODE::debugger(BAFFA1_BYTE final_condition, BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty) {

	char str_out[255];


	if (cpu_config.DEBUG_MICROCODE) {


		BAFFA1_REGISTER_8BIT IR;  // Instruction Register

		BAFFA1_REGISTER_8BIT U_ADDRESSl;  // Address Register
		BAFFA1_REGISTER_8BIT U_ADDRESSh;  // Address Register

		str_out[0] = '\0';

		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ MICROCODE\n\n");

		//_hw_tty.print("U-ADDRESS: ");  print_word_bin(str_out, this->u_ad_bus); _hw_tty.print(str_out); _hw_tty.print("\n");		
		//sprintf(str_out, "OPCODE: %02x (cycle %02x)\n", (this->u_ad_bus / 64), (this->u_ad_bus % 64)); _hw_tty.print(str_out);
		//_hw_tty.print("microcode: \n");
		this->rom.display_current_cycles_desc((this->u_ad_bus / 64), (this->u_ad_bus % 64), _hw_tty);
		_hw_tty.print("\n");

	}
	if (cpu_config.DEBUG_UADDRESSER) {
		_hw_tty.print("+------------------------------------------------------------------------------\n");
		char str_out[255];
		_hw_tty.print("+ U_ADDRESSER\n\n");

		_hw_tty.print("  Next(typ): "); print_nibble_bin(str_out, next); _hw_tty.print(str_out);
		_hw_tty.print(" | Any Interruption: "); print_nibble_bin(str_out, controller_bus.any_interruption); _hw_tty.print(str_out);
		//_hw_tty.print(" | Mux_B: "); print_nibble_bin(str_out, mux_B); _hw_tty.print(str_out);
		//_hw_tty.print(" | Mux_A: "); print_nibble_bin(str_out, mux_A); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("  Mux: ");
		print_nibble_bin(str_out, this->mux); _hw_tty.print(str_out);
		_hw_tty.print("  | U_ESC: "); print_nibble_bin(str_out, u_escape_0 | (u_escape_1 << 1)); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");
		sprintf(str_out, "  REG U_ADDRESS=%04x", value_register(this->U_ADDRESSl, this->U_ADDRESSh)); _hw_tty.print(str_out);
		_hw_tty.print(" | ");
		sprintf(str_out, "U_AD=%04x", this->u_ad_bus); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		sprintf(str_out, "  REG IR=%02x", this->IR.value()); _hw_tty.print(str_out);
		_hw_tty.print("  | IR_WRT: "); sprintf(str_out, "%d", ir_wrt); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");

	}
	if (cpu_config.DEBUG_UADDER) {
		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ U_ADDER\n\n");

		//BAFFA1_BYTE typ = this->controller_bus.next;
		BAFFA1_BYTE typ = next;
		char str_out[255];
		_hw_tty.print("  next(typ): "); print_nibble_bin(str_out, typ); _hw_tty.print(str_out);
		_hw_tty.print(" | ");
		_hw_tty.print(" u_offset: "); print_byte_bin(str_out, u_offset); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		bool type_branch = (typ == 0b01) && (final_condition == 0);
		_hw_tty.print("  Type_Branch: "); sprintf(str_out, "%d", type_branch); _hw_tty.print(str_out);
		_hw_tty.print(" | ");
		_hw_tty.print("Final Condition: "); print_nibble_bin(str_out, final_condition); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		_hw_tty.print("\n");
		_hw_tty.print("  A(u_ad): ");  print_word_bin_nibbles(str_out, this->u_ad_bus); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		//_hw_tty.print("* B: ");  print_word_bin_nibbles(str_out, this->u_adder_b); _hw_tty.print(str_out);
		//_hw_tty.print("\n");
		_hw_tty.print("  u_adder: ");  print_word_bin(str_out, this->u_adder); _hw_tty.print(str_out);
		sprintf(str_out, " | u_esc_in_src: %02x", u_esc_in_src); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}
}


