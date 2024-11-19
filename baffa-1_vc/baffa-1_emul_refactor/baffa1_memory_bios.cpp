//
// baffa1_memory.cpp
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
#include "baffa1_memory_bios.h"
#include <stdlib.h>
#include <assert.h>
#include "baffa1_utils.h"


void BAFFA1_MEMORY_BIOS::init(HW_TTY &_hw_tty) {

	int i = 0;
	this->rom_bios.create_mem(BAFFA1_BIOS_MEMORY_SIZE);

	this->pagtbl_ram_l.create_mem(BAFFA1_PAGING_MEMORY_SIZE);
	this->pagtbl_ram_h.create_mem(BAFFA1_PAGING_MEMORY_SIZE);

	this->low_memory.create_mem(BAFFA1_MAIN_MEMORY_SIZE);

	this->high_memory0.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory1.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory2.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory3.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory4.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory5.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory6.create_mem(BAFFA1_MAIN_MEMORY_SIZE);
	this->high_memory7.create_mem(BAFFA1_MAIN_MEMORY_SIZE);

	this->debug_mem_offset = 0;
	this->debug_manual_offset = 0x00;

	this->reset(_hw_tty);
}

void BAFFA1_MEMORY_BIOS::reset(HW_TTY &_hw_tty)
{
	this->rom_bios.reset();
	this->pagtbl_ram_l.reset();
	this->pagtbl_ram_h.reset();
	this->low_memory.reset();

	this->high_memory0.reset();
	this->high_memory1.reset();
	this->high_memory2.reset();
	this->high_memory3.reset();
	this->high_memory4.reset();
	this->high_memory5.reset();
	this->high_memory6.reset();
	this->high_memory7.reset();

	this->load_bios(_hw_tty);

	strcpy(this->mem_bank, "BIOS");
}


int BAFFA1_MEMORY_BIOS::load_bios(HW_TTY &_hw_tty) {
	char str_out[255];
	int i;
	long size = 0;
	char * buf = loadfile(str_out, (char*)"bios.obj", &size);
	_hw_tty.print(str_out);

	if (buf == NULL)
		return 0;

	this->rom_bios.readonly = false;

	for (i = 0; i < size; i++) {
		this->rom_bios.write(i, buf[i]);
	}
	this->rom_bios.readonly = true;

	return 1;
}


void BAFFA1_MEMORY_BIOS::refresh_int_pending(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_INTERRUPTIONS_BUS& interruptions_bus, BAFFA1_BYTE MSWl_value) {
	controller_bus.int_pending = get_byte_bit(interruptions_bus.int_request, 0) & get_byte_bit(MSWl_value,MSWl_INTERRUPT_ENABLE);
}


void BAFFA1_MEMORY_BIOS::refresh_any_interruption(BAFFA1_CONTROLLER_BUS& controller_bus) {
	controller_bus.any_interruption = get_byte_bit(controller_bus.int_pending, 0) | get_byte_bit(controller_bus.dma_req, 0);
}
///////////////////

BAFFA1_BYTE BAFFA1_MEMORY_BIOS::read_memory(BAFFA1_MWORD addr, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_rd) {
	BAFFA1_BYTE memory = 0;

	BAFFA1_BYTE peripheral = get_word_bit(addr, 15); //0x8000-0xFFFF
	BAFFA1_BYTE IORQ = (addr & 0b111111110000000) == 0b111111110000000; //7F80-7FFF = IRQ - !7F80-7FFF
	BAFFA1_BYTE highMem = (addr & 0b1111110000000000000000) != 0;  //0x010000-0x3F0000 = high mem from pages

	// 0x0000-0x7FFF - Bios
	// 0x8000-0xFFFF - Main Mem (lowmem)
	BAFFA1_BYTE enable_bios = (buffer_mem_io & !IORQ & !peripheral);
	BAFFA1_BYTE enable_main_mem = (buffer_mem_io & !highMem & !IORQ & peripheral);

	if (buffer_rd == 0x00) {

		if (enable_bios) memory = this->rom_bios.read(addr);

		else if (enable_main_mem) memory = this->low_memory.read(addr);

		else if (!buffer_mem_io) {

			BAFFA1_BYTE mem_sel = get_word_bit(addr, 19) | (get_word_bit(addr, 20) << 1) | (get_word_bit(addr, 21) << 2);

			switch (mem_sel) {
			case 0:
				memory = this->high_memory0.read(addr);
				break;
			case 1:
				memory = this->high_memory1.read(addr);
				break;
			case 2:
				memory = this->high_memory2.read(addr);
				break;
			case 3:
				memory = this->high_memory3.read(addr);
				break;
			case 4:
				memory = this->high_memory4.read(addr);
				break;
			case 5:
				memory = this->high_memory5.read(addr);
				break;
			case 6:
				memory = this->high_memory6.read(addr);
				break;
			case 7:
				memory = this->high_memory7.read(addr);
				break;
			}

		}
	}

	return memory;
}

void BAFFA1_MEMORY_BIOS::write_memory(BAFFA1_MWORD addr, BAFFA1_BYTE value, BAFFA1_BYTE buffer_mem_io, BAFFA1_BYTE buffer_wr) {

	BAFFA1_BYTE peripheral = get_word_bit(addr, 15); //0x8000-0xFFFF
	BAFFA1_BYTE IORQ = (addr & 0b111111110000000) == 0b111111110000000; //7F80-7FFF = IRQ - !7F80-7FFF
	BAFFA1_BYTE highMem = (addr & 0b1111110000000000000000) != 0;  //0x010000-0x3F0000 = high mem from pages

	// 0x0000-0x7FFF - Bios
	// 0x8000-0xFFFF - Main Mem (lowmem)
	BAFFA1_BYTE enable_bios = (buffer_mem_io & !IORQ & !peripheral);
	BAFFA1_BYTE enable_main_mem = (buffer_mem_io & !highMem & !IORQ & peripheral);

	if (buffer_wr == 0x00) {

		if (enable_bios) {}

		else if (enable_main_mem)  this->low_memory.write(addr, value);

		else if (!buffer_mem_io) {

			BAFFA1_BYTE mem_sel = get_word_bit(addr, 19) | (get_word_bit(addr, 20) << 1) | (get_word_bit(addr, 21) << 2);

			switch (mem_sel) {
			case 0:
				this->high_memory0.write(addr, value);
				break;
			case 1:
				this->high_memory1.write(addr, value);
				break;
			case 2:
				this->high_memory2.write(addr, value);
				break;
			case 3:
				this->high_memory3.write(addr, value);
				break;
			case 4:
				this->high_memory4.write(addr, value);
				break;
			case 5:
				this->high_memory5.write(addr, value);
				break;
			case 6:
				this->high_memory6.write(addr, value);
				break;
			case 7:
				this->high_memory7.write(addr, value);
				break;
			}
		}
	}
}



BAFFA1_BYTE BAFFA1_MEMORY_BIOS::page_table_addr_src(BAFFA1_BYTE force_user_ptb, BAFFA1_BYTE MSWl_value) {
	return get_byte_bit(force_user_ptb, 0) | get_byte_bit(MSWl_value, MSWl_CPU_MODE);
}

void BAFFA1_MEMORY_BIOS::refresh_pagetable_mem(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_ALU_BUS& alu_bus, BAFFA1_BYTE MSWl_value, BAFFA1_BYTE MARh_value, BAFFA1_BYTE MDRl_value, BAFFA1_BYTE MDRh_value, BAFFA1_CONFIG &cpu_config) {


	if (controller_bus.ptb_wrt == 0x00) { this->PTB.set(alu_bus.z_bus); if (cpu_config.DEBUG_TRACE_WRREG) { cpu_config.io_file( (char*)"WRITE", (char*)"PTB", alu_bus.z_bus); } }

	if (controller_bus.pagtbl_ram_we && controller_bus.mdr_to_pagtbl_en)
	{
		BAFFA1_MWORD ptb_mem_addr = 0;

		if (page_table_addr_src(controller_bus.force_user_ptb, MSWl_value)) ptb_mem_addr = PTB.value();

		unsigned long pagtbl_ram_addr = 0;
		pagtbl_ram_addr = (unsigned long)((MARh_value >> 3) & 0b00011111);
		pagtbl_ram_addr |= (unsigned long)(ptb_mem_addr << 5);

		this->pagtbl_ram_l.write(pagtbl_ram_addr, MDRl_value);
		this->pagtbl_ram_h.write(pagtbl_ram_addr, MDRh_value);
	}
}

unsigned long BAFFA1_MEMORY_BIOS::read_address_bus(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_BYTE MSWl_value, BAFFA1_BYTE MARl_value, BAFFA1_BYTE MARh_value, BAFFA1_BYTE MDRl_value, BAFFA1_BYTE MDRh_value) {
	unsigned long address_bus = 0x00;

	BAFFA1_BYTE bus_tristate = controller_bus.bus_tristate(MSWl_value);

	if (bus_tristate == 0x00) {
		
		if (!get_byte_bit(MSWl_value, MSWl_PAGING_EN)) {
			address_bus = (unsigned long)((MARh_value << 8) | MARl_value);

			controller_bus.memory_io = 1;
			controller_bus.page_present = 1;
			controller_bus.page_writable = 1;
		}
		else {
			unsigned long mem_table_addr = 0;

			if (!controller_bus.mdr_to_pagtbl_en) {

				BAFFA1_MWORD ptb_mem_addr = 0; // ((MARh_value >> 3) & 0b00011111);

				if (page_table_addr_src(controller_bus.force_user_ptb, MSWl_value)) ptb_mem_addr = (unsigned long)(PTB.value());

				unsigned long pagtbl_ram_addr = 0;
				pagtbl_ram_addr = (unsigned long)((MARh_value >> 3) & 0b00011111);
				pagtbl_ram_addr |= (unsigned long)(ptb_mem_addr << 5);

				mem_table_addr = (unsigned long)this->pagtbl_ram_l.read(pagtbl_ram_addr);
				mem_table_addr |= (unsigned long)(this->pagtbl_ram_h.read(pagtbl_ram_addr) & 0b00000111) << 8;

				controller_bus.memory_io = get_byte_bit(this->pagtbl_ram_h.read(pagtbl_ram_addr), 3);
				controller_bus.page_present = get_byte_bit(this->pagtbl_ram_h.read(pagtbl_ram_addr), 4);
				controller_bus.page_writable = get_byte_bit(this->pagtbl_ram_h.read(pagtbl_ram_addr), 5);
			}
			else {
				mem_table_addr = MDRl_value;
				mem_table_addr |= (MDRh_value & 0b00000111) << 8;

				controller_bus.memory_io = get_byte_bit(MDRh_value, 3);
				controller_bus.page_present = get_byte_bit(MDRh_value, 4);
				controller_bus.page_writable = get_byte_bit(MDRh_value, 5);
			}

			address_bus = ((MARh_value & 0b111) << 8) | MARl_value;
			address_bus |= (mem_table_addr << 11);

		}
	}
	else {
		address_bus = controller_bus.panel_address;
	}
	/////MEMORIA AQUI
	// ~oe = output enabled
	// ~we = write enabled
	// ~ce = chip enabled

	return address_bus;
}


#pragma region "Debug"

void BAFFA1_MEMORY_BIOS::debugger(BAFFA1_DWORD MAR, HW_TTY &_hw_tty)
{
	char str_out[255];
	int i = 0;
	bool autoaddress = false;
	long current = 0;


	if (strcmp(this->mem_bank, "BIOS") == 0 && this->debug_mem_offset >= BAFFA1_BIOS_MEMORY_SIZE)
		autoaddress = true;
	else if ((strcmp(this->mem_bank, "LOW") == 0 ||
		strcmp(this->mem_bank, "MAIN0") == 0 ||
		strcmp(this->mem_bank, "MAIN1") == 0 ||
		strcmp(this->mem_bank, "MAIN2") == 0 ||
		strcmp(this->mem_bank, "MAIN3") == 0 ||
		strcmp(this->mem_bank, "MAIN4") == 0 ||
		strcmp(this->mem_bank, "MAIN5") == 0 ||
		strcmp(this->mem_bank, "MAIN6") == 0 ||
		strcmp(this->mem_bank, "MAIN7") == 0) && this->debug_mem_offset >= BAFFA1_MAIN_MEMORY_SIZE)
		autoaddress = true;
	else if ((strcmp(this->mem_bank, "TAB0") == 0 ||
		strcmp(this->mem_bank, "TAB0") == 0) && this->debug_mem_offset >= BAFFA1_PAGING_MEMORY_SIZE)
		autoaddress = true;

	if (autoaddress) {
		if (this->debug_manual_offset == 0x3FFFFF) {
			if (0 + this->debug_mem_offset > MAR || MAR >= 256 + this->debug_mem_offset)
				this->debug_mem_offset = (MAR / 0x10) * 0x10;
		}
	}

	this->debug_mem_offset = (this->debug_mem_offset / 0x10) * 0x10;

	BAFFA1_BYTE mem_bytes[256];
	for (i = 0; i < 256; i++) {

		if (strcmp(this->mem_bank, "BIOS") == 0)
			mem_bytes[i] = this->rom_bios.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "LOW") == 0)
			mem_bytes[i] = this->low_memory.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN0") == 0)
			mem_bytes[i] = this->high_memory0.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN1") == 0)
			mem_bytes[i] = this->high_memory1.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN2") == 0)
			mem_bytes[i] = this->high_memory2.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN3") == 0)
			mem_bytes[i] = this->high_memory3.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN4") == 0)
			mem_bytes[i] = this->high_memory4.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN5") == 0)
			mem_bytes[i] = this->high_memory5.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN6") == 0)
			mem_bytes[i] = this->high_memory6.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "MAIN7") == 0)
			mem_bytes[i] = this->high_memory7.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "TAB0") == 0)
			mem_bytes[i] = this->pagtbl_ram_l.read(i + this->debug_mem_offset);
		else if (strcmp(this->mem_bank, "TAB1") == 0)
			mem_bytes[i] = this->pagtbl_ram_h.read(i + this->debug_mem_offset);
		else {
			if (i + this->debug_mem_offset > 0x7FFF)
				mem_bytes[i] = this->low_memory.read(i + this->debug_mem_offset);
			else
				mem_bytes[i] = this->rom_bios.read(i + this->debug_mem_offset);
		}

	}

	sprintf(str_out, "\nMemory %s from %06x\n", this->mem_bank, this->debug_mem_offset); _hw_tty.print(str_out);

	this->displayMemory(mem_bytes, this->debug_mem_offset, MAR, _hw_tty);
}



void BAFFA1_MEMORY_BIOS::displayMemory(BAFFA1_BYTE* mem_bytes, long offset, long current, HW_TTY &_hw_tty)
{
	char str_out[255];
	int i = 0, j = 0;

	_hw_tty.print("\n        ");

	for (i = 0; i < 16; i++) {
		sprintf(str_out, "%02x ", i);
		_hw_tty.print(str_out);
	}

	sprintf(str_out, "\n\n %06x ", (unsigned int)offset); _hw_tty.print(str_out);

	for (i = 0; i < 256; i++) {
		if (i % 16 == 0)
			if (current == i)
				_hw_tty.print(" *");
			else
				_hw_tty.print("  ");
		if (current == i + offset || current - 1 == i + offset) {
			sprintf(str_out, "%02x*", mem_bytes[i]); _hw_tty.print(str_out);
		}
		else {
			sprintf(str_out, "%02x ", mem_bytes[i]); _hw_tty.print(str_out);
		}

		if ((i + 1) % 16 == 0 && i <= 255) {
			_hw_tty.print("  |");
			for (j = (i + 1) - 16; j < (i + 1); j++) {
				if (mem_bytes[j] < 0x20)
					_hw_tty.print(".");
				else {
					sprintf(str_out, "%c", mem_bytes[j]);
					_hw_tty.print(str_out);
				}
			}
			_hw_tty.print("|");

			if (i < 255) {
				sprintf(str_out, "\n %06x ", (unsigned int)(i + offset + 1));
				_hw_tty.print(str_out);
			}
			else
				_hw_tty.print("\n");

		}
	}
}



void BAFFA1_MEMORY_BIOS::display_test(BAFFA1_DWORD MAR, HW_TTY &_hw_tty)
{
	char str_out[255];
	unsigned int i = 0, j = 0;
	
	if (this->debug_manual_offset == 0x00) {
		if (0 + this->debug_mem_offset > MAR || MAR >= 256 + this->debug_mem_offset)
			this->debug_mem_offset = (MAR / 0x10) * 0x10;
	}

	_hw_tty.print("\n        ");

	for (i = 0; i < 16; i++) {
		sprintf(str_out, "%02x ", i);
		_hw_tty.print(str_out);
	}

	sprintf(str_out, "\n\n %04x ", this->debug_mem_offset); _hw_tty.print(str_out);

	for (i = 0 + this->debug_mem_offset; i < 256 + this->debug_mem_offset; i++) {
		if (i % 16 == 0)
			if (MAR == i)
				_hw_tty.print(" *");
			else
				_hw_tty.print("  ");
		if (MAR == i || MAR - 1 == i) {
			sprintf(str_out, "%02x*", this->low_memory.read(i));
			_hw_tty.print(str_out);
		}
		else {
			sprintf(str_out, "%02x ", this->low_memory.read(i));
			_hw_tty.print(str_out);
		}

		if ((i + 1) % 16 == 0 && i <= 255 + this->debug_mem_offset) {
			_hw_tty.print("  |");
			for (j = (i + 1) - 16; j < (i + 1); j++) {
				if (this->low_memory.read(j) < 0x20)
					_hw_tty.print(".");
				else {
					sprintf(str_out, "%c", this->low_memory.read(j));
					_hw_tty.print(str_out);
				}
			}
			_hw_tty.print("|");

			if (i < 255 + this->debug_mem_offset) {
				sprintf(str_out, "\n %04x ", i + 1);
				_hw_tty.print(str_out);
			}
			else
				_hw_tty.print("\n");

		}
	}
}
#pragma endregion
