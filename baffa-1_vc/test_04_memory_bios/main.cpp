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
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
#include "baffa1_memory_bios.h"

#include "baffa1_utils.h"

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

	cpu_config.DEBUG_REG_READ = 1;
	cpu_config.DEBUG_REGISTERS = 1;

	cpu_config.DEBUG_TRACE_RDREG = 1;
	cpu_config.DEBUG_TRACE_WRREG = 1;

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
	BAFFA1_ALU_BUS alu_bus;
	BAFFA1_INTERRUPTIONS_BUS interruptions_bus;

	BAFFA1_MEMORY_BIOS memory_bios;
	memory_bios.cpu_config = cpu_config;

	memory_bios.init();
	controller_bus.reset();
	alu_bus.reset();
	interruptions_bus.reset();





	//	while (true) {

	BAFFA1_BYTE MSWl_value = 0;

	BAFFA1_BYTE MARl_value = 0;
	BAFFA1_BYTE MARh_value = 0;

	BAFFA1_BYTE MDRl_value = 0;
	BAFFA1_BYTE MDRh_value = 0;

	BAFFA1_MWORD addr = 0;
	BAFFA1_BYTE buffer_mem_io = 1;
	BAFFA1_BYTE buffer_rd = 1;
	BAFFA1_BYTE buffer_wr = 1;
	BAFFA1_BYTE value = 0;

	unsigned long mem_addr = 0;

	// READING BIOS
	;
	controller_bus.rd = 0; buffer_rd = controller_bus.rd;
	controller_bus.wr = 0; buffer_wr = controller_bus.wr;


	memory_bios.refresh_int_pending(controller_bus, interruptions_bus, MSWl_value);
	memory_bios.refresh_any_interruption(controller_bus);

	MSWl_value = set_byte_bit(1, MSWl_CPU_MODE) | set_byte_bit(1, MSWl_PAGING_EN); //LIGA HIGH MEM
	controller_bus.ptb_wrt = 0;
	controller_bus.pagtbl_ram_we = 1;
	controller_bus.mdr_to_pagtbl_en = 1;
	
	
	int page = 0;

	alu_bus.z_bus = page; // PTB
	MARh_value = 0;
	MARl_value = 0;
	
	MDRl_value = 0;
	MDRh_value = page & 0xFF;
	memory_bios.refresh_pagetable_mem(controller_bus, alu_bus, MSWl_value, MARh_value, MDRl_value, MDRh_value);

	page = 1;
	alu_bus.z_bus = page; // PTB
	MARh_value = 0;
	MARl_value = 0;
	MDRl_value = 0;
	MDRh_value = page & 0xFF;
	memory_bios.refresh_pagetable_mem(controller_bus, alu_bus, MSWl_value, MARh_value, MDRl_value, MDRh_value);


	controller_bus.ptb_wrt = 1;
	controller_bus.pagtbl_ram_we = 0;
	controller_bus.mdr_to_pagtbl_en = 0;
	//controller_bus.mdr_to_pagtbl_en

	for (int i = 0; i < 256; i++) {
		addr = i & 0xFFFF;
		//addr = i & 0b0111111111111111; // BIOS
		//addr = i | 0b1000000000000000; // LOW MEM 
		MARl_value = addr & 0xFF;
		MARh_value = (addr >> 8) & 0xFF;

		mem_addr = memory_bios.read_address_bus(controller_bus, MSWl_value, MARl_value, MARh_value, MDRl_value, MDRh_value);
		buffer_mem_io = controller_bus.memory_io;

		value = i;
		memory_bios.write_memory(mem_addr, value, buffer_mem_io, buffer_wr);
	}

	for (int i = 0; i < 256; i++) {
		addr = i & 0xFFFF;
		//addr = i & 0b0111111111111111; // BIOS
		//addr = i | 0b1000000000000000; // LOW MEM 

		MARl_value = addr & 0xFF;
		MARh_value = (addr >> 8) & 0xFF;

		mem_addr = memory_bios.read_address_bus(controller_bus, MSWl_value, MARl_value, MARh_value, MDRl_value, MDRh_value);
		buffer_mem_io = controller_bus.memory_io;

		BAFFA1_BYTE mem = memory_bios.read_memory(mem_addr, buffer_mem_io, buffer_rd);
		char str_out[255];
		io_print("  mem_read"); sprintf(str_out, "[%04x]: ", addr); io_print(str_out); print_byte_bin(str_out, mem); io_print(str_out);
		io_print("\n");
	}


	io_print("\n");

	MSWl_value = set_byte_bit(1, MSWl_CPU_MODE) | set_byte_bit(1, MSWl_PAGING_EN); //LIGA HIGH MEM

	controller_bus.ptb_wrt = 0;
	page = 2;
	alu_bus.z_bus = page; // PTB
	MARh_value = 0;
	MARl_value = 0;
	MDRl_value = 0;
	MDRh_value = page & 0xFF;
	memory_bios.refresh_pagetable_mem(controller_bus, alu_bus, MSWl_value, MARh_value, MDRl_value, MDRh_value);
	controller_bus.ptb_wrt = 1;

	for (int i = 0; i < 256; i++) {
		addr = i & 0xFFFF;
		//addr = i & 0b0111111111111111; // BIOS
		//addr = i | 0b1000000000000000; // LOW MEM 

		MARl_value = addr & 0xFF;
		MARh_value = (addr >> 8) & 0xFF;

		mem_addr = memory_bios.read_address_bus(controller_bus, MSWl_value, MARl_value, MARh_value, MDRl_value, MDRh_value);
		buffer_mem_io = controller_bus.memory_io;

		BAFFA1_BYTE mem = memory_bios.read_memory(mem_addr, buffer_mem_io, buffer_rd);
		char str_out[255];
		io_print("  mem_read"); sprintf(str_out, "[%04x]: ", addr); io_print(str_out); print_byte_bin(str_out, mem); io_print(str_out);
		io_print("\n");
	}



	memory_bios.write_memory(addr, value, buffer_mem_io, buffer_wr);


	memory_bios.refresh_int_pending(controller_bus, interruptions_bus, MSWl_value);
	memory_bios.refresh_any_interruption(controller_bus);

	//bus_tristate = controller_bus.bus_tristate(MSWl_value);
	//mem_addr = memory_bios.read_address_bus(controller_bus, bus_tristate, MSWl_value, MARl_value, MARh_value, MDRl_value, MDRh_value);
	//memory_bios.debugger(mem_addr);

	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

	cout << "Press enter to continue or 'q' enter to quit..." << endl;
	char c = cin.get();

	//if (c == 'q') break;
//}


	return 0;
}
