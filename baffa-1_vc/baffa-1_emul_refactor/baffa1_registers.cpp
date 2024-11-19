//
// baffa1_registers.cpp
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
#include "baffa1_registers.h"

#include <stdio.h>
#include "baffa1_utils.h"


void BAFFA1_REGISTERS::init() {
	this->reset();
}

void BAFFA1_REGISTERS::reset() {
	this->Ah.reset(); // AX (16bit) Accumulator	(Ah/Al)
	this->Al.reset();
	this->Bh.reset(); // BX (16bit) Base		(Bh/Bl)
	this->Bl.reset();
	this->Ch.reset(); // CX (16bit) Counter		(Ch/Cl)
	this->Cl.reset();
	this->Dh.reset(); // DX (16bit) Data		(Dh/Dl)
	this->Dl.reset();


	//Pointer Registers
	this->BPh.reset(); // BP (16bit) Base Pointer  (Used to manage stack frames)
	this->BPl.reset();
	this->SPh.reset(); // SP (16bit) Stack Pointer
	this->SPl.reset();
	this->SSPh.reset(); // SSP (16bit) Supervisor Stack Pointer
	this->SSPl.reset();

	//Index Registers
	this->SIh.reset(); // SI (16bit) Source index (Source address for string operations)
	this->SIl.reset();
	this->DIh.reset(); // DI (16bit) Destination Index (Destination address for string operations)
	this->DIl.reset();
	this->PCh.reset(); // PC (16bit) Program Counter
	this->PCl.reset();

	this->TDRh.reset(); // TDR (16bit) Temporary Data Register
	this->TDRl.reset();


	this->INT_MASKS.reset(); // INT FLAGS


//	this->PTB.reset();  // PTB (8bit) = Page table base

	this->MARh.reset(); // memory address register
	this->MARl.reset();
	this->MDRh.reset(); // memory data register
	this->MDRl.reset();
}


void BAFFA1_REGISTERS::process(BAFFA1_DATA_BUS &data_bus, BAFFA1_CONTROLLER_BUS &controller_bus, BAFFA1_ALU_BUS &alu_bus, BAFFA1_BYTE MSWl_value, BAFFA1_BYTE u_sf, BAFFA1_CONFIG &cpu_config) {
	//#######################
//IC86B //IC58B //IC86C //IC241 //IC14 //IC255 //IC23
	/*
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
	*/


	///////////////////////////////////////////////////////////////////////////
	// READ DATA
	//DATA REGISTERS
	if (controller_bus.ah_wrt == 0x00) { this->Ah.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Ah", alu_bus.z_bus); } }
	if (controller_bus.al_wrt == 0x00) { this->Al.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Al", alu_bus.z_bus); } }

	if (controller_bus.bh_wrt == 0x00) { this->Bh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Bh", alu_bus.z_bus); } }
	if (controller_bus.bl_wrt == 0x00) { this->Bl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Bl", alu_bus.z_bus); } }

	if (controller_bus.ch_wrt == 0x00) { this->Ch.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Ch", alu_bus.z_bus); } }
	if (controller_bus.cl_wrt == 0x00) { this->Cl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Cl", alu_bus.z_bus); } }

	if (controller_bus.dh_wrt == 0x00) { this->Dh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Dh", alu_bus.z_bus); } }
	if (controller_bus.dl_wrt == 0x00) { this->Dl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"Dl", alu_bus.z_bus); } }

	//Index Registers
	if (controller_bus.dih_wrt == 0x00) { this->DIh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"DIh", alu_bus.z_bus); } }
	if (controller_bus.dil_wrt == 0x00) { this->DIl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"DIl", alu_bus.z_bus); } }

	if (controller_bus.sih_wrt == 0x00) { this->SIh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SIh", alu_bus.z_bus); } }
	if (controller_bus.sil_wrt == 0x00) { this->SIl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SIl", alu_bus.z_bus); } }

	//Pointer Registers
	if (controller_bus.bph_wrt == 0x00) { this->BPh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"BPh", alu_bus.z_bus); } }
	if (controller_bus.bpl_wrt == 0x00) { this->BPl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"BPl", alu_bus.z_bus); } }

	if (controller_bus.sph_wrt == 0x00) { this->SPh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SPh", alu_bus.z_bus); } }
	if (controller_bus.spl_wrt == 0x00) { this->SPl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SPl", alu_bus.z_bus); } }

	// get_byte_bit(MSWl_value, MSWl_CPU_MODE)) = 0 >> update both sp and ssp 
	// get_byte_bit(MSWl_value, MSWl_CPU_MODE)) = 1 >> update only sp
	if (!get_byte_bit(MSWl_value, MSWl_CPU_MODE)) {
		if (controller_bus.sph_wrt == 0x00) { this->SSPh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SSPh", alu_bus.z_bus); } }
		if (controller_bus.spl_wrt == 0x00) { this->SSPl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"SSPl", alu_bus.z_bus); } }
	}

	if (controller_bus.tdrh_wrt == 0x00) { this->TDRh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"TDRh", alu_bus.z_bus); } }
	if (controller_bus.tdrl_wrt == 0x00) { this->TDRl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"TDRl", alu_bus.z_bus); } }


	if (controller_bus.pch_wrt == 0x00) { this->PCh.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"PCh", alu_bus.z_bus); } }
	if (controller_bus.pcl_wrt == 0x00) { this->PCl.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"PCl", alu_bus.z_bus); } }

	/////////////////////////////////////////////////////////////////////////////
	if (controller_bus.mask_flags_wrt == 0x00) this->INT_MASKS.set(alu_bus.z_bus);
	/////////////////////////////////////////////////////////////////////////////
	// SET MDR
	//IC7 //IC24 //IC19 //IC183
	if (controller_bus.mdrl_wrt == 0x00) { this->MDRl.set(controller_bus.mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus.data);  if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MDRl", (controller_bus.mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus.data)); } }
	if (controller_bus.mdrh_wrt == 0x00) { this->MDRh.set(controller_bus.mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus.data); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MDRh", (controller_bus.mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus.data)); } }
	////////////////////////////////////////////////////////////////////////////
	//MEMORY SET MAR

	if (controller_bus.marl_wrt == 0x00) {
		//IC131 //IC128			
		if (controller_bus.mar_in_src == 0x00)
		{
			this->MARl.set(alu_bus.z_bus);
			if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MARl", alu_bus.z_bus); }
		}
		else {
			this->MARl.set(this->PCl.value());
			if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MARl", this->PCl.value()); }
		}
	}

	if (controller_bus.marh_wrt == 0x00) {
		//IC129 //IC132			
		if (controller_bus.mar_in_src == 0x00) {
			this->MARh.set(alu_bus.z_bus);
			if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MARh", alu_bus.z_bus); }
		}
		else {
			this->MARh.set(this->PCh.value());
			if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file((char*)"WRITE", (char*)"MARh", this->PCh.value()); }
		}
	}

}



BAFFA1_BYTE BAFFA1_REGISTERS::w_bus_refresh(
	BAFFA1_BYTE bus_tristate,
	BAFFA1_BYTE panel_regsel,
	BAFFA1_BYTE alu_a_src,
	BAFFA1_BYTE display_reg_load,
	BAFFA1_BYTE int_vector,
	BAFFA1_BYTE int_masks,
	BAFFA1_BYTE int_status, BAFFA1_CONFIG &cpu_config) {

	//IC125 //IC118 //IC3   //IC2  //IC9  //IC42  //IC6   //IC20 //IC5  //IC80  //IC41  //IC44
	//IC30  //IC130 //IC56  //IC62 //IC53 //IC133 //IC68  //IC69 //IC67 //IC141 //IC81
	//IC82  //IC71  //IC144 //IC85 //IC86 //IC84  //IC152 //IC88 //IC89 //IC86  //IC160

	//char str_out[255];

	BAFFA1_BYTE w_bus = 0x00;

	BAFFA1_BYTE selRegCol = 0x00;
	BAFFA1_BYTE selRegRow = 0x00;


	//REGISTER SELECTION - que está na placa ALU e deveria estar aqui
	// Seleciona registro pelo barramento
	if ((bus_tristate == 0x00) & (display_reg_load == 0x00)) {
		selRegCol = alu_a_src & 0b00000111;
		selRegRow = get_byte_bit(alu_a_src, 3) | set_byte_bit(get_byte_bit(alu_a_src, 4), 1);
	}
	else { // Seleciona registro pelo painel
		selRegCol = panel_regsel & 0b00000111;
		selRegRow = get_byte_bit(panel_regsel, 3) | set_byte_bit(get_byte_bit(panel_regsel, 4), 1);
	}

	if (selRegRow == 0x00) {
		switch (selRegCol) {
		case 0x00:
			w_bus = Al.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Al", w_bus); }
			break;
		case 0x01:
			w_bus = Ah.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Ah", w_bus); }
			break;
		case 0x02:
			w_bus = Bl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Bl", w_bus); }
			break;
		case 0x03:
			w_bus = Bh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Bh", w_bus); }
			break;
		case 0x04:
			w_bus = Cl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Cl", w_bus); }
			break;
		case 0x05:
			w_bus = Ch.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Ch", w_bus); }
			break;
		case 0x06:
			w_bus = Dl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Dl", w_bus); }
			break;
		case 0x07:
			w_bus = Dh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"Dh", w_bus); }
			break;
		}

	}
	else if (selRegRow == 0x01) {
		switch (selRegCol) {
		case 0x00:
			w_bus = SPl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SPl", w_bus); }
			break;
		case 0x01:
			w_bus = SPh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SPh", w_bus); }
			break;
		case 0x02:
			w_bus = BPl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"BPl", w_bus); }
			break;
		case 0x03:
			w_bus = BPh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"BPh", w_bus); }
			break;
		case 0x04:
			w_bus = SIl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SIl", w_bus); }
			break;
		case 0x05:
			w_bus = SIh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SIh", w_bus); }
			break;
		case 0x06:
			w_bus = DIl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"DIl", w_bus); }
			break;
		case 0x07:
			w_bus = DIh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"DIh", w_bus); }
			break;
		}
	}
	else if (selRegRow == 0x02) {
		switch (selRegCol) {
		case 0x00:
			w_bus = PCl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"PCl", w_bus); }
			break;
		case 0x01:
			w_bus = PCh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"PCh", w_bus); }
			break;
		case 0x02:
			w_bus = MARl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"MARl", w_bus); }
			break;
		case 0x03:
			w_bus = MARh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"MARh", w_bus); }
			break;
		case 0x04:
			w_bus = MDRl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"MDRl", w_bus); }
			break;
		case 0x05:
			w_bus = MDRh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"MDRh", w_bus); }
			break;
		case 0x06:
			w_bus = TDRl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"TDRl", w_bus); }
			break;
		case 0x07:
			w_bus = TDRh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"TDRh", w_bus); }
			break;
		}
	}
	else if (selRegRow == 0x03) {
		switch (selRegCol) {
		case 0x00:
			w_bus = SSPl.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SSPl", w_bus); }
			break;
		case 0x01:
			w_bus = SSPh.value();
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"SSPh", w_bus); }
			break;
		case 0x02:
			w_bus = int_vector;
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"INT_VECTOR", w_bus); }
			break;
		case 0x03:
			w_bus = int_masks;
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"INT_MASKS", w_bus); }
			break;
		case 0x04:
			w_bus = int_status;
			if (cpu_config.DEBUG_REG_READ) { cpu_config.io_file((char *)"READ ", (char *)"INT_STATUS", w_bus); }
			break;
		}
	}
	return w_bus;
}


BAFFA1_BYTE BAFFA1_REGISTERS::k_bus_refresh(BAFFA1_BYTE alu_b_src) {

	//IC92 //I103 //IC116

	BAFFA1_BYTE k_bus = 0x00;
	switch (alu_b_src & 0b00000011) {
	case 0x00: k_bus = MDRl.value(); break;
	case 0x01: k_bus = MDRh.value(); break;
	case 0x02: k_bus = TDRl.value(); break;
	case 0x03: k_bus = TDRh.value(); break;
	}

	return k_bus;
}





void BAFFA1_REGISTERS::debugger(BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty) {

	if (cpu_config.DEBUG_REGISTERS) {
		display_registers(cpu_config, _hw_tty);
	}
}


void BAFFA1_REGISTERS::display_registers(BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty) {
	char str_out[255];
	_hw_tty.print("+------------------------------------------------------------------------------\n");
	_hw_tty.print("+ REGISTERS\n\n");

	_hw_tty.print("  A: "); print_byte_bin(str_out, Ah.value()); sprintf(str_out, " "); print_byte_bin(str_out, Al.value()); _hw_tty.print(str_out); _hw_tty.print(" | ");
	_hw_tty.print("B: "); print_byte_bin(str_out, Bh.value()); sprintf(str_out, " "); print_byte_bin(str_out, Bl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  C: "); print_byte_bin(str_out, Ch.value()); sprintf(str_out, " "); print_byte_bin(str_out, Cl.value()); _hw_tty.print(str_out); _hw_tty.print(" | ");
	_hw_tty.print("D: "); print_byte_bin(str_out, Dh.value()); sprintf(str_out, " "); print_byte_bin(str_out, Dl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");

	_hw_tty.print("  Ax (16bit) Accumulator | Bx (16bit) Base\n");
	_hw_tty.print("  Cx (16bit) Counter     | Dx (16bit) Data\n");
	_hw_tty.print("\n");

	//Pointer Registers
	_hw_tty.print("  BP: "); print_byte_bin(str_out, BPh.value()); sprintf(str_out, " "); print_byte_bin(str_out, BPl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  SP: "); print_byte_bin(str_out, SPh.value()); sprintf(str_out, " "); print_byte_bin(str_out, SPl.value()); _hw_tty.print(str_out); _hw_tty.print(" | ");
	_hw_tty.print("SSP: "); print_byte_bin(str_out, SSPh.value()); sprintf(str_out, " "); print_byte_bin(str_out, SSPl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  BP (16bit) Base Pointer  (Used to manage stack frames)\n");
	_hw_tty.print("  SP (16bit) Stack Pointer | SSP (16bit) Supervisor Stack Pointer\n");
	_hw_tty.print("\n");

	//Index Registers
	_hw_tty.print("  SI: "); print_byte_bin(str_out, SIh.value()); sprintf(str_out, " "); print_byte_bin(str_out, SIl.value()); _hw_tty.print(str_out); _hw_tty.print(" | ");
	_hw_tty.print("DI: "); print_byte_bin(str_out, DIh.value()); sprintf(str_out, " "); print_byte_bin(str_out, DIl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  PC: "); print_byte_bin(str_out, PCh.value()); sprintf(str_out, " "); print_byte_bin(str_out, PCl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  SI (16bit) Source index (Src addr for string operations)\n");
	_hw_tty.print("  DI (16bit) Destination Index (Dest addr for string operations)\n");
	_hw_tty.print("  PC (16bit) Program Counter\n");
	_hw_tty.print("\n");

	_hw_tty.print("  TDR: "); print_byte_bin(str_out, TDRh.value()); sprintf(str_out, " "); print_byte_bin(str_out, TDRl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  TDR (16bit) Temporary Data Register\n");
	_hw_tty.print("\n");

	_hw_tty.print("  INT_MASKS: "); print_byte_bin(str_out, INT_MASKS.value());  _hw_tty.print(str_out);  _hw_tty.print("\n");
	_hw_tty.print("  INT_MASKS (8bit) Interruption Flags\n");
	_hw_tty.print("\n");

	_hw_tty.print("  MAR: "); print_byte_bin(str_out, MARh.value()); sprintf(str_out, " "); print_byte_bin(str_out, MARl.value()); _hw_tty.print(str_out); _hw_tty.print(" | ");
	_hw_tty.print("DI: "); print_byte_bin(str_out, MDRh.value()); sprintf(str_out, " "); print_byte_bin(str_out, MDRl.value()); _hw_tty.print(str_out); _hw_tty.print("\n");
	_hw_tty.print("  MAR (16bit) Memory Address Register\n");
	_hw_tty.print("  MDR (16bit) Memory Data Register\n");
	_hw_tty.print("\n");
	_hw_tty.print("\n");
}
