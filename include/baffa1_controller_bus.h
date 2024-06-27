//
// baffa1_controller_bus.h
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
#ifndef BAFFA1CONTROLLERBUS_H
#define BAFFA1CONTROLLERBUS_H

#include "config.h"

struct baffa1_controller_rom {

	//ROM 01
	//BAFFA1_BYTE next;		//TYP
	//BAFFA1_BYTE u_offset;

	//ROM 02
	BAFFA1_BYTE cond_inv;
	BAFFA1_BYTE cond_flags_src;
	BAFFA1_BYTE cond_sel;
	//BAFFA1_BYTE u_escape_0;

	//ROM 03
	BAFFA1_BYTE uzf_in_src;
	BAFFA1_BYTE ucf_in_src;
	BAFFA1_BYTE usf_in_src;
	BAFFA1_BYTE uof_in_src;
	//BAFFA1_BYTE ir_wrt;
	BAFFA1_BYTE status_wrt; //mswl_wrt // status (flags de controle)

	//ROM 04
	BAFFA1_BYTE shift_src;
	BAFFA1_BYTE zbus_out_src;
	BAFFA1_BYTE alu_a_src;

	//ROM 05
	BAFFA1_BYTE alu_op;
	BAFFA1_BYTE alu_mode;

	//ROM 06
	BAFFA1_BYTE alu_cf_in_src;
	BAFFA1_BYTE alu_cf_in_inv;
	BAFFA1_BYTE zf_in_src; // ZeroFlag
	BAFFA1_BYTE alu_cf_out_inv;
	BAFFA1_BYTE cf_in_src; // Carry Flag

	//ROM 07
	BAFFA1_BYTE sf_in_src; // Sign Flag
	BAFFA1_BYTE of_in_src; // Overflow Flag
	BAFFA1_BYTE rd;
	BAFFA1_BYTE wr;

	//ROM 08
	BAFFA1_BYTE alu_b_src;
	BAFFA1_BYTE display_reg_load;
	BAFFA1_BYTE dl_wrt;
	BAFFA1_BYTE dh_wrt;
	BAFFA1_BYTE cl_wrt;
	BAFFA1_BYTE ch_wrt;

	//ROM 09
	BAFFA1_BYTE bl_wrt;
	BAFFA1_BYTE bh_wrt;
	BAFFA1_BYTE al_wrt;
	BAFFA1_BYTE ah_wrt;
	BAFFA1_BYTE mdr_in_src;
	BAFFA1_BYTE mdr_out_src;
	BAFFA1_BYTE mdr_out_en;
	BAFFA1_BYTE mdrl_wrt;

	//ROM 10
	BAFFA1_BYTE mdrh_wrt;
	BAFFA1_BYTE tdrl_wrt;
	BAFFA1_BYTE tdrh_wrt;
	BAFFA1_BYTE dil_wrt;
	BAFFA1_BYTE dih_wrt;
	BAFFA1_BYTE sil_wrt;
	BAFFA1_BYTE sih_wrt;
	BAFFA1_BYTE marl_wrt;

	//ROM 11
	BAFFA1_BYTE marh_wrt;
	BAFFA1_BYTE bpl_wrt;
	BAFFA1_BYTE bph_wrt;
	BAFFA1_BYTE pcl_wrt;
	BAFFA1_BYTE pch_wrt;
	BAFFA1_BYTE spl_wrt;
	BAFFA1_BYTE sph_wrt;
	//BAFFA1_BYTE u_escape_1;

	//ROM 12
	//BAFFA1_BYTE u_esc_in_src;
	BAFFA1_BYTE int_vector_wrt;
	BAFFA1_BYTE mask_flags_wrt;
	BAFFA1_BYTE mar_in_src;
	BAFFA1_BYTE int_ack;
	BAFFA1_BYTE clear_all_ints;
	BAFFA1_BYTE ptb_wrt;
	BAFFA1_BYTE pagtbl_ram_we;

	//ROM 13
	BAFFA1_BYTE mdr_to_pagtbl_en;
	BAFFA1_BYTE force_user_ptb;
	//BAFFA1_BYTE IC6_3;
	//BAFFA1_BYTE IC6_4;
	//BAFFA1_BYTE IC6_5;
	//BAFFA1_BYTE IC6_6;
	BAFFA1_BYTE gl_wrt;
	BAFFA1_BYTE gh_wrt;

	//ROM 14
	BAFFA1_BYTE imm;

	//ROM 15
	//BAFFA1_BYTE IC8_1;
	//BAFFA1_BYTE IC8_2;
	//BAFFA1_BYTE IC8_3;
	//BAFFA1_BYTE IC8_4;
	//BAFFA1_BYTE IC8_5;
	//BAFFA1_BYTE IC8_6;
	//BAFFA1_BYTE IC8_7;
	//BAFFA1_BYTE IC8_8;




	//?????????????
	BAFFA1_BYTE memory_io; // bus_mem_io //?????????????????
	
	
	BAFFA1_BYTE page_present; //memory
	BAFFA1_BYTE page_writable; //memory
	//?????????????


	//
	 //??
	BAFFA1_BYTE any_interruption; //memory
	
	BAFFA1_BYTE int_pending; //alu, memory

	//

	BAFFA1_BYTE int_vector; // register, int
	BAFFA1_BYTE int_status;// register, int


	BAFFA1_BYTE int_request; //Memory	Int/Clk
	BAFFA1_BYTE int_req; // Int/Clk
	//

	BAFFA1_BYTE dma_req;  //alu, memory
	BAFFA1_BYTE wait; //alu, memory
	BAFFA1_BYTE ext_input; //alu, memory

	

	BAFFA1_BYTE panel_regsel; //alu
	BAFFA1_BYTE panel_rd;//memory
	BAFFA1_BYTE panel_wr;//memory
	BAFFA1_BYTE panel_mem_io; //memory

	//panel
	long panel_address;
	BAFFA1_BYTE panel_data;
	BAFFA1_BYTE panel_req;
	BAFFA1_BYTE panel_run = 0;
	BAFFA1_BYTE panel_step = 0;
	BAFFA1_BYTE panel_microcodestep = 0;

	BAFFA1_BYTE clk = 1; 


	BAFFA1_BYTE reset = 0; //controller, alu, memory
	BAFFA1_BYTE restart = 0;
};

#endif