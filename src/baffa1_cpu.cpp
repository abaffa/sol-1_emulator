//
// baffa1_cpu.cpp
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
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <time.h>

#include "baffa1_cpu.h"
#include "utils.h"

/*
const char* BAFFA1_ROM_CONTROL_LIST[] = {
"next_0", "next_1", "offset_0", "offset_1", "offset_2", "offset_3", "offset_4", "offset_5", "offset_6", "cond_inv", "cond_flags_src", "cond_sel_0",
"cond_sel_1", "cond_sel_2", "cond_sel_3", "ESCAPE", "uzf_in_src_0", "uzf_in_src_1", "ucf_in_src_0", "ucf_in_src_1", "usf_in_src", "uof_in_src", "IR_wrt", "status_wrt",
"shift_src_0", "shift_src_1", "shift_src_2", "zbus_out_src_0", "zbus_out_src_1", "alu_a_src_0", "alu_a_src_1", "alu_a_src_2", "alu_a_src_3", "alu_a_src_4", "alu_a_src_5",
"alu_op_0", "alu_op_1", "alu_op_2", "alu_op_3", "alu_mode", "alu_cf_in_src_0", "alu_cf_in_src_1", "alu_cf_in_inv", "zf_in_src_0", "zf_in_src_1", "alu_cf_out_inv",
"cf_in_src_0", "cf_in_src_1", "cf_in_src_2", "sf_in_src_0", "sf_in_src_1", "of_in_src_0", "of_in_src_1", "of_in_src_2", "rd", "wr", "alu_b_src_0", "alu_b_src_1",
"alu_b_src_2", "display_reg_load", "dl_wrt", "dh_wrt", "cl_wrt", "ch_wrt", "bl_wrt", "bh_wrt", "al_wrt", "ah_wrt", "mdr_in_src", "mdr_out_src", "mdr_out_en",
"mdrl_wrt", "mdrh_wrt", "tdrl_wrt", "tdrh_wrt", "dil_wrt", "dih_wrt", "sil_wrt", "sih_wrt", "marl_wrt", "marh_wrt", "bpl_wrt", "bph_wrt", "pcl_wrt", "pch_wrt",
"spl_wrt", "sph_wrt", "-", "-", "int_vector_wrt", "mask_flags_wrt", "mar_in_src", "int_ack", "clear_all_ints", "ptb_wrt", "pagtbl_ram_we", "mdr_to_pagtbl_en",
"force_user_ptb", "-", "-", "-", "-", "gl_wrt", "gh_wrt", "imm_0", "imm_1", "imm_2", "imm_3", "imm_4", "imm_5", "imm_6", "imm_7", "-", "-", "-", "-", "-", "-", "-", "-"
};
*/

void BAFFA1_CPU::init(HW_TTY& hw_tty)
{	this->alu.baffa1_alu_init();

	this->microcode.init(hw_tty);

	reset();

	this->display_reg_load = 0;
	this->config.DEBUG_MICROCODE = INI_DEBUG_MICROCODE;
	this->config.DEBUG_UADDRESSER = INI_DEBUG_UADDRESSER;
	this->config.DEBUG_UADDER = INI_DEBUG_UADDER;
	this->config.DEBUG_UFLAGS = INI_DEBUG_UFLAGS;


	this->config.DEBUG_BUSES = INI_DEBUG_BUSES;
	this->config.DEBUG_ALU = INI_DEBUG_ALU;

	this->config.DEBUG_TRACE_RDREG = INI_DEBUG_TRACE_RDREG;
	this->config.DEBUG_TRACE_WRREG = INI_DEBUG_TRACE_WRREG;
	this->config.DEBUG_REGISTERS = INI_DEBUG_REGISTERS;

	this->config.DEBUG_TRACE_RDMEM = INI_DEBUG_TRACE_RDMEM;
	this->config.DEBUG_TRACE_WRMEM = INI_DEBUG_TRACE_WRMEM;
	this->config.DEBUG_MEMORY = INI_DEBUG_MEMORY;

	this->config.DEBUG_LITE = INI_DEBUG_LITE;
	this->config.DEBUG_LITE_CYCLES = INI_DEBUG_LITE_CYCLES;

	this->config.DEBUG_UART = INI_DEBUG_UART;
	this->config.DEBUG_IDE = INI_DEBUG_IDE;
	this->config.DEBUG_RTC = INI_DEBUG_RTC;
	this->config.DEBUG_TIMER = INI_DEBUG_TIMER;

	this->config.SERVER = INI_SERVER;
	this->config.WEB_SERVER = INI_WEB_SERVER;
	this->config.DEBUG_LOG_OPCODE = INI_DEBUG_LOG_OPCODE;
}

void BAFFA1_CPU::reset()
{

	//DATA REGISTERS
	BAFFA1_REGISTERS::reset(this->registers.Al, this->registers.Ah); // AX (16bit) Accumulator	(Ah/Al)
	BAFFA1_REGISTERS::reset(this->registers.Bl, this->registers.Bh); // BX (16bit) Base		(Bh/Bl)
	BAFFA1_REGISTERS::reset(this->registers.Cl, this->registers.Ch); // CX (16bit) Counter		(Ch/Cl)
	BAFFA1_REGISTERS::reset(this->registers.Dl, this->registers.Dh); // DX (16bit) Data		(Dh/Dl)
	BAFFA1_REGISTERS::reset(this->alu.Gl, this->alu.Gh); // GX (16bit)	Gh/Gl	General Register(For scratch)

	//Pointer Registers
	BAFFA1_REGISTERS::reset(this->registers.BPl, this->registers.BPh); // BP (16bit) Base Pointer  (Used to manage stack frames)
	BAFFA1_REGISTERS::reset(this->registers.SPl, this->registers.SPh); // SP (16bit) Stack Pointer
	BAFFA1_REGISTERS::reset(this->registers.SSPl, this->registers.SSPh); // SSP (16bit) Supervisor Stack Pointer

	//Index Registers
	BAFFA1_REGISTERS::reset(this->registers.SIl, this->registers.SIh); // SI (16bit) Source index (Source address for string operations)
	BAFFA1_REGISTERS::reset(this->registers.DIl, this->registers.DIh); // DI (16bit) Destination Index (Destination address for string operations)
	BAFFA1_REGISTERS::reset(this->registers.PCl, this->registers.PCh); // PC (16bit) Program Counter

	BAFFA1_REGISTERS::reset(this->registers.TDRl, this->registers.TDRh); // TDR (16bit) Temporary Data Register
	this->memory.PTB.reset();  // PTB (8bit) = Page table base

	BAFFA1_REGISTERS::reset(this->alu.MSWl, this->alu.MSWh); // 

	BAFFA1_REGISTERS::reset(this->registers.MARl, this->registers.MARh); // 
	BAFFA1_REGISTERS::reset(this->registers.MDRl, this->registers.MDRh); // 

	this->BKPT = 0xFFFF; // Breakpoint
}

void BAFFA1_CPU::display_registers(HW_TTY& hw_tty) {

	BAFFA1_MWORD memADDR = BAFFA1_REGISTERS::value(this->registers.PCl, this->registers.PCh);
	BAFFA1_BYTE opcode = get_current_memory().read(memADDR);

	char str_out[255];

	hw_tty.print(" DATA REGISTERS                                        | POINTER REGISTERS\n");
	sprintf(str_out, " *A=%04x", BAFFA1_REGISTERS::value(this->registers.Al, this->registers.Ah)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "B=%04x", BAFFA1_REGISTERS::value(this->registers.Bl, this->registers.Bh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "C=%04x", BAFFA1_REGISTERS::value(this->registers.Cl, this->registers.Ch)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "D=%04x", BAFFA1_REGISTERS::value(this->registers.Dl, this->registers.Dh)); hw_tty.print(str_out);
	hw_tty.print(" | ");

	sprintf(str_out, "G=%04x", BAFFA1_REGISTERS::value(this->alu.Gl, this->alu.Gh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "BP=%04x", BAFFA1_REGISTERS::value(this->registers.BPl, this->registers.BPh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "SP=%04x", BAFFA1_REGISTERS::value(this->registers.SPl, this->registers.SPh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "SSP=%04x", BAFFA1_REGISTERS::value(this->registers.SSPl, this->registers.SSPh)); hw_tty.print(str_out);
	hw_tty.print("\n\n");

	hw_tty.print(" MEMORY REGISTERS       INDEX REGISTERS\n");
	sprintf(str_out, " *MAR=%04x", BAFFA1_REGISTERS::value(this->registers.MARl, this->registers.MARh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "MDR=%04x", BAFFA1_REGISTERS::value(this->registers.MDRl, this->registers.MDRh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "SI=%04x", BAFFA1_REGISTERS::value(this->registers.SIl, this->registers.SIh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "DI=%04x", BAFFA1_REGISTERS::value(this->registers.DIl, this->registers.DIh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "PC=%04x", BAFFA1_REGISTERS::value(this->registers.PCl, this->registers.PCh)); hw_tty.print(str_out);
	hw_tty.print(" | ");


	sprintf(str_out, "@PC=%02x:%s", opcode, &this->microcode.rom.rom_desc[0x400000 + (opcode * 256)]); hw_tty.print(str_out);

	if (memADDR < get_current_memory_size() - 3) {
		sprintf(str_out, " [%02x%02x%02x]", get_current_memory().read(memADDR), get_current_memory().read(memADDR + 1), get_current_memory().read(memADDR + 2)); hw_tty.print(str_out);
	}
	else if (memADDR < get_current_memory_size() - 2) {
		sprintf(str_out, " [%02x%02x]", get_current_memory().read(memADDR),
			get_current_memory().read(memADDR + 1)
		); hw_tty.print(str_out);
	}
	else if (memADDR < get_current_memory_size() - 1) {
		sprintf(str_out, " [%02x]", get_current_memory().read(memADDR));
		hw_tty.print(str_out);
	}

	hw_tty.print("\n\n");

	hw_tty.print("                              SPECIAL REGISTERS\n");
	hw_tty.print(" *FLAGS="); print_byte_bin(str_out, this->alu.MSWh.value()); hw_tty.print(str_out); hw_tty.print(":"); this->alu.mswh_flags_desc(hw_tty);
	sprintf(str_out, " | IR=%02x", this->microcode.IR.value()); hw_tty.print(str_out);
	sprintf(str_out, " | TDR=%04x", BAFFA1_REGISTERS::value(this->registers.TDRl, this->registers.TDRh)); hw_tty.print(str_out);
	sprintf(str_out, " | PTB=%02x", this->memory.PTB.value()); hw_tty.print(str_out);
	hw_tty.print("\n");
	hw_tty.print(" *STATS="); print_byte_bin(str_out, this->alu.MSWl.value()); hw_tty.print(str_out);
	hw_tty.print(":       ");
	this->alu.mswl_status_desc(hw_tty);
	hw_tty.print("\n\n");
}

void BAFFA1_CPU::display_registers_lite(HW_TTY& hw_tty) {

	char str_out[255];
	sprintf(str_out, "*   A=%04x", BAFFA1_REGISTERS::value(this->registers.Al, this->registers.Ah)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "  B=%04x", BAFFA1_REGISTERS::value(this->registers.Bl, this->registers.Bh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "  C=%04x", BAFFA1_REGISTERS::value(this->registers.Cl, this->registers.Ch)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "  D=%04x", BAFFA1_REGISTERS::value(this->registers.Dl, this->registers.Dh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "  G=%04x", BAFFA1_REGISTERS::value(this->alu.Gl, this->alu.Gh)); hw_tty.print(str_out);
	hw_tty.print("\n");

	sprintf(str_out, "*  SI=%04x", BAFFA1_REGISTERS::value(this->registers.SIl, this->registers.SIh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, " DI=%04x", BAFFA1_REGISTERS::value(this->registers.DIl, this->registers.DIh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, " PC=%04x", BAFFA1_REGISTERS::value(this->registers.PCl, this->registers.PCh)); hw_tty.print(str_out);
	hw_tty.print(" | ");


	BAFFA1_MWORD memADDR = BAFFA1_REGISTERS::value(this->registers.PCl, this->registers.PCh);
	BAFFA1_BYTE opcode = get_current_memory().read(memADDR);

	sprintf(str_out, "@PC=%02x:%s", opcode, &this->microcode.rom.rom_desc[0x400000 + (opcode * 256)]); hw_tty.print(str_out);

	if (memADDR < get_current_memory_size() - 3) {
		sprintf(str_out, " [%02x%02x%02x]", get_current_memory().read(memADDR), get_current_memory().read(memADDR + 1), get_current_memory().read(memADDR + 2)); hw_tty.print(str_out);
	}
	else if (memADDR < get_current_memory_size() - 2) {
		sprintf(str_out, " [%02x%02x]", get_current_memory().read(memADDR),
			get_current_memory().read(memADDR + 1)
		); hw_tty.print(str_out);
	}
	else if (memADDR < get_current_memory_size() - 1) {
		sprintf(str_out, " [%02x]", get_current_memory().read(memADDR));
		hw_tty.print(str_out);
	}

	hw_tty.print("\n");



	sprintf(str_out, "* MAR=%04x", BAFFA1_REGISTERS::value(this->registers.MARl, this->registers.MARh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "MDR=%04x", BAFFA1_REGISTERS::value(this->registers.MDRl, this->registers.MDRh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "TDR=%04x", BAFFA1_REGISTERS::value(this->registers.TDRl, this->registers.TDRh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "PTB=%02x", this->memory.PTB.value()); hw_tty.print(str_out);
	hw_tty.print("\n");

	sprintf(str_out, "*  BP=%04x", BAFFA1_REGISTERS::value(this->registers.BPl, this->registers.BPh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, " SP=%04x", BAFFA1_REGISTERS::value(this->registers.SPl, this->registers.SPh)); hw_tty.print(str_out);
	hw_tty.print(" | ");
	sprintf(str_out, "SSP=%04x", BAFFA1_REGISTERS::value(this->registers.SSPl, this->registers.SSPh)); hw_tty.print(str_out);
	hw_tty.print("\n");

	sprintf(str_out, "* FLAGS="); print_byte_bin(str_out + strlen(str_out), this->alu.MSWh.value()); hw_tty.print(str_out);
	hw_tty.print("   | ");
	sprintf(str_out, "STATUS="); print_byte_bin(str_out + strlen(str_out), this->alu.MSWl.value()); hw_tty.print(str_out);
	hw_tty.print("\n");
	sprintf(str_out, "* IR: %02x\n", this->microcode.IR.value());
	hw_tty.print("Flags: "); this->alu.mswh_flags_desc(hw_tty); hw_tty.print("\n");
	hw_tty.print("Status: "); this->alu.mswl_status_desc(hw_tty); hw_tty.print("\n");
}




void  BAFFA1_CPU::memory_display(HW_TTY& hw_tty)
{
	char str_out[255];
	unsigned int i = 0, j = 0;
	BAFFA1_DWORD PC = BAFFA1_REGISTERS::value(this->registers.PCl, this->registers.PCh);

	if (this->memory.debug_manual_offset == 0x00) {
		if (0 + this->memory.debug_mem_offset > PC || PC >= 256 + this->memory.debug_mem_offset)
			this->memory.debug_mem_offset = (PC / 0x10) * 0x10;
	}

	hw_tty.print("\n        ");

	for (i = 0; i < 16; i++) {
		sprintf(str_out, "%02x ", i); hw_tty.print(str_out);
	}

	sprintf(str_out, "\n\n %04x ", this->memory.debug_mem_offset); hw_tty.print(str_out);

	for (i = 0 + this->memory.debug_mem_offset; i < 256 + this->memory.debug_mem_offset; i++) {
		if (i % 16 == 0)
			if (PC == i)
				hw_tty.print(" *");
			else
				hw_tty.print("  ");
		if (PC == i || PC - 1 == i) {
			sprintf(str_out, "%02x*", this->memory.rom_bios.read(i)); hw_tty.print(str_out);
		}
		else {
			sprintf(str_out, "%02x ", this->memory.rom_bios.read(i)); hw_tty.print(str_out);
		}

		if ((i + 1) % 16 == 0 && i <= 255 + this->memory.debug_mem_offset) {
			hw_tty.print("  |");
			for (j = (i + 1) - 16; j < (i + 1); j++) {
				if (this->memory.rom_bios.read(j) < 0x20) {
					hw_tty.print(".");
				}
				else {
					sprintf(str_out, "%c", this->memory.rom_bios.read(j)); hw_tty.print(str_out);
				}
			}
			hw_tty.print("|");

			if (i < 255 + this->memory.debug_mem_offset) {
				sprintf(str_out, "\n %04x ", i + 1);
				hw_tty.print(str_out);
			}
			else {
				hw_tty.print("\n");
			}

		}
	}
}



Memory BAFFA1_CPU::get_current_memory() {
	Memory memory;
	if (!get_byte_bit(this->alu.MSWl.value(), MSWl_PAGING_EN))
		memory = this->memory.rom_bios;

	else
		memory = this->memory.low_memory;

	return memory;
}



unsigned int BAFFA1_CPU::get_current_memory_size() {

	if (!get_byte_bit(this->alu.MSWl.value(), MSWl_PAGING_EN))
		return  this->memory.rom_bios.size;

	else
		return this->memory.rom_bios.size;
}


