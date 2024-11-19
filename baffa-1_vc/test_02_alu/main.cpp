//
// main.cpp
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
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "baffa1_alu.h"
#include "baffa1_data_bus.h"
#include "baffa1_controller_bus.h"
#include "baffa1_interruptions_bus.h"

int main(int argc, char** argv) {

	BAFFA1_CONFIG cpu_config;
	//cpu_config.DEBUG_OPCODE = 1;
	cpu_config.DEBUG_MICROCODE = 1;
	cpu_config.DEBUG_UADDRESSER = 1;
	cpu_config.DEBUG_UADDER = 1;

	cpu_config.DEBUG_BUSES = 1;

	cpu_config.DEBUG_UFLAGS = 1;
	cpu_config.DEBUG_ALU = 1;
	cpu_config.DEBUG_ALU_REGISTERS = 1;
	cpu_config.DEBUG_ALU_STATUS_FLAGS = 1;
	cpu_config.DEBUG_ALU_MSWh_FLAGS = 1;

	cpu_config.DEBUG_TRACE_RDREG = 1;
	cpu_config.DEBUG_TRACE_WRREG = 1;
	cpu_config.DEBUG_REGISTERS = 1;


	cpu_config.DEBUG_TRACE_RDMEM = 1;
	cpu_config.DEBUG_TRACE_WRMEM = 1;
	cpu_config.DEBUG_MEMORY = 1;

	cpu_config.DEBUG_UART = 1;
	cpu_config.DEBUG_IDE = 1;
	cpu_config.DEBUG_RTC = 1;
	cpu_config.DEBUG_TIMER = 1;

	cpu_config.DEBUG_LOG_OPCODE = 1;
	cpu_config.DEBUG_LITE = 1;
	cpu_config.DEBUG_LITE_CYCLES = 1;

	BAFFA1_CONTROLLER_BUS controller_bus;
	BAFFA1_INTERRUPTIONS_BUS interruptions_bus;
	
	BAFFA1_ALU alu;
	alu.cpu_config = cpu_config;

	alu.init();
	controller_bus.reset();
	interruptions_bus.reset();
	//data_bus.reset();


	while (true) {

		//NEXT MICRO INSTRUCTION
		// microcontroller next (typ)
		controller_bus.cond_flags_src = 0; // from CPU FLAGS;
		//controller_bus.cond_flags_src = 1; // from Microcode FLAGS;

		controller_bus.cond_inv = 0; // cond_sel signal
		//controller_bus.cond_inv = 1; // inverse (not) cond_sel signal

		controller_bus.cond_sel = 0; // zf
		//controller_bus.cond_sel = 1; // cf
		//controller_bus.cond_sel = 2; // sf
		//controller_bus.cond_sel = 3; // of
		//controller_bus.cond_sel = 4; // L sf xor of
		//controller_bus.cond_sel = 5; // LE zf or (sf xor of)
		//controller_bus.cond_sel = 6; // LEU zf or cf
		//controller_bus.cond_sel = 7; // DMA_REQ
		//controller_bus.cond_sel = 8; // MSWl Status Mode
		//controller_bus.cond_sel = 9; // Wait
		//controller_bus.cond_sel = 10; // Pending
		//controller_bus.cond_sel = 11; // Ext_Input
		//controller_bus.cond_sel = 12; // MSWl Status Dir
		//controller_bus.cond_sel = 13; // MSWl Display Load

		//microcontroller offset; // valor com 7 bits
		//microcontroller offset; // valor com 7 bits
		controller_bus.imm = 0; // valor com 8 bits;

		///////////////////////////////////


		//microcode.sequencer_start(0, data_bus, controller_bus);		// Sets Microcode
		//microcode.debugger(0, controller_bus);
		//microcode.sequencer_end(controller_bus);

		// ALU INPUTS A
		// vindo do W_BUS
		controller_bus.alu_a_src = 0; // Al
		//controller_bus.alu_a_src = 1; // Ah
		//controller_bus.alu_a_src = 2; // Bl
		//controller_bus.alu_a_src = 3; // Bh
		//controller_bus.alu_a_src = 4; // Cl
		//controller_bus.alu_a_src = 5; // Ch
		//controller_bus.alu_a_src = 6; // Dl
		//controller_bus.alu_a_src = 7; // Dh
		//controller_bus.alu_a_src = 8; // sp_l
		//controller_bus.alu_a_src = 9; // sp_h
		//controller_bus.alu_a_src = 10; // bp_l
		//controller_bus.alu_a_src = 11; // bp_h
		//controller_bus.alu_a_src = 12; // si_l
		//controller_bus.alu_a_src = 13; // si_h
		//controller_bus.alu_a_src = 14; // di_l
		//controller_bus.alu_a_src = 15; // di_h
		//controller_bus.alu_a_src = 16; // pc_l
		//controller_bus.alu_a_src = 17; // pc_h
		//controller_bus.alu_a_src = 18; // mar_l
		//controller_bus.alu_a_src = 19; // mar_h
		//controller_bus.alu_a_src = 20; // mdr_l
		//controller_bus.alu_a_src = 21; // mdr_h
		//controller_bus.alu_a_src = 22; // tdr_l
		//controller_bus.alu_a_src = 23; // tdr_h
		//controller_bus.alu_a_src = 24; // ksp_l
		//controller_bus.alu_a_src = 25; // ksp_h

		//controller_bus.alu_a_src = 26; // int_vector
		//controller_bus.alu_a_src = 27; // int_masks
		//controller_bus.alu_a_src = 28; // int_status

		// vindo dos registradores internos da ALU
		//controller_bus.alu_a_src /bit 5 - define  origem XBUS se vem do W ou dos registradores
		//controller_bus.alu_a_src /bit 5 = 1
		//controller_bus.alu_a_src = 32; // 0 MSWh arithmetic flags
		//controller_bus.alu_a_src = 33; // 1 MSWL status flags
		//controller_bus.alu_a_src = 34; // 2 Gl
		//controller_bus.alu_a_src = 35; // 3 Gh
						
		// ALU INPUTS B
		//controller_bus.alu_b_src  /bit 2 - define  origem YBUS se vem do IMM ou do K
		//controller_bus.alu_b_src = 0; // immediate

		// vindo do K_BUS
		controller_bus.alu_b_src = 4; // mdr_l
		//controller_bus.alu_b_src = 5; // mdr_h
		//controller_bus.alu_b_src = 6; // tdr_l
		//controller_bus.alu_b_src = 7; // tdr_h

		// CARRY IN / OUT
		// entrada do cf
		controller_bus.alu_cf_in_inv = 0; // src cf 
		//controller_bus.alu_cf_in_inv = 1; // inverse(not) src cf
		
		controller_bus.alu_cf_in_src = 0; // valor 1 (vcc) ou valor 0 (gnd) se invertido
		//controller_bus.alu_cf_in_src = 1; // cf ou !cf
		//controller_bus.alu_cf_in_src = 2; // u_cf ou !u_cf
		// saída do cf
		controller_bus.alu_cf_out_inv = 0; // cf out 
		//controller_bus.alu_cf_out_inv = 1; // inverse (not) cf out 

		// SHIFT SRC
		// Entrada p/ shift
		controller_bus.shift_src = 0; // valor zero (gnd)
		//controller_bus.shift_src = 1; // u_cf
		//controller_bus.shift_src = 2; // cf
		//controller_bus.shift_src = 3; // alu_result[0]
		//controller_bus.shift_src = 4; // alu_result[7]

		// ALU TO ZBUS
		// Saída da ALU para o ZBUS
		controller_bus.zbus_out_src = 0; // normal alu result
		//controller_bus.zbus_out_src = 1; // shift result right
		//controller_bus.zbus_out_src = 2; // shift result left
		//controller_bus.zbus_out_src = 3; // sign extended

		// MICRO FLAGS IN
		// Entrada de Flags Microcodigo
		controller_bus.uzf_in_src = 0; // unchanged
		//controller_bus.uzf_in_src = 1; // ALU_ZF
		//controller_bus.uzf_in_src = 2; // ALU_ZF && ZF
		//controller_bus.uzf_in_src = 3; // ZBUS[0]
		
		controller_bus.ucf_in_src = 0; // unchanged
		//controller_bus.ucf_in_src = 1; // ALU_FINAL_CF
		//controller_bus.ucf_in_src = 2; // alu_output[0]
		//controller_bus.ucf_in_src = 3; // alu_output[7]

		controller_bus.usf_in_src = 0; // unchanged
		//controller_bus.usf_in_src = 1; // ZBUS[7]

		controller_bus.uof_in_src = 0; // unchanged
		//controller_bus.uof_in_src = 1; // ALU_OF

		// ARITHMETIC FLAGS IN
		// Entrada de Flags Aritmeticas
		controller_bus.zf_in_src = 0; // unchanged
		//controller_bus.zf_in_src = 1; // ALU_ZF
		//controller_bus.zf_in_src = 2; // ALU_ZF && ZF
		//controller_bus.zf_in_src = 3; // ZBUS[0]

		controller_bus.cf_in_src = 0; // unchanged
		//controller_bus.cf_in_src = 1; // ALU_FINAL_CF
		//controller_bus.cf_in_src = 2; // alu_output[0]
		//controller_bus.cf_in_src = 3; // ZBUS[1]
		//controller_bus.cf_in_src = 4; // alu_output[7]

		controller_bus.sf_in_src = 0; // unchanged
		//controller_bus.sf_in_src = 1; // ZBUS[7]
		//controller_bus.sf_in_src = 2; // valor 0 (gnd)
		//controller_bus.sf_in_src = 3; // ZBUS[2]

		controller_bus.of_in_src = 0; // unchanged
		//controller_bus.of_in_src = 1; // ALU_OF
		//controller_bus.of_in_src = 2; // ZBUS[7]
		//controller_bus.of_in_src = 3; // ZBUS[3]
		//controller_bus.of_in_src = 4; // U_SF XOR ZBUS[7]
	
		// ALU OPERATION
		controller_bus.alu_mode = 0;
		controller_bus.alu_op = 0b1001; // plus
		//controller_bus.alu_op = 0b0110; // minus

		//controller_bus.alu_mode = 1;
		//controller_bus.alu_op = 0b1011; // and		
		//controller_bus.alu_op = 0b1110; // or
		//controller_bus.alu_op = 0b0110; // xor
		//controller_bus.alu_op = 0b1111; // A		
		//controller_bus.alu_op = 0b1010; // B
		//controller_bus.alu_op = 0b0000; // not A
		//controller_bus.alu_op = 0b0101; // not B
		//controller_bus.alu_op = 0b0100; // nand
		//controller_bus.alu_op = 0b0001; // nor
		//controller_bus.alu_op = 0b1001; // nxor


		//CF Carry Flag
		//SF Sign Flag
		//ZF Zero Flag
		//OF Overflow Flag

		BAFFA1_BYTE w_bus = 0;
		BAFFA1_BYTE k_bus = 0;

		alu.exec(controller_bus, interruptions_bus, w_bus, k_bus);
		alu.debugger(controller_bus, alu.alu_bus);

		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

		cout << "Press enter to continue or 'q' enter to quit..." << endl;
		char c = cin.get();

		if (c == 'q') break;
	}


	return 0;
}
