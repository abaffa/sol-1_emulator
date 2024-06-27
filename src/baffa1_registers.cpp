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
#include "utils.h"

BAFFA1_REGISTERS::BAFFA1_REGISTERS() {}


BAFFA1_DWORD BAFFA1_REGISTERS::value(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h) {

	return l.value() | (((BAFFA1_DWORD)h.value()) << 8);
}

void BAFFA1_REGISTERS::set(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h, BAFFA1_DWORD v) {
	l.set((BAFFA1_BYTE)(v & 0b11111111));
	h.set((BAFFA1_BYTE)((v >> 8) & 0b11111111));
}

void BAFFA1_REGISTERS::reset(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h) {
	l.reset();
	h.reset();
}



void BAFFA1_REGISTERS::refresh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE data_bus, BAFFA1_BYTE u_sf, BAFFA1_CONFIG& config, FILE *fa) {
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

	if (controller_bus->status_wrt == 0x00) {
		this->MSWl.set(alu_bus.z_bus);
	}



	///////////////////////////////////////////////////////////////////////////
	// READ DATA
	//DATA REGISTERS
	if (controller_bus->ah_wrt == 0x00) { this->Ah.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Ah", alu_bus.z_bus); } }
	if (controller_bus->al_wrt == 0x00) { this->Al.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Al", alu_bus.z_bus); } }

	if (controller_bus->bh_wrt == 0x00) { this->Bh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Bh", alu_bus.z_bus); } }
	if (controller_bus->bl_wrt == 0x00) { this->Bl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Bl", alu_bus.z_bus); } }

	if (controller_bus->ch_wrt == 0x00) { this->Ch.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Ch", alu_bus.z_bus); } }
	if (controller_bus->cl_wrt == 0x00) { this->Cl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Cl", alu_bus.z_bus); } }

	if (controller_bus->dh_wrt == 0x00) { this->Dh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Dh", alu_bus.z_bus); } }
	if (controller_bus->dl_wrt == 0x00) { this->Dl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Dl", alu_bus.z_bus); } }

	if (controller_bus->gh_wrt == 0x00) { this->Gh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Gh", alu_bus.z_bus); } }
	if (controller_bus->gl_wrt == 0x00) { this->Gl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"Gl", alu_bus.z_bus); } }

	//Pointer Registers
	if (controller_bus->bph_wrt == 0x00) { this->BPh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"BPh", alu_bus.z_bus); } }
	if (controller_bus->bpl_wrt == 0x00) { this->BPl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"BPl", alu_bus.z_bus); } }

	// MSWl_CPU_MODE = 0 >> update both sp and ssp 
	// MSWl_CPU_MODE = 1 >> update only sp
	if (!get_byte_bit(this->MSWl.value(), MSWl_CPU_MODE)) {
		if (controller_bus->sph_wrt == 0x00){ this->SSPh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SSPh", alu_bus.z_bus); } }
		if (controller_bus->spl_wrt == 0x00){ this->SSPl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SSPl", alu_bus.z_bus); } }
	}

	if (controller_bus->sph_wrt == 0x00) { this->SPh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SPh", alu_bus.z_bus); } }
	if (controller_bus->spl_wrt == 0x00) { this->SPl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SPl", alu_bus.z_bus); } }

	//Index Registers
	if (controller_bus->sih_wrt == 0x00) { this->SIh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SIh", alu_bus.z_bus); } }
	if (controller_bus->sil_wrt == 0x00) { this->SIl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"SIl", alu_bus.z_bus); } }

	if (controller_bus->dih_wrt == 0x00) { this->DIh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"DIh", alu_bus.z_bus); } }
	if (controller_bus->dil_wrt == 0x00) { this->DIl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"DIl", alu_bus.z_bus); } }

	if (controller_bus->pch_wrt == 0x00) { this->PCh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"PCh", alu_bus.z_bus); } }
	if (controller_bus->pcl_wrt == 0x00) { this->PCl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"PCl", alu_bus.z_bus); } }

	if (controller_bus->tdrh_wrt == 0x00) { this->TDRh.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"TDRh", alu_bus.z_bus); } }
	if (controller_bus->tdrl_wrt == 0x00) { this->TDRl.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"TDRl", alu_bus.z_bus); } }

	if (controller_bus->ptb_wrt == 0x00) { this->PTB.set(alu_bus.z_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"PTB", alu_bus.z_bus); } }

	/////////////////////////////////////////////////////////////////////////////
	if (controller_bus->mask_flags_wrt == 0x00) this->INT_MASKS.set(alu_bus.z_bus);
	/////////////////////////////////////////////////////////////////////////////
	// SET MDR
	//IC7 //IC24 //IC19 //IC183
	if (controller_bus->mdrl_wrt == 0x00) { this->MDRl.set(controller_bus->mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus);  if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MDRl", (controller_bus->mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus)); } }
	if (controller_bus->mdrh_wrt == 0x00) { this->MDRh.set(controller_bus->mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus); if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MDRh", (controller_bus->mdr_in_src == 0x00 ? alu_bus.z_bus : data_bus)); } }
	////////////////////////////////////////////////////////////////////////////
	//MEMORY SET MAR

	if (controller_bus->marl_wrt == 0x00) {
		//IC131 //IC128			
		if (controller_bus->mar_in_src == 0x00)
		{
			this->MARl.set(alu_bus.z_bus);
			if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MARl", alu_bus.z_bus); }
		}
		else {
			this->MARl.set(this->PCl.value());
			if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MARl", this->PCl.value()); }
		}
	}

	if (controller_bus->marh_wrt == 0x00) {
		//IC129 //IC132			
		if (controller_bus->mar_in_src == 0x00) {
			this->MARh.set(alu_bus.z_bus);
			if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MARh", alu_bus.z_bus); }
		}
		else {
			this->MARh.set(this->PCh.value());
			if (config.DEBUG_TRACE_WRREG) { reg8bit_print(fa, (char*)"WRITE", (char*)"MARh", this->PCh.value()); }
		}
	}
}




void  BAFFA1_REGISTERS::mswh_flags_desc(HW_TTY& hw_tty) {

	BAFFA1_BYTE b = this->MSWh.value();
	hw_tty.print(" [");
	if (get_byte_bit(b, MSWh_ZF) != 0x00)
		hw_tty.print("Z"); else hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_CF) != 0x00)
		hw_tty.print("C"); else hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_SF) != 0x00)
		hw_tty.print("S"); else hw_tty.print(" ");
	if (get_byte_bit(b, MSWh_OF) != 0x00)
		hw_tty.print("O"); else hw_tty.print(" ");
	hw_tty.print("]");
}




void BAFFA1_REGISTERS::mswl_status_desc(HW_TTY& hw_tty) {

	BAFFA1_BYTE b = this->MSWl.value();
	if (get_byte_bit(b, MSWl_DMA_ACK) != 0x00)
		hw_tty.print(" | dma_ack ");
	if (get_byte_bit(b, MSWl_INTERRUPT_ENABLE) != 0x00)
		hw_tty.print(" | interrupt_enable ");
	if (get_byte_bit(b, MSWl_CPU_MODE) != 0x00)
		hw_tty.print(" | cpu_mode ");
	if (get_byte_bit(b, MSWl_PAGING_EN) != 0x00)
		hw_tty.print(" | paging_en ");
	if (get_byte_bit(b, MSWl_HALT) != 0x00)
		hw_tty.print(" | halt ");
	if (get_byte_bit(b, MSWl_DISPLAY_REG_LOAD) != 0x00)
		hw_tty.print(" | display_reg_load ");
	if (get_byte_bit(b, MSWl_DIR) != 0x00)
		hw_tty.print(" | dir ");
}





BAFFA1_BYTE BAFFA1_REGISTERS::get_MSWh_ZF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_ZF = 0x00;

	switch (controller_bus->zf_in_src & 0b00000011) {
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


BAFFA1_BYTE  BAFFA1_REGISTERS::get_MSWh_CF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_CF = 0x00;

	switch (controller_bus->cf_in_src & 0b00000111) {
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

BAFFA1_BYTE BAFFA1_REGISTERS::get_MSWh_SF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus) {

	BAFFA1_BYTE inMSWh_SF = 0x00;

	switch (controller_bus->sf_in_src & 0b00000011) {
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

BAFFA1_BYTE BAFFA1_REGISTERS::get_MSWh_OF(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf) {

	BAFFA1_BYTE inMSWh_OF = 0x00;

	switch (controller_bus->of_in_src & 0b00000111) {
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


void BAFFA1_REGISTERS::refresh_reg_flags_MSWh(struct baffa1_controller_rom *controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE u_sf) {

	BAFFA1_BYTE inMSWh_ZF = get_MSWh_ZF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_CF = get_MSWh_CF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_SF = get_MSWh_SF(controller_bus, alu_bus);
	BAFFA1_BYTE inMSWh_OF = get_MSWh_OF(controller_bus, alu_bus, u_sf);

	BAFFA1_BYTE inMSW_H = set_byte_bit(inMSWh_ZF, 0) | set_byte_bit(inMSWh_CF, 1) | set_byte_bit(inMSWh_SF, 2) | set_byte_bit(inMSWh_OF, 3);
	this->MSWh.set(inMSW_H);
	//}
}
