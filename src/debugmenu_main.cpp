//
// debugmenu_main.cpp
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

#if defined(__linux__) || defined(__MINGW32__)

#else

#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "debugmenu_main.h"
#include "debugmenu_roms.h"
#include "utils.h"
#include "hw_tty.h"

void debugmenu_main_menu(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {
	
	char str_out[255]; 
	hw_tty.print("\n");
	hw_tty.print("SOL-1 Debug Monitor | 30 May 2021\n");
	hw_tty.print("\n");
	
	sprintf(str_out, "  S - Current Memory Bank [%s]\n", baffa1_cpu.memory.mem_bank); hw_tty.print(str_out);

	sprintf(str_out, "  N - Memory Offset: (%06x)\n", baffa1_cpu.memory.debug_mem_offset); hw_tty.print(str_out);

	hw_tty.print("  D - Display Memory\n");
	hw_tty.print("  I - Disassemble Memory\n");
	hw_tty.print("  E - Edit Memory\n");
	hw_tty.print("  F - Fill Memory\n");
	hw_tty.print("  L - Load Memory\n");
	hw_tty.print("\n");
	hw_tty.print("  R - Display Registers\n");
	hw_tty.print("  A - Edit Register\n");
	hw_tty.print("  B - Edit Breakpoint\n");
	hw_tty.print("  P - Edit Program Counter\n");
	hw_tty.print("\n");
	hw_tty.print("  G - Go(Run)\n");
	hw_tty.print("  T - Debug Trace\n");
	hw_tty.print("  Z - Reset CPU\n");
	hw_tty.print("  V - Reload Bios\n");
	hw_tty.print("\n");
	hw_tty.print("  X - Debug Roms\n");
	hw_tty.print("\n");
	hw_tty.print("  ? - Display Menu\n");
	hw_tty.print("  Q - Quit\n");
	hw_tty.print("\n");
}


void debugmenu_main_disassemble_mem(HW_TTY& hw_tty) {
}


void debugmenu_main_set_mem_bank(BAFFA1_MEMORY& memory, HW_TTY& hw_tty) {

	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);
	
	do {
		sprintf(str_out, "Set Memory Bank [%s] (BIOS, LOW, MAIN[0-7], TAB[0-1]) ? ", memory.mem_bank); hw_tty.print(str_out);
		input = hw_tty.gets(6);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		hw_tty.print("\n");
	} while (strcmp(input, "BIOS") &&
		strcmp(input, "LOW") &&
		strcmp(input, "MAIN0") &&
		strcmp(input, "MAIN1") &&
		strcmp(input, "MAIN2") &&
		strcmp(input, "MAIN3") &&
		strcmp(input, "MAIN4") &&
		strcmp(input, "MAIN5") &&
		strcmp(input, "MAIN6") &&
		strcmp(input, "MAIN7") &&
		strcmp(input, "TAB0") &&
		strcmp(input, "TAB1"));

	strcpy(memory.mem_bank, input);

	if (strcmp(memory.mem_bank, "BIOS") == 0 && memory.debug_mem_offset >= BAFFA1_BIOS_MEMORY_SIZE)
		memory.debug_mem_offset = 0;
	else if ((strcmp(memory.mem_bank, "LOW") == 0 ||
		strcmp(memory.mem_bank, "MAIN0") == 0 ||
		strcmp(memory.mem_bank, "MAIN1") == 0 ||
		strcmp(memory.mem_bank, "MAIN2") == 0 ||
		strcmp(memory.mem_bank, "MAIN3") == 0 ||
		strcmp(memory.mem_bank, "MAIN4") == 0 ||
		strcmp(memory.mem_bank, "MAIN5") == 0 ||
		strcmp(memory.mem_bank, "MAIN6") == 0 ||
		strcmp(memory.mem_bank, "MAIN7") == 0) && memory.debug_mem_offset >= BAFFA1_MAIN_MEMORY_SIZE)
		memory.debug_mem_offset = 0;
	else if ((strcmp(memory.mem_bank, "TAB0") == 0 ||
		strcmp(memory.mem_bank, "TAB0") == 0) && memory.debug_mem_offset >= BAFFA1_PAGING_MEMORY_SIZE)
		memory.debug_mem_offset = 0;		
}

void debugmenu_main_set_mem_offset(BAFFA1_MEMORY& memory, HW_TTY& hw_tty) {

	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);

	long maxmem = 0;

	if (strcmp(memory.mem_bank, "BIOS") == 0)
		maxmem = BAFFA1_BIOS_MEMORY_SIZE - 1;

	else if ((strcmp(memory.mem_bank, "LOW") == 0 ||
		strcmp(memory.mem_bank, "MAIN0") == 0 ||
		strcmp(memory.mem_bank, "MAIN1") == 0 ||
		strcmp(memory.mem_bank, "MAIN2") == 0 ||
		strcmp(memory.mem_bank, "MAIN3") == 0 ||
		strcmp(memory.mem_bank, "MAIN4") == 0 ||
		strcmp(memory.mem_bank, "MAIN5") == 0 ||
		strcmp(memory.mem_bank, "MAIN6") == 0 ||
		strcmp(memory.mem_bank, "MAIN7") == 0))
		maxmem = BAFFA1_MAIN_MEMORY_SIZE - 1;

	else if ((strcmp(memory.mem_bank, "TAB0") == 0 ||
		strcmp(memory.mem_bank, "TAB0") == 0))
		maxmem = BAFFA1_PAGING_MEMORY_SIZE - 1;

	do{

		sprintf(str_out, "Set Memory %s Offset [0-%06x] | (%06x) ? ", memory.mem_bank, (unsigned int)maxmem, (unsigned int)memory.debug_mem_offset); hw_tty.print(str_out);
		input = hw_tty.gets(6);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 6);

		hw_tty.print("\n");
	} while (
	(strcmp(memory.mem_bank, "BIOS") == 0 && convert_hexstr_to_value(value) >= BAFFA1_BIOS_MEMORY_SIZE) ||
	((strcmp(memory.mem_bank, "LOW") == 0 ||
		strcmp(memory.mem_bank, "MAIN0") == 0 ||
		strcmp(memory.mem_bank, "MAIN1") == 0 ||
		strcmp(memory.mem_bank, "MAIN2") == 0 ||
		strcmp(memory.mem_bank, "MAIN3") == 0 ||
		strcmp(memory.mem_bank, "MAIN4") == 0 ||
		strcmp(memory.mem_bank, "MAIN5") == 0 ||
		strcmp(memory.mem_bank, "MAIN6") == 0 ||
		strcmp(memory.mem_bank, "MAIN7") == 0) && convert_hexstr_to_value(value) >= BAFFA1_MAIN_MEMORY_SIZE) ||
		((strcmp(memory.mem_bank, "TAB0") == 0 ||
			strcmp(memory.mem_bank, "TAB0") == 0) && convert_hexstr_to_value(value) >= BAFFA1_PAGING_MEMORY_SIZE)
		);

		memory.debug_mem_offset = convert_hexstr_to_value(value);
	
}

void debugmenu_main_edit_mem(BAFFA1_MEMORY& memory, HW_TTY& hw_tty) {

	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);

	hw_tty.print("Edit Memory | Address? ");
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);

	unsigned short address = convert_hexstr_to_value(value);
	sprintf(str_out, "\n  %04x=%02x ? ", address, memory.rom_bios.read(address)); hw_tty.print(str_out);
	input = hw_tty.gets(2);

	while (strlen(input) > 0) {

		leftpad(input, value, 2);

		unsigned char b = convert_hexstr_to_value(value);

		memory.rom_bios.write(address, b);
		address++;
		if (address >= BAFFA1_BIOS_MEMORY_SIZE) { hw_tty.print("\n");  return; }

		sprintf(str_out, "\n  %04x=%02x ? ", address, memory.rom_bios.read(address)); hw_tty.print(str_out);
		input = hw_tty.gets(2);
	}

	hw_tty.print("\n");
}



void debugmenu_main_fill_mem(BAFFA1_MEMORY& memory, HW_TTY& hw_tty) {

	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);
	unsigned long address, start, end;
	unsigned char data;

	hw_tty.print("Fill Memory | Start Address? ");
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);
	start = convert_hexstr_to_value(value);

	hw_tty.print(" | End Address? ");
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);
	end = convert_hexstr_to_value(value);

	hw_tty.print(" | Data? ");
	input = hw_tty.gets(2);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 2);
	data = convert_hexstr_to_value(value);


	for (address = start; address <= end && address < BAFFA1_BIOS_MEMORY_SIZE; address++)
		memory.rom_bios.write(address, data);

	hw_tty.print("\n");
}




void debugmenu_main_load_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);
	unsigned short start; //address,  end
//	unsigned char data;

	hw_tty.print("Load Memory | Start Address? ");
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);
	start = convert_hexstr_to_value(value);


	hw_tty.print("\n  Code (max 128 bytes) ] ");
	input = hw_tty.gets(256);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	size_t numdigits = strlen(input) / 2;
	size_t i;
	for (i = 0; i != numdigits; ++i)
	{
		unsigned char output = 16 * toInt(input[2 * i]) + toInt(input[2 * i + 1]);
		baffa1_cpu.memory.rom_bios.write((long)(i + start), output);
	}

	BAFFA1_REGISTERS::set(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh, start);

	hw_tty.print("\n");
}



void debugmenu_main_display_registers(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	hw_tty.print("Display Registers\n");
	hw_tty.print("\n");
	baffa1_cpu.display_registers(hw_tty);
}

void debugmenu_main_edit_register(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);
	//unsigned short address; // , end
//	unsigned char data;

	hw_tty.print("Edit Register ? ");
	input = hw_tty.gets(3);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	if (strcmp(input, "A") == 0) {
		sprintf(str_out, " | A=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.Al, baffa1_cpu.registers.Ah)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.Al, baffa1_cpu.registers.Ah, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "B") == 0) {
		sprintf(str_out, " | B=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.Bl, baffa1_cpu.registers.Bh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.Bl, baffa1_cpu.registers.Bh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "C") == 0) {
		sprintf(str_out, " | C=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.Cl, baffa1_cpu.registers.Ch)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.Cl, baffa1_cpu.registers.Ch, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "D") == 0) {
		sprintf(str_out, " | D=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.Dl, baffa1_cpu.registers.Dh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.Dl, baffa1_cpu.registers.Dh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "G") == 0) {
		sprintf(str_out, " | G=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.alu.Gl, baffa1_cpu.alu.Gh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.alu.Gl, baffa1_cpu.alu.Gh, convert_hexstr_to_value(value));
	}


	else if (strcmp(input, "BP") == 0) {
		sprintf(str_out, " | BP=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.BPl, baffa1_cpu.registers.BPh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.BPl, baffa1_cpu.registers.BPh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "SP") == 0) {
		sprintf(str_out, " | SP=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.SPl, baffa1_cpu.registers.SPh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.SPl, baffa1_cpu.registers.SPh, convert_hexstr_to_value(value));
	}

	else if (strcmp(input, "SI") == 0) {
		sprintf(str_out, " | SI=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.SIl, baffa1_cpu.registers.SIh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.SIl, baffa1_cpu.registers.SIh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "DI") == 0) {
		sprintf(str_out, " | DI=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.DIl, baffa1_cpu.registers.DIh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.DIl, baffa1_cpu.registers.DIh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "PC") == 0) {
		sprintf(str_out, " | PC=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh, convert_hexstr_to_value(value));
	}


	else if (strcmp(input, "TDR") == 0) {
		sprintf(str_out, " | TDR=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.TDRl, baffa1_cpu.registers.TDRh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.TDRl, baffa1_cpu.registers.TDRh, convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "SSP") == 0) {
		sprintf(str_out, " | SSP=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.SSPl, baffa1_cpu.registers.SSPh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.SSPl, baffa1_cpu.registers.SSPh, convert_hexstr_to_value(value));

	}
	else if (strcmp(input, "PTB") == 0) {
		sprintf(str_out, " | PTB=%02x | Data? ", baffa1_cpu.memory.PTB.value());
		input = hw_tty.gets(2);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 2);
		baffa1_cpu.memory.PTB.set(convert_hexstr_to_value(value));
	}
	else if (strcmp(input, "MSW") == 0) {
		sprintf(str_out, " | MSW=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.alu.MSWl, baffa1_cpu.alu.MSWh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_REGISTERS::set(baffa1_cpu.alu.MSWl, baffa1_cpu.alu.MSWh, convert_hexstr_to_value(value));
	}

	else if (strcmp(input, "MAR") == 0) {
		sprintf(str_out, " | MAR=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.MARl, baffa1_cpu.registers.MARh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_MWORD index = convert_hexstr_to_value(value);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.MARl, baffa1_cpu.registers.MARh, index);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.MDRl, baffa1_cpu.registers.MDRh, baffa1_cpu.memory.rom_bios.read(index));
	}

	else if (strcmp(input, "MDR") == 0) {
		sprintf(str_out, " | MDR=%04x | Data? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.MDRl, baffa1_cpu.registers.MDRh)); hw_tty.print(str_out);
		input = hw_tty.gets(4);

		if (strlen(input) == 0) { hw_tty.print("\n");  return; }

		leftpad(input, value, 4);
		BAFFA1_MWORD index = convert_hexstr_to_value(value);
		BAFFA1_REGISTERS::set(baffa1_cpu.registers.MDRl, baffa1_cpu.registers.MDRh, index);
	}
	hw_tty.print("\n");
}

void debugmenu_main_edit_breakpoint(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {
	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);

	sprintf(str_out, "Edit Breakpoint | BKPT=%04x | Address (FFFF=disable) ? ", baffa1_cpu.BKPT); hw_tty.print(str_out);
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);
	baffa1_cpu.BKPT = convert_hexstr_to_value(value);

	hw_tty.print("\n");
}

void debugmenu_main_edit_programcounter(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	char str_out[255];
	char *input;
	char *value = (char*)malloc(sizeof(char) * 257);

	sprintf(str_out, "Edit Program Counter | PC=%04x | Address ? ", BAFFA1_REGISTERS::value(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh)); hw_tty.print(str_out);
	input = hw_tty.gets(4);

	if (strlen(input) == 0) { hw_tty.print("\n");  return; }

	leftpad(input, value, 4);
	BAFFA1_REGISTERS::set(baffa1_cpu.registers.PCl, baffa1_cpu.registers.PCh, convert_hexstr_to_value(value));
	hw_tty.print("\n");
}

void debugmenu_main_reset_cpu(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	hw_tty.print("Reset CPU\n");
	hw_tty.print("\n");
	baffa1_cpu.microcode.init(hw_tty);
	baffa1_cpu.reset();
	baffa1_cpu.display_registers(hw_tty);
}

void debugmenu_main_reload_bios(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	hw_tty.print("Reloading BIOS\n");
	baffa1_cpu.memory.load_bios();
}



void debugmenu_nextpage_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty, int size) {

	baffa1_cpu.memory.debug_mem_offset += size;

	if((strcmp(baffa1_cpu.memory.mem_bank, "BIOS") == 0 && baffa1_cpu.memory.debug_mem_offset >= BAFFA1_BIOS_MEMORY_SIZE) ||
		((strcmp(baffa1_cpu.memory.mem_bank, "LOW") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN0") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN1") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN2") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN3") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN4") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN5") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN6") == 0 ||
			strcmp(baffa1_cpu.memory.mem_bank, "MAIN7") == 0) && baffa1_cpu.memory.debug_mem_offset >= BAFFA1_MAIN_MEMORY_SIZE) ||
			((strcmp(baffa1_cpu.memory.mem_bank, "TAB0") == 0 ||
				strcmp(baffa1_cpu.memory.mem_bank, "TAB0") == 0) && baffa1_cpu.memory.debug_mem_offset >= BAFFA1_PAGING_MEMORY_SIZE)
		)
		baffa1_cpu.memory.debug_mem_offset -= size;

	baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
}
void debugmenu_lastpage_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty, int size){
	if (baffa1_cpu.memory.debug_mem_offset - size >= 0)
		baffa1_cpu.memory.debug_mem_offset -= size;
	else
		baffa1_cpu.memory.debug_mem_offset = 0;

	baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
}


//debugmenu_main_menu();
int debugmenu_main(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty) {

	while (1) {

		hw_tty.print("> ");
		int key = hw_tty.get_char();

		////////

		if (key == (int)'d' || key == (int)'D')
			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);

		else if (key == (int)'s' || key == (int)'S')
			debugmenu_main_set_mem_bank(baffa1_cpu.memory, hw_tty);

		else if (key == (int)'n' || key == (int)'N')
			debugmenu_main_set_mem_offset(baffa1_cpu.memory, hw_tty);

		//else if (key == (int)'i' || key == (int)'I')

		else if (key == (int)'e' || key == (int)'E')
			debugmenu_main_edit_mem(baffa1_cpu.memory, hw_tty);

		else if (key == (int)'f' || key == (int)'F')
			debugmenu_main_fill_mem(baffa1_cpu.memory, hw_tty);

		else if (key == (int)'l' || key == (int)'L')
			debugmenu_main_load_mem(baffa1_cpu, hw_tty);

		////////

		else if (key == (int)'r' || key == (int)'R')
			debugmenu_main_display_registers(baffa1_cpu, hw_tty);

		else if (key == (int)'a' || key == (int)'A')
			debugmenu_main_edit_register(baffa1_cpu, hw_tty);

		else if (key == (int)'b' || key == (int)'B')
			debugmenu_main_edit_breakpoint(baffa1_cpu, hw_tty);

		else if (key == (int)'p' || key == (int)'P')
			debugmenu_main_edit_programcounter(baffa1_cpu, hw_tty);

		////////

		else if (key == (int)'g' || key == (int)'G') {
			return 1;
		}

		else if (key == (int)'t' || key == (int)'T') {
			return 2;
		}

		else if (key == (int)'z' || key == (int)'Z')
			debugmenu_main_reset_cpu(baffa1_cpu, hw_tty);

		else if (key == (int)'v' || key == (int)'V')
			debugmenu_main_reload_bios(baffa1_cpu, hw_tty);

		////////

		else if (key == (int)'x' || key == (int)'X')
			debugmenu_roms(baffa1_cpu, hw_tty);

		////////

		else if (key == (int)'?')
			debugmenu_main_menu(baffa1_cpu, hw_tty);

		else if (key == (int)'q' || key == (int)'Q')
			return 0;

		//----------------------
		/*
		else if (key == (int)'m' || key == (int)'M') {
			baffa1_cpu.memory.debug_manual_offset = baffa1_cpu.memory.debug_manual_offset == 0x00 ? 0x01 : 0x00;

			if (!get_byte_bit(baffa1_cpu.alu.MSWl.value(), MSWl_PAGING_EN))
				baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
			else
				baffa1_cpu.memory.display_test(baffa1_cpu.registers, hw_tty);
		}
		*/

		else if (key == (int)'2') {
			debugmenu_nextpage_mem(baffa1_cpu, hw_tty, 0x10);
		}
		if (key == (int)'1') {
			debugmenu_lastpage_mem(baffa1_cpu, hw_tty, 0x10);
		}

		if (key == (int)'4') {
			debugmenu_nextpage_mem(baffa1_cpu, hw_tty, 0x100);
		}
		if (key == (int)'3') {
			debugmenu_lastpage_mem(baffa1_cpu, hw_tty, 0x100);
		}
		if (key == (int)'6') {
			debugmenu_nextpage_mem(baffa1_cpu, hw_tty, 0x1000);
		}
		if (key == (int)'5') {
			debugmenu_lastpage_mem(baffa1_cpu, hw_tty, 0x1000);
		}



		else if (key == (int)'@') {
			if (baffa1_cpu.memory.debug_mem_offset + 0x10 < BAFFA1_BIOS_MEMORY_SIZE)
				baffa1_cpu.memory.debug_mem_offset += 0x10;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
		}
		if (key == (int)'!') {
			if (baffa1_cpu.memory.debug_mem_offset - 0x10 >= 0)
				baffa1_cpu.memory.debug_mem_offset -= 0x10;
			else
				baffa1_cpu.memory.debug_mem_offset = 0;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
		}
		if (key == (int)'$') {
			if (baffa1_cpu.memory.debug_mem_offset + 0x100 < BAFFA1_BIOS_MEMORY_SIZE)
				baffa1_cpu.memory.debug_mem_offset += 0x100;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
		}
		if (key == (int)'#') {
			if (baffa1_cpu.memory.debug_mem_offset - 0x100 >= 0)
				baffa1_cpu.memory.debug_mem_offset -= 0x100;
			else
				baffa1_cpu.memory.debug_mem_offset = 0;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
		}
		if (key == (int)'^') {
			if (baffa1_cpu.memory.debug_mem_offset + 0x1000 < BAFFA1_BIOS_MEMORY_SIZE)
				baffa1_cpu.memory.debug_mem_offset += 0x1000;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);
		}
		if (key == (int)'%') {
			if (baffa1_cpu.memory.debug_mem_offset - 0x1000 >= 0)
				baffa1_cpu.memory.debug_mem_offset -= 0x1000;
			else
				baffa1_cpu.memory.debug_mem_offset = 0;
			baffa1_cpu.memory.debug_manual_offset = 1;

			baffa1_cpu.memory.displayMainMemory(baffa1_cpu.registers, hw_tty);

		}

		//----------------------
		////////

		else
			hw_tty.print("\n");

		////////
	}

	return 0;
}

