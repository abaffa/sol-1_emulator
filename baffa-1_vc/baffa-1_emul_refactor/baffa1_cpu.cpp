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
#include "baffa1_utils.h"

void BAFFA1_CPU::init(BAFFA1_CONFIG &cpu_config, HW_TTY &_hw_tty)
{
	this->display_reg_load = 0;
	
	//this->cpu_config = cpu_config;
	//this->_hw_tty = _hw_tty;
	//this->_hw_web = _hw_web;

	this->microcode.init(_hw_tty);
	this->alu.init();
	this->registers.init();
	this->memory_bios.init(_hw_tty);
	this->interruptions.init();

	this->controller_bus.init();
	//this->alu_bus.init();
	//this->registers_bus.init();
	this->data_bus.init();
	this->interruptions_bus.init();

	reset(_hw_tty);

}

void BAFFA1_CPU::reset(HW_TTY &_hw_tty)
{
	/*
	this->microcode.cpu_config = this->cpu_config;
	this->alu.cpu_config = this->cpu_config;
	this->registers.cpu_config = this->cpu_config;
	this->memory_bios.cpu_config = this->cpu_config;
	this->interruptions.cpu_config = this->cpu_config;
	*/

	this->microcode.reset();
	this->alu.reset();
	this->registers.reset();
	this->memory_bios.reset(_hw_tty);
	this->interruptions.reset(); 

	this->controller_bus.reset();
	//this->alu_bus.reset();
	//this->registers_bus.reset();
	this->data_bus.reset();
	this->interruptions_bus.reset();


	this->BKPT = 0xFFFF; // Breakpoint
}



BAFFA1_MEMORY BAFFA1_CPU::get_current_memory() {
	BAFFA1_MEMORY memory;
	if (!get_byte_bit(this->alu.MSWl.value(), MSWl_PAGING_EN))
		memory = this->memory_bios.rom_bios;

	else
		memory = this->memory_bios.low_memory;

	return memory;
}



unsigned int BAFFA1_CPU::get_current_memory_size() {

	if (!get_byte_bit(this->alu.MSWl.value(), MSWl_PAGING_EN))
		return  this->memory_bios.rom_bios.size;

	else
		return this->memory_bios.rom_bios.size;
}


