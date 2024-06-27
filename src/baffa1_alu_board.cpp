//
// baffa1_alu.cpp
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
#include "baffa1_alu_board.h"
#include "baffa1_registers.h"
#include "utils.h"

void BAFFA1_ALU::baffa1_alu_reset() {

	//this->_A = 0x00;
	//this->_B = 0x00;
	//this->_C = 0x00;

	this->A = 0x00;
	this->B = 0x00;

	this->C = 0x00;

	this->CIN = 0x00; //carry in

	this->COUT = 0x00; //carry in

	this->EQ = 0x00; //carry in
	this->F = 0x00; //Larger, equal, zero, carry out

}

void BAFFA1_ALU::baffa1_alu_init() {


	this->baffa1_alu_reset();

	this->u_zf = 0x00;
	this->u_cf = 0x00;
	this->u_sf = 0x00;
	this->u_of = 0x00;
	//this->u_esc = 0x00;
	this->U_FLAGS.reset();
}

BAFFA1_BYTE BAFFA1_ALU::ALU_EXEC(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS &alu_bus,
	BAFFA1_BYTE u_cf, BAFFA1_BYTE msw_cf, BAFFA1_CONFIG& config, HW_TTY& hw_tty) {

	////////////////////////////////////////////////////////////////
	//
	// Set C_in
	//
	BAFFA1_BYTE alu_cin = 0x00;

	if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x00)
		alu_cin = 1;
	else {
		if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x01)
			alu_cin = msw_cf;

		else if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x02)
			alu_cin = u_cf;

		else if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x03)
			alu_cin = 0;
	}

	alu_cin = (alu_cin ^ controller_bus->alu_cf_in_inv) & 0b00000001;

	////////////////////////////////////////////////////////////////
	//
	// Calculate ALU
	//
	this->A = alu_bus.x_bus;
	this->B = alu_bus.y_bus;
	this->CIN = (alu_cin) & 0b00000001;

	BAFFA1_BYTE ALU_S = controller_bus->alu_op;
	BAFFA1_BYTE ALU_M = controller_bus->alu_mode;

	baffa1_alu_4bit ALUl;
	BAFFA1_BYTE ALUl_A = this->A & 0b00001111;
	BAFFA1_BYTE ALUl_B = this->B & 0b00001111;
	BAFFA1_BYTE ALUl_CIN = this->CIN;
	baffa1_alu_4bit_op(&ALUl, ALUl_A, ALUl_B, ALUl_CIN, ALU_S, ALU_M);

	baffa1_alu_4bit ALUh;
	BAFFA1_BYTE ALUh_A = (this->A & 0b11110000) >> 4;
	BAFFA1_BYTE ALUh_B = (this->B & 0b11110000) >> 4;
	BAFFA1_BYTE ALUh_CIN = (ALUl.COUT) & 0b00000001;
	baffa1_alu_4bit_op(&ALUh, ALUh_A, ALUh_B, ALUh_CIN, ALU_S, ALU_M);

	this->C = (ALUl.F & 0b00001111) | ((ALUh.F & 0b00001111) << 4); 
	this->COUT = ALUh.COUT;

	alu_bus.alu_output = this->C;

	/////////////////////////////////////////////////////////////////
	//
	// ALU_CF (Carry Flag)
	//

	alu_bus.alu_cf = this->COUT;
	alu_bus.alu_final_cf = (alu_bus.alu_cf ^ (controller_bus->alu_cf_out_inv)) & 0b00000001;


	/////////////////////////////////////////////////////////////////
	//
	// SHIFT
	//
	/*
	SHL: MSB goes into CF.LSB replaced by 0
	SLA : same as SHL
	ROL : LSB becomes MSB.
	RLC : MSB goes into CF.LSB becomes CF

	SHR : LSB goes into CF.MSB replaced by 0
	SRA : LSB goes into CF.MSB is whatever the previous one was
	ROR : MSB becomes LSB
	RRC : MSB becomes CF.LSB goes into CF
	*/

	alu_bus.z_bus = 0x00;

	BAFFA1_BYTE inIC16 = 0x00;

	if (controller_bus->shift_src == 0x00) inIC16 = 0x00;
	else if (controller_bus->shift_src == 0x01) inIC16 = u_cf;
	else if (controller_bus->shift_src == 0x02) inIC16 = msw_cf;
	else if (controller_bus->shift_src == 0x03) inIC16 = get_byte_bit(alu_bus.alu_output, 0);
	else if (controller_bus->shift_src == 0x04) inIC16 = get_byte_bit(alu_bus.alu_output, 7);
	else if (controller_bus->shift_src == 0x05) inIC16 = 0x01;
	else if (controller_bus->shift_src == 0x06) inIC16 = 0x01;
	else if (controller_bus->shift_src == 0x07) inIC16 = 0x01;


	if ((controller_bus->zbus_out_src & 0b00000011) == 0x00)
		alu_bus.z_bus = alu_bus.alu_output;

	else if ((controller_bus->zbus_out_src & 0b00000011) == 0x01)
		alu_bus.z_bus = (alu_bus.alu_output >> 1) | (inIC16 << 7);

	else if ((controller_bus->zbus_out_src & 0b00000011) == 0x02)
		alu_bus.z_bus = (alu_bus.alu_output << 1) | inIC16;

	else if ((controller_bus->zbus_out_src & 0b00000011) == 0x03)
		alu_bus.z_bus = get_byte_bit(alu_bus.alu_output, 7) != 0x00 ? 0b11111111 : 0b00000000;
	

	/////////////////////////////////////////////////////////////////
	//
	// ALU_OF (Overflow Flag)
	//
	BAFFA1_BYTE zNEQx = (get_byte_bit(alu_bus.z_bus, 7) != get_byte_bit(alu_bus.x_bus, 7));
	BAFFA1_BYTE xNEQy = (get_byte_bit(alu_bus.x_bus, 7) != get_byte_bit(alu_bus.y_bus, 7));

	alu_bus.alu_of = zNEQx && ((controller_bus->alu_op != 0b1001) == xNEQy);
	//
	/////////////////////////////////////////////////////////////////
	//
	// ALU_ZF (Zero Flag)
	//
	alu_bus.alu_zf = (alu_bus.z_bus == 0x00);
	/////////////////////////////////////////////////////////////////

	if (config.DEBUG_ALU) {
		display_debug_alu(controller_bus, alu_bus, u_cf, msw_cf, hw_tty);
	}

	return alu_bus.z_bus;
}





void BAFFA1_ALU::u_flags_refresh(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG& config, HW_TTY& hw_tty)
{

	BAFFA1_BYTE inZF = 0x00;
	BAFFA1_BYTE inCF = 0x00;
	BAFFA1_BYTE inSF = 0x00;
	BAFFA1_BYTE inOF = 0x00;

	switch (controller_bus->uzf_in_src) {
	case 0x00:
		inZF = get_byte_bit(this->u_zf, 0); // unchanged
		break;
	case 0x01:
		inZF = get_byte_bit(alu_bus.alu_zf, 0);
		break;
	case 0x02:
		inZF = get_byte_bit(this->u_zf, 0) & get_byte_bit(alu_bus.alu_zf, 0);
		break;
	case 0x03:
		inZF = 0; // GND
		break;
	}

	switch (controller_bus->ucf_in_src) {
	case 0x00:
		inCF = get_byte_bit(this->u_cf, 0);// unchanged
		break;
	case 0x01:
		inCF = get_byte_bit(alu_bus.alu_final_cf, 0);
		break;
	case 0x02:
		inCF = get_byte_bit(alu_bus.alu_output, 0);
		break;
	case 0x03:
		inCF = get_byte_bit(alu_bus.alu_output, 7);
		break;
	}

	if (controller_bus->usf_in_src == 0x00)// unchanged
		inSF = get_byte_bit(this->u_sf, 0);
	else
		inSF = get_byte_bit(alu_bus.z_bus, 7); 

	if (controller_bus->uof_in_src == 0x00)
		inOF = get_byte_bit(this->u_of, 0);// unchanged
	else
		inOF = get_byte_bit(alu_bus.alu_of, 0);


	this->u_zf = inZF;
	this->u_cf = inCF;
	this->u_sf = inSF;
	this->u_of = inOF;


	////// PQ ESTA AQUI??
	//if (controller_bus->u_esc_in_src != 0x00) // byte do escape 
		//this->u_esc = controller_bus->imm & 0b00000011;

	// nao posso usar final condition sem ter atualizado o MSW
	alu_bus.final_condition = this->calc_final_condition(controller_bus, reg_status_value, reg_flags_value);
	//////

	if (config.DEBUG_UFLAGS) {
		hw_tty.print("***** U_FLAGS\n");
		display_u_flags(controller_bus, hw_tty);
	}
}


// usado apenas pelo update_final_condition
BAFFA1_BYTE BAFFA1_ALU::int_pending(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value) {
	return get_byte_bit(controller_bus->int_request, 0) & get_byte_bit(reg_status_value, MSWl_INTERRUPT_ENABLE);
}




BAFFA1_BYTE BAFFA1_ALU::calc_final_condition(struct baffa1_controller_rom *controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value) {
	
	BAFFA1_BYTE condition = 0x0;

	if (!get_byte_bit(controller_bus->cond_sel, 3)) {
		BAFFA1_BYTE inZF = 0x00;
		BAFFA1_BYTE inCF = 0x00;
		BAFFA1_BYTE inSF = 0x00;
		BAFFA1_BYTE inOF = 0x00;

		//IC_U_CODE_FLAGS
		if (controller_bus->cond_flags_src == 0x00) {
			inZF = get_byte_bit(reg_flags_value, MSWh_ZF);
			inCF = get_byte_bit(reg_flags_value, MSWh_CF);
			inSF = get_byte_bit(reg_flags_value, MSWh_SF);
			inOF = get_byte_bit(reg_flags_value, MSWh_OF);
		}
		else {
			inZF = get_byte_bit(this->u_zf, 0); //0
			inCF = get_byte_bit(this->u_cf, 0); //1
			inSF = get_byte_bit(this->u_sf, 0); //2
			inOF = get_byte_bit(this->u_of, 0); //3
		}

		//IC_XOR_2, IC_OR_1
		BAFFA1_BYTE SFneqOF = inSF ^ inOF; //XOR //4
		BAFFA1_BYTE ZForSFneqOF = inZF | SFneqOF; //5
		BAFFA1_BYTE ZForCF = inZF | inCF;		 //6
		

		//IC_CMP1
		switch (controller_bus->cond_sel & 0b00000111) {
		case 0x00:
			condition = get_byte_bit(inZF, 0);
			break;

		case 0x01:
			condition = get_byte_bit(inCF, 0);
			break;

		case 0x02:
			condition = get_byte_bit(inSF, 0);
			break;

		case 0x03:
			condition = get_byte_bit(inOF, 0);
			break;

		case 0x04:
			condition = get_byte_bit(SFneqOF, 0);
			break;

		case 0x05:
			condition = get_byte_bit(ZForSFneqOF, 0);
			break;

		case 0x06:
			condition = get_byte_bit(ZForCF, 0);
			break;

		case 0x07:
			condition = get_byte_bit(controller_bus->dma_req, 0);
			break;
		}
	}
	else {

		//IC_CMP2
		switch (controller_bus->cond_sel & 0b00000111) {
		case 0x00:
			condition = get_byte_bit(reg_status_value, MSWl_CPU_MODE);
			break;

		case 0x01:
			condition = get_byte_bit(controller_bus->wait, 0);
			break;

		case 0x02:
			condition = int_pending(controller_bus, reg_status_value);
			break;

		case 0x03:
			condition = get_byte_bit(controller_bus->ext_input, 0);
			break;

		case 0x04:
			condition = get_byte_bit(reg_status_value, MSWl_DIR);
			break;

		case 0x05:
			condition = get_byte_bit(reg_status_value, MSWl_DISPLAY_REG_LOAD);
			break;

		case 0x06:
			condition = 0;
			break;

		case 0x07:
			condition = 0;
			break;

		}
	}
	return get_byte_bit(controller_bus->cond_inv, 0) ^ condition;
}



void BAFFA1_ALU::baffa1_alu_display_registers(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS &alu_bus, HW_TTY& hw_tty) {

	char str_out[255];

	hw_tty.print("* A:"); print_byte_bin(str_out, this->A); hw_tty.print(str_out); hw_tty.print("  | ");
	hw_tty.print("B:"); print_byte_bin(str_out, this->B); hw_tty.print(str_out); hw_tty.print("  | ");
	hw_tty.print("C:"); print_byte_bin(str_out, this->C); hw_tty.print(str_out); hw_tty.print("  | ");
	hw_tty.print("Z_bus:"); print_byte_bin(str_out, alu_bus.z_bus); hw_tty.print(str_out);
	
	hw_tty.print("\n");
	hw_tty.print("* Cin:"); print_nibble_bin(str_out, this->CIN); hw_tty.print(str_out); hw_tty.print(" | ");
	hw_tty.print("Cout:"); print_nibble_bin(str_out, this->COUT); hw_tty.print(str_out);
	hw_tty.print("\n");

	BAFFA1_BYTE ALU_S = controller_bus->alu_op;
	BAFFA1_BYTE ALU_M = controller_bus->alu_mode;

	hw_tty.print("* Op: "); print_nibble_bin(str_out, controller_bus->alu_op); hw_tty.print(str_out); hw_tty.print(" | Mode: "); print_nibble_bin(str_out, controller_bus->alu_mode); hw_tty.print(str_out);
	hw_tty.print("\n");
	//hw_tty.print("* EQ="); print_nibble_bin(str_out, this->EQ); hw_tty.print(str_out);  hw_tty.print(" | ");	
	//hw_tty.print("F="); print_byte_bin(str_out, this->F); hw_tty.print(str_out); hw_tty.print("\n");

	hw_tty.print("* Flags: [");

	if (alu_bus.alu_zf != 0x00) hw_tty.print("Z"); else hw_tty.print(" ");
	if (alu_bus.alu_cf != 0x00) hw_tty.print("C"); else hw_tty.print(" ");
	if (alu_bus.alu_of != 0x00) hw_tty.print("O"); else hw_tty.print(" ");
	hw_tty.print("] ");
	hw_tty.print("\n");
	hw_tty.print("\n");
}


void BAFFA1_ALU::display_debug_alu(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_BYTE u_cf, BAFFA1_BYTE msw_cf, HW_TTY& hw_tty) {

	char str_out[255];
	hw_tty.print("***** ALU\n");
	this->baffa1_alu_display_registers(controller_bus, alu_bus, hw_tty);
	hw_tty.print("*****\n");
	
	hw_tty.print("Final Condition : "); print_nibble_bin(str_out, alu_bus.final_condition); hw_tty.print(str_out);
	/*
	sprintf(str_out, " | alu_a_src=%02x", controller_bus->alu_a_src); hw_tty.print(str_out);
	sprintf(str_out, " | alu_b_src=%02x", controller_bus->alu_b_src); hw_tty.print(str_out);
	sprintf(str_out, " | alu_cf_in_src=%02x", controller_bus->alu_cf_in_src); hw_tty.print(str_out);
	sprintf(str_out, " | alu_cf_in_inv=%02x", controller_bus->alu_cf_in_inv != 0x00); hw_tty.print(str_out);
	sprintf(str_out, " | alu_cf_out_inv=%02x", controller_bus->alu_cf_out_inv); hw_tty.print(str_out);
	sprintf(str_out, " | alu_final_cf=%02x", alu_bus.alu_final_cf); hw_tty.print(str_out);
	hw_tty.print("\n");
	hw_tty.print("*****\n");
	hw_tty.print(" | alu_output="); print_byte_bin(str_out, alu_bus.alu_output); hw_tty.print(str_out); hw_tty.print("\n");

	if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x00)
		hw_tty.print("* alu_cin = 0\n");

	else if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x01) {
		hw_tty.print("* alu_cin = msw_cf:"); print_byte_bin(str_out, msw_cf); hw_tty.print(str_out); hw_tty.print("\n");
	}
	else if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x02) {
		hw_tty.print("* alu_cin = u_cf:"); print_byte_bin(str_out, u_cf); hw_tty.print(str_out); hw_tty.print("\n");
	}
	else if ((controller_bus->alu_cf_in_src & 0b00000011) == 0x03)
		hw_tty.print("* alu_cin = 1\n");

	hw_tty.print("* z_bus="); print_byte_bin(str_out, alu_bus.z_bus); hw_tty.print(str_out);
	hw_tty.print("\n");
	hw_tty.print("\n");
	*/
}

void BAFFA1_ALU::display_u_flags(struct baffa1_controller_rom *controller_bus, HW_TTY& hw_tty) {

	display_u_flags_lite(controller_bus, hw_tty);
	hw_tty.print("\n");
}


void BAFFA1_ALU::display_u_flags_lite(struct baffa1_controller_rom *controller_bus, HW_TTY& hw_tty) {
	char str_out[255];
	hw_tty.print("* U_FLAGS: ");
	hw_tty.print(" [");

	if (this->u_zf != 0x00) hw_tty.print("Z"); else hw_tty.print(" ");
	if (this->u_cf != 0x00) hw_tty.print("C"); else hw_tty.print(" ");
	if (this->u_sf != 0x00) hw_tty.print("S"); else hw_tty.print(" ");
	if (this->u_of != 0x00) hw_tty.print("O"); else hw_tty.print(" ");
	hw_tty.print("]");
	hw_tty.print("\n");

	sprintf(str_out, "* u_zf_in_src:%02x", controller_bus->uzf_in_src); hw_tty.print(str_out);
	sprintf(str_out, " | u_cf_in_src:%02x", controller_bus->ucf_in_src); hw_tty.print(str_out);
	sprintf(str_out, " | u_sf_in_src:%02x", controller_bus->usf_in_src); hw_tty.print(str_out);
	sprintf(str_out, " | u_of_in_src:%02x", controller_bus->uof_in_src); hw_tty.print(str_out);
	hw_tty.print("\n");
}
