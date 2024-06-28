//
// baffa1_bus.cpp
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
#include "baffa1_bus.h"
#include "utils.h"

BAFFA1_BYTE BAFFA1_BUS::bus_tristate(BAFFA1_ALU& baffa1_alu) {
	return get_byte_bit(baffa1_alu.MSWl.value(), MSWl_DMA_ACK) | get_byte_bit(baffa1_alu.MSWl.value(), MSWl_HALT); //IC151
}

BAFFA1_BYTE BAFFA1_BUS::bus_rd(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE rd, BAFFA1_BYTE panel_rd) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(baffa1_alu) != 0x00)
		ret = panel_rd;
	else
		ret = rd;

	return (~ret) & 0b00000001;
}

BAFFA1_BYTE BAFFA1_BUS::bus_wr(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE wr, BAFFA1_BYTE panel_wr) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(baffa1_alu) != 0x00)
		ret = panel_wr;
	else
		ret = wr;

	return (~ret) & 0b00000001;
}


BAFFA1_BYTE BAFFA1_BUS::bus_mem_io(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE mem_io, BAFFA1_BYTE panel_mem_io) {

	BAFFA1_BYTE ret = 0x00;

	if (bus_tristate(baffa1_alu) != 0x00)
		ret = panel_mem_io;
	else
		ret = mem_io;

	return ret;
}


void BAFFA1_BUS::init() {
	this->reset();
	
	//this->alu_bus.init(); // flags do alu
}

void BAFFA1_BUS::reset() {
	this->data_bus = 0b00000000;
	//this->k_bus = 0b00000000; // input pra alu x e y
	//this->w_bus = 0b00000000; // input pra alu x e y
	
	//this->alu_bus.reset();
}

