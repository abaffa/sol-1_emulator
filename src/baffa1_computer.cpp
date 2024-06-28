//
// baffa1_computer.cpp
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
#include <assert.h>
#include <string.h>
#include <ctype.h>

#if defined(__linux__) || defined(__MINGW32__)
#include <time.h>
#else
#include <chrono>
using namespace std::chrono;

#endif

#include "config.h"
#include "baffa1_computer.h"
#include "baffa1_bus.h"
#include "debugmenu_main.h"

#include "utils.h"

#include <math.h>

/*
#ifdef _MSC_VER

#include <windows.h>
// Windows sleep in 100ns units
BOOLEAN nanosleep(LONGLONG ns) {
	// Declarations
	HANDLE timer;	// Timer handle
	LARGE_INTEGER li;	// Time defintion
	// Create timer
	if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;
	// Set timer properties
	li.QuadPart = -ns;
	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		CloseHandle(timer);
		return FALSE;
	}
	// Start & wait for timer
	WaitForSingleObject(timer, INFINITE);
	// Clean resources
	CloseHandle(timer);
	// Slept without problems
	return TRUE;
}
#endif
*/
/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//boot sequence: bios, boot, kernel, shell
/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

BAFFA1_BYTE data_pio[4];

BAFFA1_MWORD oldPC = -1;
BAFFA1_BYTE oldOP = -1;

BAFFA1_DWORD last_opcode = 0xFF;

FILE *fa;





//////////////////////////////////////////////////////////////////////

BAFFA1_BYTE opcode_esc = 0x00;

void BAFFA1_COMPUTER::disassembly_current_opcode() {
	BAFFA1_DWORD current_opcode = get_current_opcode();
	BAFFA1_BYTE current_cycle = get_current_opcode_cycle();

	if (last_opcode != current_opcode) {
		char temp[5];
		char line[255];

		if (current_opcode == 0xFD) {
			opcode_esc = 0x01;
		}
		else if (current_opcode != 0xFD) {

			if (opcode_esc == 0x01) {
				opcode_esc = 0x02;
				sprintf(temp, "%02xfd", current_opcode);
				current_opcode = (0x00FD << 8) | current_opcode;
			}
			else {
				sprintf(temp, "%02x", current_opcode);

				if (opcode_esc == 0x02)
					opcode_esc = 0x03;
			}

			if (opcode_esc == 0x03) {
				opcode_esc = 0x00;
			}
			else if (this->ht_opcodes.find(temp) != this->ht_opcodes.end()) {
				Tasm_Opcode tt = this->ht_opcodes[temp];
				BAFFA1_MWORD memADDR = BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh);
				unsigned long vmemADDR = this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers);

				if (memADDR == vmemADDR)
					sprintf(line, "    %04x]          %02x:%s", (unsigned int)(memADDR - (strlen(temp) / 2)), current_opcode, tt.desc.c_str());
				else
					sprintf(line, "    %04x] (%06x) %02x:%s", (unsigned int)(memADDR - (strlen(temp) / 2)), (unsigned int)vmemADDR, current_opcode, tt.desc.c_str());

				if (tt.size > 1) {

					int i = 0;
					int opcodesize = ((int)strlen(temp) / 2) - 1;
					int param_size = tt.size - opcodesize;
					sprintf(line + strlen(line), " (");

					for (i = param_size - 2; i >= 0; i--) {
						if (i != param_size - 2)
							//sprintf(line + strlen(line), " %02x", this->cpu.get_current_memory()[memADDR + i]);
							sprintf(line + strlen(line), " %02x", this->cpu.memory.read_memory(vmemADDR + i, buffer_mem_io(), buffer_rd()));
						else
							//sprintf(line + strlen(line), "%02x", this->cpu.get_current_memory()[memADDR + i]);
							sprintf(line + strlen(line), "%02x", this->cpu.memory.read_memory(vmemADDR + i, buffer_mem_io(), buffer_rd()));
					}
					sprintf(line + strlen(line), ")");
				}

				sprintf(line + strlen(line), "\n");

				char str_out[255];
				save_to_log(str_out, fa, line);
				//this->hw_tty.print(str_out);
			}
			/*
			else if (current_opcode == 0 && baffa1_registers_value(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh) == 0)
			{
				save_to_log("########################\n");
				save_to_log("# STARTING - RESET CPU #\n");
				save_to_log("########################\n");

				sprintf(, "RESTARTING ...pressione uma tecla para continuar...\n");
				getch();
			}
			*/
			last_opcode = current_opcode;
		}
	}
}

//////////////////////////////////////////////////////////////////////


BAFFA1_BYTE BAFFA1_COMPUTER::get_current_opcode() {
	return (this->cpu.microcode.u_ad_bus / 64);
}

BAFFA1_BYTE BAFFA1_COMPUTER::get_current_opcode_cycle() {
	return  (this->cpu.microcode.u_ad_bus % 64);
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//Refresh Interruptions

void BAFFA1_COMPUTER::refresh_int() {

	BAFFA1_BYTE int_req_0 = 0x00;
	BAFFA1_BYTE int_req_1 = 0x00;
	BAFFA1_BYTE int_req_2 = 0x00;
	BAFFA1_BYTE int_req_3 = 0x00;
	BAFFA1_BYTE int_req_4 = 0x00;
	BAFFA1_BYTE int_req_5 = 0x00;
	BAFFA1_BYTE int_req_6 = 0x00;
	BAFFA1_BYTE int_req_7 = 0x00;

	BAFFA1_BYTE not_clear_all_ints = get_byte_bit(~this->cpu.microcode.controller_bus.clear_all_ints, 0x00);

	BAFFA1_BYTE int_vector_0 = 0x01; // 74ls138 disable = 1
	BAFFA1_BYTE int_vector_1 = 0x01;
	BAFFA1_BYTE int_vector_2 = 0x01;
	BAFFA1_BYTE int_vector_3 = 0x01;
	BAFFA1_BYTE int_vector_4 = 0x01;
	BAFFA1_BYTE int_vector_5 = 0x01;
	BAFFA1_BYTE int_vector_6 = 0x01;
	BAFFA1_BYTE int_vector_7 = 0x01;

	BAFFA1_BYTE int_status_0 = 0x00;
	BAFFA1_BYTE int_status_1 = 0x00;
	BAFFA1_BYTE int_status_2 = 0x00;
	BAFFA1_BYTE int_status_3 = 0x00;
	BAFFA1_BYTE int_status_4 = 0x00;
	BAFFA1_BYTE int_status_5 = 0x00;
	BAFFA1_BYTE int_status_6 = 0x00;
	BAFFA1_BYTE int_status_7 = 0x00;


	if (this->cpu.microcode.controller_bus.int_ack == 0x01) {

		BAFFA1_BYTE out_vector = (get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 3) << 2) |
			(get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 2) << 1) |
			(get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 1) << 0);

		if (out_vector == 0)
			int_vector_0 = 0;
		else if (out_vector == 1)
			int_vector_1 = 0;
		else if (out_vector == 2)
			int_vector_2 = 0;
		else if (out_vector == 3)
			int_vector_3 = 0;
		else if (out_vector == 4)
			int_vector_4 = 0;
		else if (out_vector == 5)
			int_vector_5 = 0;
		else if (out_vector == 6)
			int_vector_6 = 0;
		else if (out_vector == 7)
			int_vector_7 = 0;

		//int_vector_0 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 0), 0);
		//int_vector_1 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 1), 0);
		//int_vector_2 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 2), 0);
		//int_vector_3 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 3), 0);
		//int_vector_4 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 4), 0);
		//int_vector_5 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 5), 0);
		//int_vector_6 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 6), 0);
		//int_vector_7 = get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_vector, 7), 0);
	}

	BAFFA1_BYTE int_clr_0 = (int_vector_0 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_1 = (int_vector_1 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_2 = (int_vector_2 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_3 = (int_vector_3 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_4 = (int_vector_4 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_5 = (int_vector_5 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_6 = (int_vector_6 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_7 = (int_vector_7 & not_clear_all_ints);


	if (int_clr_0 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 0))
		int_status_0 = 1;// get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 0), 0);
	else if (int_clr_0 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11111110;

	if (int_clr_1 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 1))
		int_status_1 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 1), 0);
	else if (int_clr_1 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11111101;

	if (int_clr_2 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 2))
		int_status_2 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 2), 0);
	else if (int_clr_2 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11111011;

	if (int_clr_3 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 3))
		int_status_3 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 3), 0);
	else if (int_clr_3 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11110111;

	if (int_clr_4 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 4))
		int_status_4 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 4), 0);
	else if (int_clr_4 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11101111;

	if (int_clr_5 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 5))
		int_status_5 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 5), 0);
	else if (int_clr_5 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b11011111;

	if (int_clr_6 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 6))
		int_status_6 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 6), 0);
	else if (int_clr_6 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b10111111;

	if (int_clr_7 == 0x01 && get_byte_bit(this->cpu.microcode.controller_bus.int_req, 7))
		int_status_7 = 1;//get_byte_bit(~get_byte_bit(this->cpu.microcode.controller_bus.int_status, 7), 0);
	else if (int_clr_7 == 0x00)
		this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req & 0b01111111;


	this->cpu.microcode.controller_bus.int_status = (int_status_7 << 7) | (int_status_6 << 6) | (int_status_5 << 5) | (int_status_4 << 4) | (int_status_3 << 3) | (int_status_2 << 2) | (int_status_1 << 1) | (int_status_0 << 0);

	BAFFA1_BYTE masked_status = (~(this->cpu.microcode.controller_bus.int_status & this->cpu.registers.INT_MASKS.value())) & 0b11111111;

	this->cpu.microcode.controller_bus.int_request = (masked_status != 0b11111111) ? 0x1 : 0x0;

	if (this->cpu.microcode.controller_bus.int_vector_wrt == 0x00) {

		BAFFA1_BYTE int_out = 0;

		if (get_byte_bit(masked_status, 0) == 0)
			int_out = 7;
		else if (get_byte_bit(masked_status, 1) == 0)
			int_out = 6;
		else if (get_byte_bit(masked_status, 2) == 0)
			int_out = 5;
		else if (get_byte_bit(masked_status, 3) == 0)
			int_out = 4;
		else if (get_byte_bit(masked_status, 4) == 0)
			int_out = 3;
		else if (get_byte_bit(masked_status, 5) == 0)
			int_out = 2;
		else if (get_byte_bit(masked_status, 6) == 0)
			int_out = 1;
		else if (get_byte_bit(masked_status, 7) == 0)
			int_out = 0;

		if (this->cpu.microcode.controller_bus.int_request != 0x00)
			this->cpu.microcode.controller_bus.int_vector = ((~int_out) & 0b111) << 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//Memory

BAFFA1_BYTE  BAFFA1_COMPUTER::buffer_rd() {
	// BUFFER_RD | BUFFER_RD_MEMORY -> BUS_RD
	return this->bus.bus_rd(this->cpu.alu, this->cpu.microcode.controller_bus.rd, this->cpu.microcode.controller_bus.panel_rd);
}

BAFFA1_BYTE BAFFA1_COMPUTER::buffer_wr() {
	// BUFFER_WR | BUFFER_WR_MEMORY -> BUS_WR
	return this->bus.bus_wr(this->cpu.alu, this->cpu.microcode.controller_bus.wr, this->cpu.microcode.controller_bus.panel_wr);
}

BAFFA1_BYTE BAFFA1_COMPUTER::buffer_mem_io() {
	// BUFFER_MEM_IO -> BUS_MEM_IO
	return this->bus.bus_mem_io(this->cpu.alu, this->cpu.microcode.controller_bus.memory_io, this->cpu.microcode.controller_bus.panel_mem_io);
}

////////

int BAFFA1_COMPUTER::peripheral_selection(unsigned long addr, BAFFA1_BYTE buffer_mem_io) {

	BAFFA1_BYTE peripheral = get_word_bit(addr, 15); //0x8000-0xFFFF
	BAFFA1_BYTE IORQ = (addr & 0b111111110000000) == 0b111111110000000; //7F80-7FFF = IRQ - !7F80-7FFF
	BAFFA1_BYTE highMem = (addr & 0b1111110000000000000000) != 0;  //0x010000-0x3F0000 = high mem from pages
													  
	BAFFA1_BYTE enable_peripheral = (buffer_mem_io & !highMem & IORQ & peripheral); //0xFF80-0xFFFF

	// adicionar controlle corretamente aos periféricos
	BAFFA1_BYTE peripheral_sel = enable_peripheral ? get_word_bit(addr, 4) | (get_word_bit(addr, 5) << 1) | (get_word_bit(addr, 6) << 2) : 0xFF;

	return peripheral_sel;
}

////////

void BAFFA1_COMPUTER::mem_rd(BAFFA1_BYTE peripherical_sel) {

	char str_out[255];

	if (buffer_rd() == 0x00) {
		unsigned long mem_addr = this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers);

		switch (peripherical_sel) {
		case 0://UART SERVICES INTERRUPT = FF80 to FF85

			//printf("** UART_0 ** \n");
			//this->hw_uart.get_lsr();

			if (mem_addr - 0xFF80 == 0)
				this->_hw_uart.read();
			else if (mem_addr - 0xFF80 == 5)
				this->_hw_uart.get_lsr();

			this->bus.data_bus = this->_hw_uart.data[mem_addr - 0xFF80];

			if ((this->cpu.config.DEBUG_UART) && (get_current_opcode() > 0)) {
				char log_uart[255];
				this->_hw_uart.print((char*)"READ", (mem_addr - 0xFF80), log_uart);

				save_to_log(str_out, fa, log_uart);
			}
			break;
		case 1:
			//printf("** UART_1 ** \n");
			break;
		case 2://RTC I/O bank = FFA0 to FFAF

			//printf("** RTC ** \n");
			this->bus.data_bus = this->_hw_rtc.data[mem_addr - 0xFFA0];

			if ((this->cpu.config.DEBUG_RTC) > 0) {

				char log_rtc[255];
				hw_rtc_print(&this->_hw_rtc, (char*)"READ ", (mem_addr - 0xFFA0), log_rtc);

				save_to_log(str_out, fa, log_rtc);
				//this->hw_tty.print(str_out);

			}
			break;
		case 3:
			//printf("** PIO_0 ** \n");
			//printf("READ  PIO_0: %02x|%02x|%02x\n", data_pio[0], data_pio[1], data_pio[2], data_pio[3]);
			break;
		case 4:
			//printf("** PIO_1 ** \n");
			break;
		case 5: //IDE = FFD0 to FFD7
			//printf("** IDE ** \n");
			if (mem_addr - 0xFFD0 == 0)
				hw_ide_read(&this->_hw_ide);

			this->bus.data_bus = this->_hw_ide.data[mem_addr - 0xFFD0];


			if ((this->cpu.config.DEBUG_IDE & get_current_opcode()) > 0) {
				char log_ide[255];
				hw_ide_print(&this->_hw_ide, (char*)"READ ", (mem_addr - 0xFFD0), log_ide);

				save_to_log(str_out, fa, log_ide);
				//this->hw_tty.print(str_out);

			}
			break;
		case 6: //TIMER = FFE0 - FFE3
			//printf("** TIMER ** \n");
			this->bus.data_bus = this->_hw_timer.data[mem_addr - 0xFFE0];

			if ((this->cpu.config.DEBUG_TIMER) > 0) {

				char log_timer[255];
				hw_timer_print(&this->_hw_timer, (char*)"READ ", (mem_addr - 0xFFE0), log_timer);

				save_to_log(str_out, fa, log_timer);
				//this->hw_tty.print(str_out);
			}
			break;
		case 7:
			//printf("** BIOS_CONFIG ** \n");
			break;
		default:
			//this->bus.data_bus = this->cpu.get_current_memory()[mem_addr];
			this->bus.data_bus = this->cpu.memory.read_memory(mem_addr, buffer_mem_io(), buffer_rd());

			if (this->cpu.config.DEBUG_TRACE_RDMEM && get_current_opcode() > 0) {
				char log[255];
				if (this->bus.data_bus == 0x00)
					sprintf(log, "         \t\t\t\tREAD  RAM [%04x]\t= %02x \'\\0\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x09)
					sprintf(log, "         \t\t\t\tREAD  RAM [%04x]\t= %02x \'\\t\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x0a)
					sprintf(log, "         \t\t\t\tREAD  RAM [%04x]\t= %02x \'\\r\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x0d)
					sprintf(log, "         \t\t\t\tREAD  RAM [%04x]\t= %02x \'\\n\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else
					sprintf(log, "         \t\t\t\tREAD  RAM [%04x]\t= %02x \'%c\'\n", (unsigned int)mem_addr, this->bus.data_bus, this->bus.data_bus);

				save_to_log(str_out, fa, log);
				//this->hw_tty.print(str_out);
			}
			break;
		}
	}
}

void BAFFA1_COMPUTER::mem_wr(BAFFA1_BYTE peripherical_sel) {

	char str_out[255];

	if (buffer_wr() == 0x00) {
		unsigned long mem_addr = this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers);

		switch (peripherical_sel) {
		case 0://UART SERVICES INTERRUPT = FF80 to FF85
			//printf("** UART_0 ** \n");
			//this->hw_uart.get_lsr();

			if (mem_addr - 0xFF80 == 0) {
				this->_hw_uart.send(this->bus.data_bus);


				this->_hw_tty.send(this->bus.data_bus);


				if (this->cpu.config.WEB_SERVER)
					this->_hw_web.new_char(this->bus.data_bus);

			}
			else
				this->_hw_uart.data[mem_addr - 0xFF80] = this->bus.data_bus;

			if ((this->cpu.config.DEBUG_UART & get_current_opcode()) > 0) {
				//hw_uart_write(this->cpu, uart_out, this->bus.data_bus);
				char log_uart[255];
				this->_hw_uart.print((char*)"WRITE", (mem_addr - 0xFF80), log_uart);

				save_to_log(str_out, fa, log_uart);
				//this->hw_tty.print(str_out);
			}
			break;
		case 1:
			//printf("** UART_1 ** \n");
			break;
		case 2://RTC I/O bank = FFA0 to FFAF
			//printf("** RTC ** \n");
			this->_hw_rtc.data[mem_addr - 0xFFA0] = this->bus.data_bus;


			if ((this->cpu.config.DEBUG_RTC) > 0) {
				char log_rtc[255];
				hw_rtc_print(&this->_hw_rtc, (char*)"WRITE", (mem_addr - 0xFFA0), log_rtc);

				save_to_log(str_out, fa, log_rtc);
				//this->hw_tty.print(str_out);
			}
			break;
		case 3:
			//printf("** PIO_0 ** \n");

			data_pio[mem_addr - 0xFFB0] = this->bus.data_bus;
			if (mem_addr - 0xFFB0 == 3 && this->bus.data_bus == 0x80) {

				//printf("WRITE PIO_0: %02x|%02x|%02x\n", data_pio[0], data_pio[1], data_pio[2], data_pio[3]);
			}
			break;
		case 4:
			//printf("** PIO_1 ** \n");
			break;
		case 5: //IDE = FFD0 to FFD7
			//printf("** IDE ** \n");
			this->_hw_ide.data[mem_addr - 0xFFD0] = this->bus.data_bus;

			if (this->cpu.config.DEBUG_IDE > 0 && get_current_opcode() > 0) {
				char log_ide[255];
				hw_ide_print(&this->_hw_ide, (char*)"WRITE", (mem_addr - 0xFFD0), log_ide);

				save_to_log(str_out, fa, log_ide);
				//this->hw_tty.print(str_out);
			}

			if (mem_addr - 0xFFD0 == 0) {
				hw_ide_write(&this->_hw_ide);
			}

			// SET HD NEW STATUS AFTER LOG
			if (this->_hw_ide.data[7] == 0x04) { // RESET IDE
				this->_hw_ide.data[7] = 0x0; // 0x80 ==busy// is ready again
			}

			else if (this->_hw_ide.data[7] == 0xEF) { // SET FEATURE COMMAND
				this->_hw_ide.data[7] = 0x00; // is ready again
			}

			else if (this->_hw_ide.data[7] == 0xE6) { // SLEEP
				this->_hw_ide.data[7] = 0x00;// zerar 
				//hw_ide.data[7] = 0x80; // is ready again
			}

			else if (this->_hw_ide.data[7] == 0x20) { // read sector cmd
				this->_hw_ide.data[7] = 0b00001000;
				hw_ide_reset(&this->_hw_ide);
			}
			else if (this->_hw_ide.data[7] == 0x30) { // write sector cmd
				this->_hw_ide.data[7] = 0b00001000;
				hw_ide_reset(&this->_hw_ide);
			}
			break;
		case 6: //TIMER = FFE0 - FFE3
			//printf("** TIMER ** \n");
			if (mem_addr - 0xFFE0 == 0x00)
				hw_timer_set_c0(&this->_hw_timer, this->bus.data_bus);

			else if (mem_addr - 0xFFE0 == 0x01)
				hw_timer_set_c1(&this->_hw_timer, this->bus.data_bus);

			else
				this->_hw_timer.data[mem_addr - 0xFFE0] = this->bus.data_bus;

			if ((this->cpu.config.DEBUG_TIMER) > 0) {

				char log_timer[255];
				hw_timer_print(&this->_hw_timer, (char*)"WRITE", (mem_addr - 0xFFE0), log_timer);

				save_to_log(str_out, fa, log_timer);
				//this->hw_tty.print(str_out);
			}
		case 7:
			//printf("** BIOS_CONFIG ** \n");
			break;

		default:
			if (this->cpu.config.DEBUG_TRACE_WRMEM && get_current_opcode() > 0)
			{
				char log[255];
				if (this->bus.data_bus == 0x00)
					sprintf(log, "         \t\t\t\tWRITE RAM [%04x]\t= %02x \'\\0'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x09)
					sprintf(log, "         \t\t\t\tWRITE RAM [%04x]\t= %02x \'\\t'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x0a)
					sprintf(log, "         \t\t\t\tWRITE RAM [%04x]\t= %02x \'\\r\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else if (this->bus.data_bus == 0x0d)
					sprintf(log, "         \t\t\t\tWRITE RAM [%04x]\t= %02x \'\\n\'\n", (unsigned int)mem_addr, this->bus.data_bus);
				else
					sprintf(log, "         \t\t\t\tWRITE RAM [%04x]\t= %02x \'%c\'\n", (unsigned int)mem_addr, this->bus.data_bus, this->bus.data_bus);

				save_to_log(str_out, fa, log);
				//this->hw_tty.print(str_out);
			}

			//this->cpu.get_current_memory()[mem_addr] = this->bus.data_bus;
			this->cpu.memory.write_memory(mem_addr, this->bus.data_bus,buffer_mem_io(), buffer_wr());
			break;
		}
	}
}

/*
void BAFFA1_COMPUTER::refresh_pagetable_mem() {
	if (this->cpu.microcode.controller_bus.pagtbl_ram_we == 0x01)
	{
		BAFFA1_MWORD ptb_mem_addr = ((this->cpu.registers.MARh.value() >> 3) & 0b00011111);
		if (this->cpu.microcode.page_table_addr_src(this->cpu.registers.MSWl.value()))
			ptb_mem_addr = ptb_mem_addr | this->cpu.registers.PTB.value() << 5;

		if (this->cpu.microcode.controller_bus.mdr_to_pagtbl_en) {
			this->cpu.memory.mem_page_table0.write(ptb_mem_addr, this->cpu.registers.MDRl.value());
			this->cpu.memory.mem_page_table1.write(ptb_mem_addr, this->cpu.registers.MDRh.value());
		}

		else {
			//this->cpu.memory.mem_page_table0.write(ptb_mem_addr, 0x00);
			//this->cpu.memory.mem_page_table1.write(ptb_mem_addr, 0x00);
		}
	}
}
*/

void BAFFA1_COMPUTER::refresh_mdr_enable() {
	/*
	//https://en.wikipedia.org/wiki/Memory_buffer_register
	A memory buffer register (MBR) or memory data register (MDR) is the register in a computer's CPU that stores the data
	being transferred to and from the immediate access storage. It contains a copy of the value in the memory location
	specified by the memory address register. It acts as a buffer, allowing the processor and memory units to act independently
	without being affected by minor differences in operation. A data item will be copied to the MBR ready for use at the next
	clock cycle, when it can be either used by the processor for reading or writing, or stored in main memory after being written.
	*/

	if ((this->cpu.microcode.controller_bus.mdr_out_en & 0b00000001) == 0x01) {
		switch (this->cpu.microcode.controller_bus.mdr_out_src & 0b00000001) {
		case 0x00:
			this->bus.data_bus = this->cpu.registers.MDRl.value();
			break;
		case 0x01:
			this->bus.data_bus = this->cpu.registers.MDRh.value();
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


void BAFFA1_COMPUTER::bus_update() {
	// W = X
	
	
	if (!get_byte_bit(this->cpu.microcode.controller_bus.alu_a_src, 5)){
		// return 
		// Flags, Status, Gl, Gh
		this->cpu.registers.registers_bus.w_bus = this->cpu.registers.w_bus_refresh(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode.controller_bus.panel_regsel,
			this->cpu.microcode.controller_bus.alu_a_src, this->cpu.microcode.controller_bus.display_reg_load && this->cpu.display_reg_load,
			this->cpu.microcode.controller_bus.int_vector, this->cpu.registers.INT_MASKS.value(), this->cpu.microcode.controller_bus.int_status, fa, this->cpu.config.DEBUG_TRACE_RDREG, this->_hw_tty);

		//this->cpu.alu.alu_bus.x_bus = this->bus.w_bus;
	}
	else {
		// return from any register
		// A, B, C, D
		// SP, BP, SI, DI
		// PC, MAR, MDR, TDR
		// SPP, INT_VECTOR, INT_MASKS, INT_STATUS
		//this->cpu.alu.alu_bus.x_bus = this->bus.x_bus_refresh(this->cpu.alu, this->cpu.microcode.controller_bus.alu_a_src);// , this->bus.w_bus);
	}


	////////////////////////////////////////////////////////////////////////////
	// return MDR or TDR
		this->cpu.registers.registers_bus.k_bus = this->cpu.registers.k_bus_refresh(this->cpu.microcode.controller_bus.alu_b_src);
	////////////////////////////////////////////////////////////////////////////
	
	
}

void BAFFA1_COMPUTER::alu_update() {
	this->cpu.alu.ALU_EXEC(&this->cpu.microcode.controller_bus, this->cpu.registers.registers_bus.w_bus, this->cpu.registers.registers_bus.k_bus, this->cpu.alu.u_cf, get_byte_bit(this->cpu.alu.MSWh.value(), MSWh_CF), this->cpu.config, this->_hw_tty,fa);
	
	// nao posso usar final condition sem ter atualizado o MSW
	//this->cpu.alu.refresh_reg_flags_MSWh(&this->cpu.microcode.controller_bus, this->cpu.alu.alu_bus, this->cpu.alu.u_sf);

	// nao posso usar final condition sem ter atualizado o MSW
	//this->cpu.alu.u_flags_refresh(&this->cpu.microcode.controller_bus, this->cpu.alu.MSWl.value(), this->cpu.alu.MSWh.value(), this->cpu.alu.alu_bus, this->cpu.config, this->_hw_tty);
}





///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


void BAFFA1_COMPUTER::clock_high() {

	//CYCLE
	refresh_int();	//Refresh Interruptions

	////////////////////////////////////////////////////////////////////////////
	// MEMORY / PERIPHERAL READ/WRITE
	unsigned long memADDR = this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers);
	
	BAFFA1_BYTE peripheral_sel = this->peripheral_selection(memADDR, buffer_mem_io());

	mem_rd(peripheral_sel);
	this->refresh_mdr_enable();		// MDR READ - MDR ORDER IS CRITICAL - must be before MEM WR 	
	mem_wr(peripheral_sel);

	////////////////////////////////////////////////////////////////////////////
	// ALU 
	this->bus_update();		// BUSES K, W, X, Y		
	this->alu_update();		// ALU


}

void BAFFA1_COMPUTER::clock_low() {
	////////////////////////////////////////////////////////////////////////////
	// REGISTERS
	// IR 
	// GENERAL
	//if (this->cpu.microcode.controller_bus.ir_wrt == 0x00) this->cpu.microcode.IR.set(this->bus.data_bus);
	//this->cpu.alu.refresh(&this->cpu.microcode.controller_bus, this->cpu.alu.alu_bus, this->bus.data_bus, this->cpu.alu.u_sf, this->cpu.config, fa);
	this->cpu.registers.refresh(&this->cpu.microcode.controller_bus, this->cpu.alu, this->cpu.alu.alu_bus, this->bus.data_bus, this->cpu.alu.u_sf, this->cpu.config, fa);
	this->cpu.memory.refresh_pagetable_mem(this->cpu.microcode, this->cpu.alu, this->cpu.registers);	// PAGETABLE 	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// MICROCODE
	this->cpu.memory.refresh_int_pending(this->cpu.microcode, this->cpu.alu, this->cpu.registers);
	this->cpu.memory.refresh_any_interruption(this->cpu.microcode);

	this->cpu.microcode.sequencer_start(this->cpu.microcode.controller_bus.any_interruption, this->cpu.alu.alu_bus.final_condition, this->bus);		// Sets Microcode
	this->cpu.microcode.debugger(this->cpu.config, this->_hw_tty);
	this->cpu.microcode.sequencer_end();
	///////////////////////////////////////////////////////////////////////////		
	
}

void BAFFA1_COMPUTER::debug_cycle() {
	////////////////////////////////////////////////////////////////////////////
	// DEBUG
	BAFFA1_BYTE current_opcode = get_current_opcode();
	BAFFA1_BYTE current_opcode_cycle = get_current_opcode_cycle();
	char str_out[255];

	//if ((get_current_opcode() != 0 || (get_current_opcode() == 0 && (get_current_opcode_cycle(this->cpu) < 10 | get_current_opcode_cycle(this->cpu) > 14))))
	if ((this->cpu.config.DEBUG_LOG_OPCODE) && (current_opcode > 0)) disassembly_current_opcode();

	if (!(this->cpu.microcode.rom.bkpt_opcode == 0 && this->cpu.microcode.rom.bkpt_cycle == 0) &&
		(current_opcode == this->cpu.microcode.rom.bkpt_opcode && current_opcode_cycle == this->cpu.microcode.rom.bkpt_cycle))
	{

		sprintf(str_out, " Microcode Breakpoint | Starting Opcode/Cycle:%02x%02x.\n", this->cpu.microcode.rom.bkpt_opcode, this->cpu.microcode.rom.bkpt_cycle); this->_hw_tty.print(str_out);
		this->cpu.config.DEBUG_MICROCODE = 1;
		this->cpu.config.DEBUG_REGISTERS = 1;
		this->cpu.config.DEBUG_ALU = 1;
		this->cpu.microcode.controller_bus.panel_step = 1;
		if (this->cpu.config.DEBUG_MICROCODE) {
			this->_hw_tty.print("***** MICROCODE\n");
			//sprintf(str_out, "U-ADDRESS: ");  print_word_bin(str_out + strlen(str_out), this->cpu.microcode.u_ad_bus); sprintf(str_out + strlen(str_out), "\n"); this->hw_tty.print(str_out);
			//sprintf(str_out, "OPCODE: %02x (cycle %02x)\n", current_opcode, current_opcode_cycle); this->hw_tty.print(str_out);
			//sprintf(str_out, "microcode: \n"); this->hw_tty.print(str_out);
			sprintf(str_out, "* U_FLAGS="); print_byte_bin(str_out + strlen(str_out), this->cpu.alu.U_FLAGS.value()); sprintf(str_out + strlen(str_out), "\n"); this->_hw_tty.print(str_out);
			this->cpu.microcode.rom.display_current_cycles_desc(current_opcode, current_opcode_cycle, this->_hw_tty);
			this->_hw_tty.print("\n");
		}
		debugmenu_main(this->cpu, this->_hw_tty);
	}

	if (this->cpu.config.DEBUG_BUSES) {
		this->_hw_tty.print("***** BUS\n");
		sprintf(str_out, "* u_ad_bus    : "); print_word_bin(str_out + strlen(str_out), this->cpu.microcode.u_ad_bus); sprintf(str_out + strlen(str_out), "\n"); this->_hw_tty.print(str_out);
		sprintf(str_out, "* address bus : "); print_byte_bin(str_out + strlen(str_out), (BAFFA1_BYTE)(this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers) >> 16)); sprintf(str_out + strlen(str_out), " ");  print_word_bin(str_out + strlen(str_out), this->cpu.memory.read_address_bus(this->bus.bus_tristate(this->cpu.alu), this->cpu.microcode, this->cpu.alu, this->cpu.registers)); sprintf(str_out + strlen(str_out), "\n"); this->_hw_tty.print(str_out);
		sprintf(str_out, "*  data_bus   |"); this->_hw_tty.print(str_out);
		sprintf(str_out, "    k_bus    |"); this->_hw_tty.print(str_out);
		sprintf(str_out, "    w_bus    |"); this->_hw_tty.print(str_out);
		sprintf(str_out, "    x_bus    |"); this->_hw_tty.print(str_out);
		sprintf(str_out, "    y_bus    |"); this->_hw_tty.print(str_out);
		sprintf(str_out, "    z_bus    "); this->_hw_tty.print(str_out);
		this->_hw_tty.print("\n");
		sprintf(str_out, "  "); print_byte_bin(str_out + strlen(str_out), this->bus.data_bus); this->_hw_tty.print(str_out);
		sprintf(str_out, " | "); print_byte_bin(str_out + strlen(str_out), this->cpu.registers.registers_bus.k_bus); this->_hw_tty.print(str_out);
		sprintf(str_out, " | "); print_byte_bin(str_out + strlen(str_out), this->cpu.registers.registers_bus.w_bus); this->_hw_tty.print(str_out);
		sprintf(str_out, " | "); print_byte_bin(str_out + strlen(str_out), this->cpu.alu.alu_bus.x_bus); this->_hw_tty.print(str_out);
		sprintf(str_out, " | "); print_byte_bin(str_out + strlen(str_out), this->cpu.alu.alu_bus.y_bus); this->_hw_tty.print(str_out);
		sprintf(str_out, " | "); print_byte_bin(str_out + strlen(str_out), this->cpu.alu.alu_bus.z_bus); this->_hw_tty.print(str_out);
		this->_hw_tty.print("\n");
		this->_hw_tty.print("\n");
	}

	if (this->cpu.config.DEBUG_MEMORY) {
		//this->hw_tty.print("***** MEMORY\n"); 
		this->cpu.memory.displayMainMemory(this->cpu.registers, this->_hw_tty);
	}

	if (this->cpu.config.DEBUG_REGISTERS) {
		this->_hw_tty.print("***** REGISTERS\n");
		this->cpu.display_registers(this->_hw_tty);

	}

	if (!(this->cpu.microcode.rom.bkpt_opcode == 0 && this->cpu.microcode.rom.bkpt_cycle == 0) &&
		(current_opcode == this->cpu.microcode.rom.bkpt_opcode &&
			current_opcode_cycle == this->cpu.microcode.rom.bkpt_cycle))
	{
		sprintf(str_out, " Microcode Breakpoint | Opcode/Cycle:%02x%02x Finished.\n", this->cpu.microcode.rom.bkpt_opcode, this->cpu.microcode.rom.bkpt_cycle); this->_hw_tty.print(str_out);
		debugmenu_main(this->cpu, this->_hw_tty);
	}

	if ((this->cpu.BKPT != 0xFFFF) && (BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) == this->cpu.BKPT
		|| BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh) == this->cpu.BKPT))
	{
		this->cpu.config.DEBUG_MICROCODE = 1;
		this->cpu.config.DEBUG_REGISTERS = 1;
		this->cpu.config.DEBUG_ALU = 1;
		this->cpu.microcode.controller_bus.panel_step = 1;
		if (this->cpu.config.DEBUG_MICROCODE) {
			this->_hw_tty.print("\n***** MICROCODE\n");
			//sprintf(str_out, "U-ADDRESS: ");  print_word_bin(str_out + strlen(str_out), this->cpu.microcode.u_ad_bus); sprintf(str_out + strlen(str_out), "\n"); this->hw_tty.print(str_out);
			//sprintf(str_out, "OPCODE: %02x (cycle %02x)\n", current_opcode, current_opcode_cycle); this->hw_tty.print(str_out);
			//sprintf(str_out, "microcode: \n"); this->hw_tty.print(str_out);
			sprintf(str_out, "* U_FLAGS="); print_byte_bin(str_out + strlen(str_out), this->cpu.alu.U_FLAGS.value()); sprintf(str_out + strlen(str_out), "\n"); this->_hw_tty.print(str_out);
			this->cpu.microcode.rom.display_current_cycles_desc(current_opcode, current_opcode_cycle, this->_hw_tty);
			this->_hw_tty.print("\n");
		}

		sprintf(str_out, " Memory Breakpoint | Starting Address:%04x.\n", this->cpu.BKPT); this->_hw_tty.print(str_out);
		debugmenu_main(this->cpu, this->_hw_tty);
	}
}

void BAFFA1_COMPUTER::clock_cycle(long *runtime_counter) {

	this->cpu.microcode.controller_bus.clk = this->cpu.microcode.controller_bus.clk == 0 ? 1 : 0;

	//char str_out[255];
	//this->hw_tty.print("#################################################################################\n");
	//sprintf(str_out,"# Runtime Counter: %d\n", (*runtime_counter)++); this->hw_tty.print(str_out)
	//this->hw_tty.print("#################################################################################\n");

	if (this->cpu.microcode.controller_bus.clk == 1) {
		BAFFA1_COMPUTER::clock_high();
	}
	else {
		BAFFA1_COMPUTER::clock_low();
	}


	//CLOCK HIGH
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//this->cpu.microcode.controller_bus.clk = 0x01;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


void BAFFA1_COMPUTER::RunCPU(long *runtime_counter) {
	/*
#if defined(__linux__) || defined(__MINGW32__)
	struct timespec tstart = { 0,0 }, tend = { 0,0 };

#elif _MSC_VER
	auto tstart = high_resolution_clock::now();
	auto tend = high_resolution_clock::now();
#endif

double deltaTime = 0;
	double cpu_clk = 0;

	long ticks = 0;
	*/

	char str_out[255];

	while (1) {

		/*
#if defined(__linux__) || defined(__MINGW32__)
		clock_gettime(CLOCK_MONOTONIC, &tend);
		deltaTime = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
		clock_gettime(CLOCK_MONOTONIC, &tstart);

#elif _MSC_VER
		tend = high_resolution_clock::now();
		deltaTime = duration_cast<nanoseconds>(tend - tstart).count()*1e-9;
		tstart = high_resolution_clock::now();
#endif
		cpu_clk += deltaTime;
		*/
		//clock_cycle(runtime_counter);
		clock_low();
		clock_high();


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		BAFFA1_BYTE current_opcode = get_current_opcode();
		BAFFA1_BYTE current_opcode_cycle = get_current_opcode_cycle();

		//this->cpu.microcode.refresh_MUX(this->cpu.registers.MSWl.value());
		if (this->cpu.microcode.mux == 0x02) {
			if (!this->cpu.config.SERVER) {

#if defined(_MSC_VER) || defined(__MINGW32__)     
				if (_kbhit()) {
#else
				if (kbhit()) {
#endif
					char dddd = this->_hw_tty.get_char();


					if (dddd == 0x04) {
						this->_hw_tty.debug_call = 1;
					}
					else {
						this->_hw_uart.receive(dddd);
						//this->cpu.microcode.controller_bus.int_request = 0x01;
					}
				}
			}
		}


		if (this->_hw_tty.debug_call == 1) {
			this->_hw_tty.debug_call = 0;
			this->_hw_tty.send('\r');
			this->_hw_tty.send('\n');
			return;
		}


		//if (this->hw_uart.data[5] == 0xFF || this->hw_uart.data[5] == 0x20)
		//	if (!this->hw_uart.uart_in.empty())


		////////////////////////////////////////////////////////////////////////////
		/*
		unsigned long addr = read_address_bus();

		BAFFA1_BYTE BUFFER_MEM_IO = this->buffer_mem_io();

		BAFFA1_BYTE not_IO_ADDRESSING = get_byte_bit(~(get_word_bit(addr, 7) & get_word_bit(addr, 8) & get_word_bit(addr, 9) & get_word_bit(addr, 10) & get_word_bit(addr, 11)
			& get_word_bit(addr, 12) & get_word_bit(addr, 13) & get_word_bit(addr, 14)), 0);

		BAFFA1_BYTE not_IO_ADDRESSING2 = get_byte_bit(
			get_word_bit(~(get_word_bit(addr, 16) & get_word_bit(addr, 17) & get_word_bit(addr, 18)), 0)
			&  get_word_bit(~(get_word_bit(addr, 19) & get_word_bit(addr, 20) & get_word_bit(addr, 21)), 0)
			, 0);

		BAFFA1_BYTE peripherical_sel = 0xFF;

		if ((get_word_bit(addr, 15) == 0x01 && not_IO_ADDRESSING == 0x00 && not_IO_ADDRESSING2 == 0x01) && BUFFER_MEM_IO == 0x01) {
			// adicionar controlle corretamente aos periféricos
			peripherical_sel = get_word_bit(addr, 4) | (get_word_bit(addr, 5) << 1) | (get_word_bit(addr, 6) << 2);
		}
		*/

		if (buffer_wr() == 0x00) {

			if (this->_hw_uart.write()) {
				if (!this->_hw_uart.uart_out.empty()) {
					this->cpu.microcode.controller_bus.int_req = this->cpu.microcode.controller_bus.int_req | 0b10000000;
				}
			}
			this->_hw_uart.get_lsr();
		}

		hw_timer_tick_c0(&this->_hw_timer);

		//this->bus.reset();

		////////////////////////////////////////////////////////////////////////////
		BAFFA1_COMPUTER::debug_cycle();


		if ((this->cpu.config.DEBUG_LITE && (current_opcode != oldOP || BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh) != oldPC)) || this->cpu.config.DEBUG_LITE_CYCLES) {

			BAFFA1_BYTE cur_opcode = current_opcode;
			BAFFA1_BYTE cur_cycle = current_opcode_cycle;
			//BAFFA1_BYTE pc_opcode = this->cpu.get_current_memory()[BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh)];
			BAFFA1_BYTE pc_opcode = this->cpu.memory.read_memory(BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh), buffer_mem_io(), buffer_rd());

			if (!(cur_opcode == 0x00 && cur_cycle == 0x10)) {
				char str[256];
				char log_reg_lite[256];

				char temp[4];


				sprintf(temp, "%02x", current_opcode);

				BAFFA1_MWORD memADDR = BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh);

				sprintf(str, " Opcode=%02x:%s", cur_opcode, &this->cpu.microcode.rom.rom_desc[0x400000 + (cur_opcode * 256)]);

				sprintf(log_reg_lite, "%04x] %s", (int)(memADDR - (strlen(temp) / 2)), rightpad(str, 40)); //this->hw_tty.print(str_out);

				sprintf(log_reg_lite + strlen(log_reg_lite), " | Cycle: %02x", cur_cycle); //this->hw_tty.print(str_out);
				sprintf(log_reg_lite + strlen(log_reg_lite), " | PC: %04x", BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh)); //this->hw_tty.print(str_out);

				/*
				sprintf(log_reg_lite + strlen(log_reg_lite), " | @PC=%02x:%s\n", pc_opcode, &this->cpu.rom.rom_desc[0x400000 + (pc_opcode * 256)]);//this->hw_tty.print(str_out);
				*/
				sprintf(log_reg_lite + strlen(log_reg_lite), " | MEM: %02x%02x%02x%02x\n",
					//this->cpu.get_current_memory()[BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh)],
					//this->cpu.get_current_memory()[BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 1],
					//this->cpu.get_current_memory()[BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 2],
					//this->cpu.get_current_memory()[BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 3]
					this->cpu.memory.read_memory(BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh), buffer_mem_io(), buffer_rd()),
					this->cpu.memory.read_memory(BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 1, buffer_mem_io(), buffer_rd()),
					this->cpu.memory.read_memory(BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 2, buffer_mem_io(), buffer_rd()),
					this->cpu.memory.read_memory(BAFFA1_REGISTERS::value(this->cpu.registers.MARl, this->cpu.registers.MARh) + 3, buffer_mem_io(), buffer_rd())
				); //this->hw_tty.print(str_out);

				save_to_log(str_out, fa, log_reg_lite);
				//this->hw_tty.print("\n");

			}
			oldOP = cur_opcode;
			oldPC = BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh);
		}

		//this->cpu.microcode.refresh_MUX(this->cpu.registers.MSWl.value());		

		if (this->cpu.microcode.controller_bus.panel_step == 1 && this->cpu.microcode.mux == 0x02) {
			this->_hw_tty.print("###########################################\n");
			sprintf(str_out, "## End OpStep on Opcode/Cycle:%02x%02x. #######\n", current_opcode, current_opcode_cycle); this->_hw_tty.print(str_out);
			this->_hw_tty.print("###########################################\n");
			return;
		}
		else if (this->cpu.microcode.controller_bus.panel_microcodestep == 1 || (this->cpu.microcode.controller_bus.panel_microcodestep == 0 && this->cpu.microcode.controller_bus.panel_run == 0)) {
			this->_hw_tty.print("###########################################\n");
			sprintf(str_out, "## End MicroStep on Opcode/Cycle:%02x%02x. ####\n", current_opcode, current_opcode_cycle); this->_hw_tty.print(str_out);
			this->_hw_tty.print("###########################################\n");
			return;
		}
		else if (this->cpu.microcode.controller_bus.reset == 1 || this->cpu.microcode.controller_bus.restart == 1) {

			return;
		}
		/*
		if (microcodestep == 0 && step == 0) {

			ticks++;

			if (cpu_clk > 1) {
				//printf("%d\n", ticks);
				ticks = 0;
				cpu_clk = 0;
			}


			if (ticks > 3800000) { // limiting 3.8 mhz
#ifdef _MSC_VER

				nanosleep(1);
#else
				struct timespec ts;
				ts.tv_sec = 0;
				ts.tv_nsec = 1;
				nanosleep(&ts, NULL);

#endif

			}
		}
		*/
	}

	//this->cpu.microcode.controller_bus.next = 0x00;
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


int BAFFA1_COMPUTER::init() {

	//char str_out[255];
	char buff[255];
	//	char buff2[255];
	FILE *fp;

	fp = fopen("tasm1.tab", "r");

	if (fp == NULL)
	{
		this->_hw_tty.print("Could not found tasm1.tab file!");
		exit(1);
	}

	while (fscanf(fp, "%255[^\n]%*c\n", buff) != EOF) {


		char _desc[50];
		char _opcode[50];
		char __size[50];
		int _size = 0;

		int col = 0;


		char *token = strtok(buff, "*\t");
		while (token != NULL) {

			if (col == 0)
				sprintf(_desc, "%s", token);

			else if (col == 1)
				sprintf(_opcode, "%s", token);

			else if (col == 2) {
				sprintf(__size, "%s", token);
				_size = atoi(__size);
			}
			col++;
			token = strtok(NULL, "*\t");
		}

		if (_size >= 1) {
			this->ht_opcodes[strlower(_opcode)] = Tasm_Opcode(_opcode, _desc, _size);
		}
	}

	fclose(fp);

	this->cpu.init(this->_hw_tty);

	fa = fopen("debug_trace.log", "w");
	if (fa == NULL) {
		this->_hw_tty.print("can not open debug file\n");
		exit(1);
	}

	this->_hw_uart.init(&this->cpu);
	hw_rtc_init(&this->_hw_rtc);
	hw_rtc_start_clock(&this->_hw_rtc);
	hw_timer_init(&this->_hw_timer);
	hw_ide_init(&this->_hw_ide);
	//this->hw_tty.init(this->cpu, &this->hw_uart);

	this->cpu.memory.load_bios();

	hw_ide_load_disk(this->_hw_ide.memory);

	//init bus
	this->bus.init();

	if (this->cpu.config.SERVER) {
		this->_hw_tty.start_server(&this->_hw_uart);
	}

	if (this->cpu.config.WEB_SERVER) {
		this->_hw_web.start_server(this->cpu, &this->_hw_uart);
	}


	return 1;
}







void BAFFA1_COMPUTER::trace_menu() {
	this->_hw_tty.print("SOL-1 Debug Monitor > Trace\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  S - Opcode Step\n");
	this->_hw_tty.print("  M - Microcode Step\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  I - Back one microcode step\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  R - Display Registers\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  O - Display Memory\n");
	this->_hw_tty.print("  P - Reset Memory and Disk\n");
	this->_hw_tty.print("  B - Load BKP Disk and Reset\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  G - Go(Run)\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  ? - Display Menu\n");
	this->_hw_tty.print("  Q - Back to Debug Monitor\n");
	this->_hw_tty.print("\n");
	this->_hw_tty.print("  ... or a sequence of opcodes to load into memory");
	this->_hw_tty.print("\n");
}

void BAFFA1_COMPUTER::run() {

	char str_out[255];

	long runtime_counter = 0;

	//cpu_print(&z80);

	int debug = 0;

	int start = 1;
	while (1) {


		if (PANEL_ENABLED == 0) {
			char *input = (char*)malloc(sizeof(char) * 257);

			//baffa1_cpu_memory_display(&baffa1_cpu);
			//baffa1_cpu_display_registers_lite(&baffa1_cpu);
			//this->hw_tty.print("Flags: "); mswh_flags_desc(&baffa1_cpu); this->hw_tty.print("\n");
			//this->hw_tty.print("Status: "); mswl_status_desc(&baffa1_cpu); this->hw_tty.print("\n");

			if (start == 1) {

				input[0] = 'q';
				input[1] = '\n';
				start = 0;
			}
			else {
				this->_hw_tty.print("TRACE> ");
				input = this->_hw_tty.getline();
			}

			if (input[0] != 'q' && input[0] != 'Q' &&
				input[0] != 'm' && input[0] != 'M' &&
				input[0] != 'r' && input[0] != 'R' &&
				input[0] != 'p' && input[0] != 'P' &&
				input[0] != 's' && input[0] != 'S' &&
				input[0] != 'o' && input[0] != 'O' &&
				input[0] != 'i' && input[0] != 'I' &&
				input[0] != 'g' && input[0] != 'G' &&
				input[0] != 'b' && input[0] != 'B' &&
				input[0] != '?') {
				size_t numdigits = strlen(input) / 2;
				size_t i;

				int begin = BAFFA1_REGISTERS::value(this->cpu.registers.PCl, this->cpu.registers.PCh);
				for (i = 0; i != numdigits; ++i)
				{
					unsigned char output = 16 * toInt(input[2 * i]) + toInt(input[2 * i + 1]);
					this->cpu.memory.rom_bios.write((long)(begin + i), output);
					//sprintf("%x\n", output[i]);
				}

				debug = 1;
			}
			else if (input[0] == 'q' || input[0] == 'Q') {


				debugmenu_main_menu(this->cpu, this->_hw_tty);
				int debug_status = debugmenu_main(this->cpu, this->_hw_tty);
				if (debug_status == 1) { //SAME as R next IF
					this->cpu.config.DEBUG_MICROCODE = 0;
					this->cpu.config.DEBUG_REGISTERS = 0;
					this->cpu.config.DEBUG_ALU = 0;
					///
					this->cpu.microcode.controller_bus.panel_run = 1;
					this->cpu.microcode.controller_bus.panel_step = 0;
					this->cpu.microcode.controller_bus.panel_microcodestep = 0;
					this->_hw_tty.print("\n\n");
					this->_hw_tty.print("###########################################\n");
					this->_hw_tty.print("## Running Instructions ###################\n");
					this->_hw_tty.print("###########################################\n");
				}
				else if (debug_status == 2) { //SAME as S next IF

					this->cpu.config.DEBUG_MICROCODE = 1;
					this->cpu.config.DEBUG_REGISTERS = 1;
					this->cpu.config.DEBUG_ALU = 1;

					debug = 1;
					this->cpu.microcode.controller_bus.panel_run = 0;
					this->cpu.microcode.controller_bus.panel_step = 0;
					this->cpu.microcode.controller_bus.panel_microcodestep = 0;
					this->_hw_tty.print("\n");

				}
				else if (debug_status == 0) {
					return;
				}
			}
			else if (input[0] == 'g' || input[0] == 'G') {

				///
				this->cpu.microcode.controller_bus.panel_run = 1;
				this->cpu.microcode.controller_bus.panel_step = 0;
				this->cpu.microcode.controller_bus.panel_microcodestep = 0;
				this->_hw_tty.print("\n\n");
				this->_hw_tty.print("###########################################\n");
				this->_hw_tty.print("## Running Instructions ###################\n");
				this->_hw_tty.print("###########################################\n");
			}
			else if (input[0] == 's' || input[0] == 'S') {

				this->cpu.config.DEBUG_MICROCODE = 1;
				this->cpu.config.DEBUG_REGISTERS = 1;
				this->cpu.config.DEBUG_ALU = 1;

				this->cpu.microcode.controller_bus.panel_run = 0;
				this->cpu.microcode.controller_bus.panel_step = 1;
				this->cpu.microcode.controller_bus.panel_microcodestep = 0;
				this->_hw_tty.print("\n\n");
				this->_hw_tty.print("###########################################\n");
				this->_hw_tty.print("## OpCode Step ############################\n");
				this->_hw_tty.print("###########################################\n");
			}
			else if (input[0] == 'm' || input[0] == 'M') {
				this->cpu.microcode.controller_bus.panel_run = 0;
				this->cpu.microcode.controller_bus.panel_step = 0;
				this->cpu.microcode.controller_bus.panel_microcodestep = 1;
				this->_hw_tty.print("\n\n");
				this->_hw_tty.print("###########################################\n");
				this->_hw_tty.print("# Microcode Step ##########################\n");
				this->_hw_tty.print("###########################################\n");
				this->_hw_tty.print("***** REGISTERS\n");
				this->cpu.display_registers_lite(this->_hw_tty);
				sprintf(str_out, "* IR: %02x\n", this->cpu.microcode.IR.value()); this->_hw_tty.print(str_out);
				this->_hw_tty.print("\n");
			}
			else if (input[0] == 'i' || input[0] == 'I') {
				this->cpu.microcode.controller_bus.panel_run = 0;
				this->cpu.microcode.controller_bus.panel_step = 1;
				this->cpu.microcode.controller_bus.panel_microcodestep = 0;

				this->cpu.microcode.u_adder = this->cpu.microcode.old_u_ad_bus;
			}
			else if (input[0] == 'p' || input[0] == 'P') {

				this->_hw_tty.print("***** Reseting CPU\n");

				debugmenu_main_reset_cpu(this->cpu, this->_hw_tty);

				hw_timer_init(&this->_hw_timer);
				hw_ide_init(&this->_hw_ide);

				this->cpu.memory.reset();
				this->cpu.memory.load_bios();

				hw_ide_load_disk(this->_hw_ide.memory);

				debugmenu_main_reset_cpu(this->cpu, this->_hw_tty);

				//init bus
				this->bus.init();

				debug = 1;
				this->_hw_tty.print("***** Disk Loaded, CPU Reseted\n");
			}

			else if (input[0] == 'b' || input[0] == 'B') {

				this->_hw_tty.print("***** Loading BKP and Reseting CPU\n");

				debugmenu_main_reset_cpu(this->cpu, this->_hw_tty);

				hw_timer_init(&this->_hw_timer);
				hw_ide_init(&this->_hw_ide);

				this->cpu.memory.reset();
				this->cpu.memory.load_bios();

				hw_ide_load_bkpdisk(this->_hw_ide.memory);

				debugmenu_main_reset_cpu(this->cpu, this->_hw_tty);

				//init bus
				this->bus.init();

				debug = 1;
				this->_hw_tty.print("***** BKP Disk Loaded, CPU Reseted\n");
			}

			else if (input[0] == 'o' || input[0] == 'O') {
				this->cpu.memory.displayMainMemory(this->cpu.registers, this->_hw_tty);
				debug = 1;
			}
			else if (input[0] == 'r' || input[0] == 'R') {
				debugmenu_main_display_registers(this->cpu, this->_hw_tty);
				debug = 1;
			}
			else if (input[0] == '?') {
				trace_menu();
				debug = 1;
			}

			free(input);

			if ((debug == 0 && PANEL_ENABLED == 0) || PANEL_ENABLED == 1)
				RunCPU(&runtime_counter);
			else
				debug = 0;

			oldPC = -1;
			oldOP = -1;

			if (this->cpu.microcode.controller_bus.panel_run == 0 && this->cpu.microcode.controller_bus.panel_step == 0 && this->cpu.microcode.controller_bus.panel_microcodestep == 0)
				this->cpu.microcode.u_adder = 0x10;

		}
		else {

			if (this->cpu.microcode.controller_bus.reset == 1)
			{
				//debugmenu_main_reset_cpu(this->cpu, this->hw_tty);
				//this->cpu.memory.reset();

				//this->cpu.microcode.controller_bus.panel_run = 0;
				//this->cpu.microcode.controller_bus.panel_step = 0;
				//this->cpu.microcode.controller_bus.panel_microcodestep = 0;
				//this->cpu.microcode.controller_bus.reset = 0;
			}
			else if (this->cpu.microcode.controller_bus.restart == 1) {

				debugmenu_main_reset_cpu(this->cpu, this->_hw_tty);
				this->cpu.memory.reset();
				this->cpu.memory.load_bios();
				hw_ide_load_disk(this->_hw_ide.memory);


				//this->cpu.microcode.controller_bus.panel_run = 0;
				//this->cpu.microcode.controller_bus.panel_step = 0;
				//this->cpu.microcode.controller_bus.panel_microcodestep = 0;

				this->cpu.microcode.controller_bus.restart = 0;
			}

			if (this->cpu.microcode.controller_bus.panel_run == 1 || this->cpu.microcode.controller_bus.panel_step == 1 || this->cpu.microcode.controller_bus.panel_microcodestep == 1) {
				RunCPU(&runtime_counter);

				if (this->cpu.microcode.controller_bus.panel_step == 1)
					this->cpu.microcode.controller_bus.panel_step = 0;
				else if (this->cpu.microcode.controller_bus.panel_microcodestep == 1)
					this->cpu.microcode.controller_bus.panel_microcodestep = 0;
			}
		}
	}
}