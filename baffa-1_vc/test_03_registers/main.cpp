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
#include "baffa1_data_bus.h"
#include "baffa1_controller_bus.h"
#include "baffa1_alu_bus.h"
#include "baffa1_registers.h"

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


	BAFFA1_DATA_BUS data_bus;
	BAFFA1_CONTROLLER_BUS controller_bus;
	BAFFA1_ALU_BUS alu_bus;

	BAFFA1_REGISTERS registers;
	registers.cpu_config = cpu_config;

	registers.init();
	controller_bus.reset();
	data_bus.reset();
	alu_bus.reset();


	while (true) {


		BAFFA1_BYTE bus_tristate = 0;
		BAFFA1_BYTE panel_regsel = 0;
		BAFFA1_BYTE display_reg_load = 0;
		BAFFA1_BYTE int_vector = 0;
		BAFFA1_BYTE int_masks = 0;
		BAFFA1_BYTE int_status = 0;

		BAFFA1_BYTE w_bus = registers.w_bus_refresh(bus_tristate, panel_regsel, controller_bus.alu_a_src, display_reg_load, int_vector, int_masks, int_status);
		BAFFA1_BYTE k_bus = registers.k_bus_refresh(controller_bus.alu_b_src);


		if (cpu_config.DEBUG_BUSES) {
			char str_out[255];
			io_print("+------------------------------------------------------------------------------\n");
			io_print("+ W BUS\n\n");

			sprintf(str_out, "  bus_tristate=%02x", bus_tristate); io_print(str_out);
			sprintf(str_out, " | panel_regsel=%02x", panel_regsel); io_print(str_out);
			sprintf(str_out, " | display_reg_load=%02x", display_reg_load); io_print(str_out);
			io_print("\n");

			io_print("  int_vector: "); print_byte_bin(str_out, int_vector); io_print(str_out);
			io_print(" | int_masks: "); print_byte_bin(str_out, int_masks); io_print(str_out);
			io_print(" | int_status: "); print_byte_bin(str_out, int_status); io_print(str_out);
			io_print("\n");

			io_print("  alu_a_src: "); print_byte_bin(str_out, controller_bus.alu_a_src); io_print(str_out);
			io_print(" | DATA: "); print_byte_bin(str_out, w_bus); io_print(str_out);
			io_print("\n");
			io_print("\n");
		}

		if (cpu_config.DEBUG_BUSES) {
			char str_out[255];
			io_print("+------------------------------------------------------------------------------\n");
			io_print("+ K BUS\n\n");

			io_print("  alu_b_src: "); print_byte_bin(str_out, controller_bus.alu_b_src); io_print(str_out);
			io_print(" | DATA: "); print_byte_bin(str_out, w_bus); io_print(str_out);
			io_print("\n");
			io_print("\n");
		}


		BAFFA1_BYTE MSWl_value = 0;
		BAFFA1_BYTE u_sf = 0;

		registers.process(data_bus, controller_bus, alu_bus, MSWl_value, u_sf);
		registers.debugger();

		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

		cout << "Press enter to continue or 'q' enter to quit..." << endl;
		char c = cin.get();

		if (c == 'q') break;
	}


	return 0;
}
