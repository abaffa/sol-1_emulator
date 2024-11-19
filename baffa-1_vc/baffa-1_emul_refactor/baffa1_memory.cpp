//
// memory.cpp
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
#include "baffa1_memory.h"
#include <stdlib.h>

void BAFFA1_MEMORY::create_mem(long memsize) {
	this->readonly = false;
	this->size = memsize;
	this->data = (BAFFA1_BYTE*)malloc(size * sizeof(BAFFA1_BYTE));
}


void BAFFA1_MEMORY::init() {
	this->reset();
}

void BAFFA1_MEMORY::reset() {
	int address = 0;

	for (address = 0; address < this->size; address++) this->write(address, 0x00);
}

BAFFA1_BYTE BAFFA1_MEMORY::read(long address) {
	return this->data[address];
}

void BAFFA1_MEMORY::write(long address, BAFFA1_BYTE value) {

	if (this->readonly) return;

	this->data[address] = value;
}