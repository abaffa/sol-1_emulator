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
#include "baffa1_alu.h"
#include "baffa1_utils.h"

void BAFFA1_ALU::reset() {

	alu_bus.reset();

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


	this->u_zf = 0x00;
	this->u_cf = 0x00;
	this->u_sf = 0x00;
	this->u_of = 0x00;
	//this->u_esc = 0x00;

}

void BAFFA1_ALU::init() {
	this->alu_bus.init();
	this->reset();
	this->U_FLAGS.reset();
}



BAFFA1_BYTE ALU(
	BAFFA1_BYTE XBus, BAFFA1_BYTE YBus,
	BAFFA1_BYTE alu_op, BAFFA1_BYTE alu_mode,
	BAFFA1_BYTE alu_cf_in_src, BAFFA1_BYTE msw_cf, BAFFA1_BYTE u_cf, BAFFA1_BYTE alu_cf_in_inv) {
	////////////////////////////////////////////////////////////////
	//
	// Set C_in
	//
	BAFFA1_BYTE alu_cin = 0x00;

	if ((alu_cf_in_src & 0b00000011) == 0x00)
		alu_cin = 1;
	else {
		if ((alu_cf_in_src & 0b00000011) == 0x01)
			alu_cin = msw_cf;

		else if ((alu_cf_in_src & 0b00000011) == 0x02)
			alu_cin = u_cf;

		else if ((alu_cf_in_src & 0b00000011) == 0x03)
			alu_cin = 0;
	}

	alu_cin = (alu_cin ^ alu_cf_in_inv) & 0b00000001;

	////////////////////////////////////////////////////////////////
	//
	// Calculate ALU
	//
	//this->A = XBus;
	//this->B = YBus;
	//this->CIN = (alu_cin) & 0b00000001;

	BAFFA1_BYTE ALU_S = alu_op;
	BAFFA1_BYTE ALU_M = alu_mode;

	baffa1_alu_4bit ALUl;
	BAFFA1_BYTE ALUl_A = XBus & 0b00001111;
	BAFFA1_BYTE ALUl_B = YBus & 0b00001111;
	BAFFA1_BYTE ALUl_CIN = (alu_cin) & 0b00000001;
	baffa1_alu_4bit_op(&ALUl, ALUl_A, ALUl_B, ALUl_CIN, ALU_S, ALU_M);

	baffa1_alu_4bit ALUh;
	BAFFA1_BYTE ALUh_A = (XBus & 0b11110000) >> 4;
	BAFFA1_BYTE ALUh_B = (YBus & 0b11110000) >> 4;
	BAFFA1_BYTE ALUh_CIN = (ALUl.COUT) & 0b00000001;
	baffa1_alu_4bit_op(&ALUh, ALUh_A, ALUh_B, ALUh_CIN, ALU_S, ALU_M);


	BAFFA1_BYTE ALUh_C = (ALUl.F & 0b00001111) | ((ALUh.F & 0b00001111) << 4);
	//this->C = ALUh_C;
	//this->COUT = ALUh.COUT;

	//alu_bus.alu_output = this->C;

	return ALUh_C;
}

BAFFA1_BYTE BAFFA1_ALU::exec(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE w_bus, BAFFA1_BYTE k_bus, BAFFA1_CONFIG &cpu_config) {

	alu_bus.x_bus = x_bus_refresh(w_bus, controller_bus.alu_a_src);
	alu_bus.y_bus = y_bus_refresh(k_bus, controller_bus.imm, controller_bus.alu_b_src);

	////////////////////////////////////////////////////////////////
	//
	// Set C_in
	//
	BAFFA1_BYTE alu_cin = 0x00;

	if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x00)
		alu_cin = 1;
	else {
		if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x01)
			alu_cin = get_byte_bit(MSWh.value(), MSWh_CF);

		else if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x02)
			alu_cin = u_cf;

		else if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x03)
			alu_cin = 0;
	}

	alu_cin = (alu_cin ^ controller_bus.alu_cf_in_inv) & 0b00000001;

	////////////////////////////////////////////////////////////////
	//
	// Calculate ALU
	//
	this->A = alu_bus.x_bus;
	this->B = alu_bus.y_bus;
	this->CIN = (alu_cin) & 0b00000001;

	BAFFA1_BYTE ALU_S = controller_bus.alu_op;
	BAFFA1_BYTE ALU_M = controller_bus.alu_mode;

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

	// Corrigir aqui 
	/*
	alu_bus.alu_output = ALU(
		alu_bus.x_bus, alu_bus.y_bus,
		controller_bus.alu_op, controller_bus.alu_mode,
		controller_bus.alu_cf_in_src, msw_cf, u_cf, controller_bus.alu_cf_in_inv);
		*/
		/////////////////////////////////////////////////////////////////
		//
		// ALU_CF (Carry Flag)
		//

	alu_bus.alu_cf = this->COUT;
	alu_bus.alu_final_cf = (alu_bus.alu_cf ^ (controller_bus.alu_cf_out_inv)) & 0b00000001;


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

	if (controller_bus.shift_src == 0x00) inIC16 = 0x00;
	else if (controller_bus.shift_src == 0x01) inIC16 = u_cf;
	else if (controller_bus.shift_src == 0x02) inIC16 = get_byte_bit(MSWh.value(), MSWh_CF);
	else if (controller_bus.shift_src == 0x03) inIC16 = get_byte_bit(alu_bus.alu_output, 0);
	else if (controller_bus.shift_src == 0x04) inIC16 = get_byte_bit(alu_bus.alu_output, 7);
	else if (controller_bus.shift_src == 0x05) inIC16 = 0x01;
	else if (controller_bus.shift_src == 0x06) inIC16 = 0x01;
	else if (controller_bus.shift_src == 0x07) inIC16 = 0x01;


	if ((controller_bus.zbus_out_src & 0b00000011) == 0x00)
		alu_bus.z_bus = alu_bus.alu_output;

	else if ((controller_bus.zbus_out_src & 0b00000011) == 0x01)
		alu_bus.z_bus = (alu_bus.alu_output >> 1) | (inIC16 << 7);

	else if ((controller_bus.zbus_out_src & 0b00000011) == 0x02)
		alu_bus.z_bus = (alu_bus.alu_output << 1) | inIC16;

	else if ((controller_bus.zbus_out_src & 0b00000011) == 0x03)
		alu_bus.z_bus = get_byte_bit(alu_bus.alu_output, 7) != 0x00 ? 0b11111111 : 0b00000000;


	/////////////////////////////////////////////////////////////////
	//
	// ALU_OF (Overflow Flag)
	//
	BAFFA1_BYTE zNEQx = (get_byte_bit(alu_bus.z_bus, 7) != get_byte_bit(alu_bus.x_bus, 7));
	BAFFA1_BYTE xNEQy = (get_byte_bit(alu_bus.x_bus, 7) != get_byte_bit(alu_bus.y_bus, 7));

	alu_bus.alu_of = zNEQx && ((controller_bus.alu_op != 0b1001) == xNEQy);
	//
	/////////////////////////////////////////////////////////////////
	//
	// ALU_ZF (Zero Flag)
	//
	alu_bus.alu_zf = (alu_bus.z_bus == 0x00);
	/////////////////////////////////////////////////////////////////



	//Flags
	// nao posso usar final condition sem ter atualizado o MSW
	refresh_reg_flags_MSWh(controller_bus, alu_bus, u_sf);

	//Status
	if (controller_bus.status_wrt == 0x00) {
		this->MSWl.set(alu_bus.z_bus);
	}

	// G
	if (controller_bus.gh_wrt == 0x00) { this->Gh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Gh", alu_bus.z_bus); } }
	if (controller_bus.gl_wrt == 0x00) { this->Gl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Gl", alu_bus.z_bus); } }


	// U_FLAGS
	// nao posso usar final condition sem ter atualizado o MSW
	u_flags_refresh(controller_bus, MSWl.value(), MSWh.value(), alu_bus);


	//final condition
	// nao posso usar final condition sem ter atualizado o MSW
	alu_bus.final_condition = this->calc_final_condition(controller_bus, interruptions_bus, MSWl.value(), MSWh.value());

	return alu_bus.z_bus;
}





void BAFFA1_ALU::u_flags_refresh(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value, BAFFA1_ALU_BUS &alu_bus)
{

	BAFFA1_BYTE inZF = 0x00;
	BAFFA1_BYTE inCF = 0x00;
	BAFFA1_BYTE inSF = 0x00;
	BAFFA1_BYTE inOF = 0x00;

	switch (controller_bus.uzf_in_src) {
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

	switch (controller_bus.ucf_in_src) {
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

	if (controller_bus.usf_in_src == 0x00)// unchanged
		inSF = get_byte_bit(this->u_sf, 0);
	else
		inSF = get_byte_bit(alu_bus.z_bus, 7);

	if (controller_bus.uof_in_src == 0x00)
		inOF = get_byte_bit(this->u_of, 0);// unchanged
	else
		inOF = get_byte_bit(alu_bus.alu_of, 0);


	this->u_zf = inZF;
	this->u_cf = inCF;
	this->u_sf = inSF;
	this->u_of = inOF;


	////// PQ ESTA AQUI??
	//if (controller_bus.u_esc_in_src != 0x00) // byte do escape 
		//this->u_esc = controller_bus.imm & 0b00000011;


	//////
}




BAFFA1_BYTE BAFFA1_ALU::calc_final_condition(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE reg_status_value, BAFFA1_BYTE reg_flags_value) {

	BAFFA1_BYTE condition = 0x0;

	if (!get_byte_bit(controller_bus.cond_sel, 3)) {
		BAFFA1_BYTE inZF = 0x00;
		BAFFA1_BYTE inCF = 0x00;
		BAFFA1_BYTE inSF = 0x00;
		BAFFA1_BYTE inOF = 0x00;

		//IC_U_CODE_FLAGS
		if (controller_bus.cond_flags_src == 0x00) {
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
		switch (controller_bus.cond_sel & 0b00000111) {
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
			condition = get_byte_bit(controller_bus.dma_req, 0);
			break;
		}
	}
	else {

		//IC_CMP2
		switch (controller_bus.cond_sel & 0b00000111) {
		case 0x00:
			condition = get_byte_bit(reg_status_value, MSWl_CPU_MODE);
			break;

		case 0x01:
			condition = get_byte_bit(controller_bus.wait, 0);
			break;

		case 0x02:
			condition = int_pending(controller_bus, interruptions_bus, reg_status_value);
			break;

		case 0x03:
			condition = get_byte_bit(controller_bus.ext_input, 0);
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
	return get_byte_bit(controller_bus.cond_inv, 0) ^ condition;
}



void BAFFA1_ALU::display_registers(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty) {

	char str_out[255];

	if (cpu_config.DEBUG_ALU_REGISTERS) {

		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ ALU REGISTERS\n\n");

		_hw_tty.print("  A: "); print_byte_bin(str_out, this->A); _hw_tty.print(str_out); _hw_tty.print("  | ");
		_hw_tty.print("B: "); print_byte_bin(str_out, this->B); _hw_tty.print(str_out); _hw_tty.print("  | ");
		_hw_tty.print("C: "); print_byte_bin(str_out, this->C); _hw_tty.print(str_out); 
		_hw_tty.print("\n");

		_hw_tty.print("  Z_bus: "); print_byte_bin(str_out, alu_bus.z_bus); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		_hw_tty.print("  Cin: "); print_nibble_bin(str_out, this->CIN); _hw_tty.print(str_out); _hw_tty.print(" | ");
		_hw_tty.print("Cout: "); print_nibble_bin(str_out, this->COUT); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		BAFFA1_BYTE ALU_S = controller_bus.alu_op;
		BAFFA1_BYTE ALU_M = controller_bus.alu_mode;

		_hw_tty.print("  Op: "); print_nibble_bin(str_out, controller_bus.alu_op); _hw_tty.print(str_out); _hw_tty.print(" | Mode: "); print_nibble_bin(str_out, controller_bus.alu_mode); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		//_hw_tty.print("  EQ="); print_nibble_bin(str_out, this->EQ); _hw_tty.print(str_out);  _hw_tty.print(" | ");	
		//_hw_tty.print("F="); print_byte_bin(str_out, this->F); _hw_tty.print(str_out); 
		_hw_tty.print("\n");
		_hw_tty.print("  BUS Flags: [");

		if (alu_bus.alu_zf != 0x00) _hw_tty.print("Z"); else _hw_tty.print(" ");
		if (alu_bus.alu_cf != 0x00) _hw_tty.print("C"); else _hw_tty.print(" ");
		if (alu_bus.alu_of != 0x00) _hw_tty.print("O"); else _hw_tty.print(" ");
		_hw_tty.print("] ");
		_hw_tty.print("\n");
		_hw_tty.print("  ZF Zero Flag | CF Carry Flag\n");
		_hw_tty.print("               | OF Overflow Flag\n");
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}

	if (cpu_config.DEBUG_ALU_STATUS_FLAGS) {
		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ MSWl (status) Flags: ");
		this->mswl_status_desc(_hw_tty);
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}

	if (cpu_config.DEBUG_ALU_MSWh_FLAGS) {
		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ MSWh Flags: ");
		this->mswh_flags_desc(_hw_tty);
		_hw_tty.print("\n");
		_hw_tty.print("  ZF Zero Flag | CF Carry Flag\n");
		_hw_tty.print("  SF Sign Flag | OF Overflow Flag\n");
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}



}



void  BAFFA1_ALU::mswh_flags_desc(HW_TTY &_hw_tty) {
	
	BAFFA1_BYTE b = this->MSWh.value();
	_hw_tty.print("[");
	if (get_byte_bit(b, MSWh_ZF) != 0x00)
		_hw_tty.print("Z"); else _hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_CF) != 0x00)
		_hw_tty.print("C"); else _hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_SF) != 0x00)
		_hw_tty.print("S"); else _hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_OF) != 0x00)
		_hw_tty.print("O"); else _hw_tty.print(" ");
	_hw_tty.print("]");
}




void BAFFA1_ALU::mswl_status_desc(HW_TTY &_hw_tty) {
	
	BAFFA1_BYTE b = this->MSWl.value();
	_hw_tty.print("[");
	if (get_byte_bit(b, MSWl_DMA_ACK) != 0x00)
		_hw_tty.print(" | dma_ack ");
	if (get_byte_bit(b, MSWl_INTERRUPT_ENABLE) != 0x00)
		_hw_tty.print(" | interrupt_enable ");
	if (get_byte_bit(b, MSWl_CPU_MODE) != 0x00)
		_hw_tty.print(" | cpu_mode ");
	if (get_byte_bit(b, MSWl_PAGING_EN) != 0x00)
		_hw_tty.print(" | paging_en ");
	if (get_byte_bit(b, MSWl_HALT) != 0x00)
		_hw_tty.print(" | halt ");
	if (get_byte_bit(b, MSWl_DISPLAY_REG_LOAD) != 0x00)
		_hw_tty.print(" | display_reg_load ");
	if (get_byte_bit(b, MSWl_DIR) != 0x00)
		_hw_tty.print(" | dir ");
	_hw_tty.print("]");

}


void BAFFA1_ALU::debugger(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty) {

	char str_out[255];

	if (cpu_config.DEBUG_ALU) {
		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ ALU\n\n");

		_hw_tty.print("  Final Condition: "); print_nibble_bin(str_out, alu_bus.final_condition); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		sprintf(str_out, "  alu_a_src=%02x", controller_bus.alu_a_src); _hw_tty.print(str_out);
		sprintf(str_out, " | alu_b_src=%02x", controller_bus.alu_b_src); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		sprintf(str_out, "  alu_cf_in_src=%02x", controller_bus.alu_cf_in_src); _hw_tty.print(str_out);
		sprintf(str_out, " | alu_cf_in_inv=%02x", controller_bus.alu_cf_in_inv != 0x00); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		sprintf(str_out, "  alu_cf_out_inv=%02x", controller_bus.alu_cf_out_inv); _hw_tty.print(str_out);
		sprintf(str_out, " | alu_final_cf=%02x", alu_bus.alu_final_cf); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");

		_hw_tty.print("  alu_output="); print_byte_bin(str_out, alu_bus.alu_output); _hw_tty.print(str_out); _hw_tty.print("\n");

		if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x00)
			_hw_tty.print("  alu_cin = 0\n");

		else if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x01) {
			_hw_tty.print("  alu_cin = msw_cf: "); print_byte_bin(str_out, get_byte_bit(MSWh.value(), MSWh_CF)); _hw_tty.print(str_out); _hw_tty.print("\n");
		}
		else if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x02) {
			_hw_tty.print("  alu_cin = u_cf: "); print_byte_bin(str_out, u_cf); _hw_tty.print(str_out); _hw_tty.print("\n");
		}
		else if ((controller_bus.alu_cf_in_src & 0b00000011) == 0x03)
			_hw_tty.print("  alu_cin = 1\n");

		_hw_tty.print("  z_bus="); print_byte_bin(str_out, alu_bus.z_bus); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}

	this->display_registers(controller_bus, alu_bus, cpu_config, _hw_tty);


	if (cpu_config.DEBUG_UFLAGS) {
		str_out[0] = '\0';

		_hw_tty.print("+------------------------------------------------------------------------------\n");
		_hw_tty.print("+ U_FLAGS [");

		if (this->u_zf != 0x00) _hw_tty.print("Z"); else _hw_tty.print(" ");
		if (this->u_cf != 0x00) _hw_tty.print("C"); else _hw_tty.print(" ");
		if (this->u_sf != 0x00) _hw_tty.print("S"); else _hw_tty.print(" ");
		if (this->u_of != 0x00) _hw_tty.print("O"); else _hw_tty.print(" ");
		_hw_tty.print("]");
		_hw_tty.print("\n");
		_hw_tty.print("  ZF Zero Flag | CF Carry Flag\n");
		_hw_tty.print("  SF Sign Flag | OF Overflow Flag\n");
		_hw_tty.print("\n");

		sprintf(str_out, "  u_zf_in_src: %02x", controller_bus.uzf_in_src); _hw_tty.print(str_out);
		sprintf(str_out, " | u_cf_in_src: %02x", controller_bus.ucf_in_src); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		sprintf(str_out, "  u_sf_in_src: %02x", controller_bus.usf_in_src); _hw_tty.print(str_out);
		sprintf(str_out, " | u_of_in_src: %02x", controller_bus.uof_in_src); _hw_tty.print(str_out);
		_hw_tty.print("\n");
		_hw_tty.print("\n");
	}
}









//////////////////////





BAFFA1_BYTE BAFFA1_ALU::get_MSWh_ZF(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_ZF = 0x00;

	switch (controller_bus.zf_in_src & 0b00000011) {
	case 0x00:
		inMSWh_ZF = get_byte_bit(this->MSWh.value(), MSWh_ZF);
		break;

	case 0x01:
		inMSWh_ZF = get_byte_bit(alu_bus.alu_zf, 0);
		break;
	case 0x02:
		inMSWh_ZF = get_byte_bit(alu_bus.alu_zf & get_byte_bit(this->MSWh.value(), MSWh_ZF), 0);
		break;

	case 0x03:
		inMSWh_ZF = get_byte_bit(alu_bus.z_bus, 0);
		break;
	}

	return inMSWh_ZF;
}


BAFFA1_BYTE BAFFA1_ALU::get_MSWh_CF(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_CF = 0x00;

	switch (controller_bus.cf_in_src & 0b00000111) {
	case 0x00:
		inMSWh_CF = get_byte_bit(this->MSWh.value(), MSWh_CF);
		break;

	case 0x01:
		inMSWh_CF = get_byte_bit(alu_bus.alu_final_cf, 0);
		break;

	case 0x02:
		inMSWh_CF = get_byte_bit(alu_bus.alu_output, 0);
		break;

	case 0x03:
		inMSWh_CF = get_byte_bit(alu_bus.z_bus, 1);
		break;

	case 0x04:
		inMSWh_CF = get_byte_bit(alu_bus.alu_output, 7);
		break;
	}

	return inMSWh_CF;
}

BAFFA1_BYTE BAFFA1_ALU::get_MSWh_SF(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_SF = 0x00;

	switch (controller_bus.sf_in_src & 0b00000011) {
	case 0x00:

		inMSWh_SF = get_byte_bit(this->MSWh.value(), MSWh_SF);
		break;

	case 0x01:
		inMSWh_SF = get_byte_bit(alu_bus.z_bus, 7);
		break;

	case 0x02:
		inMSWh_SF = 0x00;
		break;

	case 0x03:
		inMSWh_SF = get_byte_bit(alu_bus.z_bus, 2);
		break;
	}

	return inMSWh_SF;
}

BAFFA1_BYTE BAFFA1_ALU::get_MSWh_OF(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf) {

	BAFFA1_BYTE inMSWh_OF = 0x00;

	switch (controller_bus.of_in_src & 0b00000111) {
	case 0x00:
		inMSWh_OF = get_byte_bit(this->MSWh.value(), MSWh_OF);
		break;

	case 0x01:
		inMSWh_OF = get_byte_bit(alu_bus.alu_of, 0);
		break;

	case 0x02:
		inMSWh_OF = get_byte_bit(alu_bus.z_bus, 7);
		break;

	case 0x03:
		inMSWh_OF = get_byte_bit(alu_bus.z_bus, 3);
		break;

	case 0x04:
		inMSWh_OF = get_byte_bit(u_sf, 0) != get_byte_bit(alu_bus.z_bus, 7);
		break;
	}

	return inMSWh_OF;
}


void BAFFA1_ALU::refresh_reg_flags_MSWh(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf) {

	BAFFA1_BYTE inMSWh_ZF = get_MSWh_ZF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_CF = get_MSWh_CF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_SF = get_MSWh_SF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_OF = get_MSWh_OF(controller_bus, alu_bus, u_sf);

	BAFFA1_BYTE inMSW_H = set_byte_bit(inMSWh_ZF, 0) | set_byte_bit(inMSWh_CF, 1) | set_byte_bit(inMSWh_SF, 2) | set_byte_bit(inMSWh_OF, 3);
	this->MSWh.set(inMSW_H);
	//}
}



void BAFFA1_ALU::refresh(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE data_bus, BAFFA1_BYTE u_sf, BAFFA1_CONFIG &cpu_config) {
	//#######################
//IC86B //IC58B //IC86C //IC241 //IC14 //IC255 //IC23

//
	BAFFA1_BYTE inMSWh_ZF = get_MSWh_ZF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_CF = get_MSWh_CF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_SF = get_MSWh_SF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_OF = get_MSWh_OF(controller_bus, alu_bus, u_sf);

	//

	//if (0x01) { // ~RST
		//IC206
	BAFFA1_BYTE inMSW_H = set_byte_bit(inMSWh_ZF, 0) | set_byte_bit(inMSWh_CF, 1) | set_byte_bit(inMSWh_SF, 2) | set_byte_bit(inMSWh_OF, 3);
	this->MSWh.set(inMSW_H);
	//}

	if (controller_bus.status_wrt == 0x00) {
		this->MSWl.set(alu_bus.z_bus);
	}

	if (controller_bus.gh_wrt == 0x00) { this->Gh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Gh", alu_bus.z_bus); } }
	if (controller_bus.gl_wrt == 0x00) { this->Gl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Gl", alu_bus.z_bus); } }
}





BAFFA1_BYTE BAFFA1_ALU::y_bus_refresh(BAFFA1_BYTE k_bus, BAFFA1_BYTE imm,
	BAFFA1_BYTE alu_b_src) {

	BAFFA1_BYTE y_bus = 0x00;
	// K = Y
	if (alu_b_src == 0x00)
		//return imm or k_bus
		y_bus = imm;
	else {
		// return MDR or TDR
		y_bus = k_bus;
	}

	return y_bus;
}





BAFFA1_BYTE BAFFA1_ALU::x_bus_refresh(BAFFA1_BYTE w_bus, BAFFA1_BYTE alu_a_src)
{

	BAFFA1_BYTE x_bus = 0x00;

	if (!get_byte_bit(alu_a_src, 5))
		x_bus = w_bus;

	else {

		switch ((alu_a_src & 0b00000011)) {
		case 0x00:

			x_bus = set_byte_bit(get_byte_bit(MSWh.value(), MSWh_ZF), 0) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_CF), 1) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_SF), 2) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_OF), 3) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_12), 4) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_13), 5) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_14), 6) |
				set_byte_bit(get_byte_bit(MSWh.value(), MSWh_15), 7);
			break;

		case 0x01:

			x_bus = get_byte_bit(MSWl.value(), MSWl_DMA_ACK) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_INTERRUPT_ENABLE), 1) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_CPU_MODE), 2) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_PAGING_EN), 3) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_HALT), 4) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_DISPLAY_REG_LOAD), 5) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_14), 6) |
				set_byte_bit(get_byte_bit(MSWl.value(), MSWl_DIR), 7);
			break;

		case 0x02:
			x_bus = Gl.value();
			break;

		case 0x03:
			x_bus = Gh.value();
			break;
		}
	}

	return x_bus;
}
//////////////////////



// usado apenas pelo update_final_condition
BAFFA1_BYTE BAFFA1_ALU::int_pending(BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_INTERRUPTIONS_BUS &interruptions_bus, BAFFA1_BYTE reg_status_value) {
	return get_byte_bit(interruptions_bus.int_request, 0) & get_byte_bit(reg_status_value, MSWl_INTERRUPT_ENABLE);
}





