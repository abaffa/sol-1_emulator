//
// baffa1_cbus.cpp
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
#include "baffa1_controller_bus.h"
#include "baffa1_utils.h"

void BAFFA1_CONTROLLER_BUS::init() {
	this->reset();
}

void BAFFA1_CONTROLLER_BUS::reset() {
	// ROM 0
	//this->next = 0;

	//change MUX

	// ROM 1
	//this->u_offset = 0;
	this->cond_inv = 0;
	this->cond_flags_src = 0;
	this->cond_sel = 0;
	//this->u_escape_0 = 0;

	// ROM 2
	this->uzf_in_src = 0;
	this->ucf_in_src = 0;
	this->usf_in_src = 0;
	this->uof_in_src = 0;
	//this->ir_wrt = 0;
	this->status_wrt = 0;

	// ROM 3
	this->shift_src = 0;
	this->zbus_out_src = 0;
	this->alu_a_src = 0;

	// ROM 4
	this->alu_op = 0;
	this->alu_mode = 0;

	// ROM 5
	this->alu_cf_in_src = 0;
	this->alu_cf_in_inv = 0;
	this->zf_in_src = 0;
	this->alu_cf_out_inv = 0;
	this->cf_in_src = 0;

	// ROM 6
	this->sf_in_src = 0;
	this->of_in_src = 0;
	this->rd = 0;
	this->wr = 0;

	// ROM 7
	this->alu_b_src = 0;
	this->display_reg_load = 0;
	this->dl_wrt = 0;
	this->dh_wrt = 0;
	this->cl_wrt = 0;
	this->ch_wrt = 0;

	// ROM 8
	this->bl_wrt = 0;
	this->bh_wrt = 0;
	this->al_wrt = 0;
	this->ah_wrt = 0;
	this->mdr_in_src = 0;
	this->mdr_out_src = 0;
	this->mdr_out_en = 0;
	this->mdrl_wrt = 0;

	// ROM 9
	this->mdrh_wrt = 0;
	this->tdrl_wrt = 0;
	this->tdrh_wrt = 0;
	this->dil_wrt = 0;
	this->dih_wrt = 0;
	this->sil_wrt = 0;
	this->sih_wrt = 0;
	this->marl_wrt = 0;

	// ROM 10
	this->marh_wrt = 0;
	this->bpl_wrt = 0;
	this->bph_wrt = 0;
	this->pcl_wrt = 0;
	this->pch_wrt = 0;
	this->spl_wrt = 0;
	this->sph_wrt = 0;
	//this->u_escape_1 = 0;

	// ROM 11
	//this->u_esc_in_src = 0;
	this->int_vector_wrt = 0;
	this->mask_flags_wrt = 0;
	this->mar_in_src = 0;
	this->int_ack = 0;
	this->clear_all_ints = 0;
	this->ptb_wrt = 0;
	this->pagtbl_ram_we = 0;

	// ROM 12
	this->mdr_to_pagtbl_en = 0;
	this->force_user_ptb = 0;
	// empty bit 3
	// empty bit 4
	// empty bit 5
	// empty bit 6
	this->gl_wrt = 0;
	this->gh_wrt = 0;

	// ROM 13
	this->imm = 0;


	/////////////////////////
}

BAFFA1_BYTE BAFFA1_CONTROLLER_BUS::bus_tristate(BAFFA1_BYTE MSWl_value) {
	return get_byte_bit(MSWl_value, MSWl_DMA_ACK) | get_byte_bit(MSWl_value, MSWl_HALT); //IC151
}


BAFFA1_BYTE BAFFA1_CONTROLLER_BUS::bus_rd(BAFFA1_BYTE MSWl_value, BAFFA1_BYTE rd, BAFFA1_BYTE panel_rd) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(MSWl_value) != 0x00)
		ret = panel_rd;
	else
		ret = rd;

	return (~ret) & 0b00000001;
}

BAFFA1_BYTE BAFFA1_CONTROLLER_BUS::bus_wr(BAFFA1_BYTE MSWl_value, BAFFA1_BYTE wr, BAFFA1_BYTE panel_wr) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(MSWl_value) != 0x00)
		ret = panel_wr;
	else
		ret = wr;

	return (~ret) & 0b00000001;
}


BAFFA1_BYTE BAFFA1_CONTROLLER_BUS::bus_mem_io(BAFFA1_BYTE MSWl_value, BAFFA1_BYTE mem_io, BAFFA1_BYTE panel_mem_io) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(MSWl_value) != 0x00)
		ret = panel_mem_io;
	else
		ret = mem_io;

	return ret;
}