//
// baffa1_computer.h
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
#ifndef BAFFA1COMPUTER_H
#define BAFFA1COMPUTER_H
#include "config.h"
#include "baffa1_bus.h"
#include "baffa1_cpu.h"

#include "hw_uart.h"
#include "hw_rtc.h"
#include "hw_timer.h"
#include "hw_ide.h"
#include "hw_tty.h"
#include "hw_web.h"
#include "tasm_opcode.h"

#include <unordered_map>
using namespace std;


class BAFFA1_COMPUTER {

private:
	void trace_menu();
	void clock_high();
	void clock_low();
	void debug_cycle();
	
public:
	BAFFA1_CPU cpu;
	BAFFA1_BUS bus;

	struct hw_rtc _hw_rtc;
	hw_uart _hw_uart;
	struct hw_ide _hw_ide;
	struct hw_timer _hw_timer;

	HW_TTY _hw_tty;
	HW_WEB _hw_web;

	unordered_map<string, Tasm_Opcode> ht_opcodes;


	//BAFFA1_BYTE apagar = 0;
	//int gambi_ide = 0;
	//int gambi_ide_total = 0;
	//int gambi_ide_read = 0;
	//int gambi_last_op = 0;

	
	int init();

	void run();
	   

	BAFFA1_BYTE get_current_opcode();
	BAFFA1_BYTE get_current_opcode_cycle();
	void disassembly_current_opcode();
	void refresh_int();

	BAFFA1_BYTE buffer_rd();
	BAFFA1_BYTE buffer_wr();
	BAFFA1_BYTE buffer_mem_io();

	void mem_rd(BAFFA1_BYTE peripherical_sel);
	void mem_wr(BAFFA1_BYTE peripherical_sel);

	void clock_cycle(long *runtime_counter);
	void RunCPU(long *runtime_counter);

	int peripheral_selection(unsigned long addr, BAFFA1_BYTE buffer_mem_io);
	void bus_update();
	void alu_update();
	//void refresh_pagetable_mem();
	void refresh_mdr_enable();
};



#endif