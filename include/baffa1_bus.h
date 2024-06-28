//
// baffa1_bus.h
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
#ifndef BAFFA1BUS_H
#define BAFFA1BUS_H
#include "config.h"
#include "baffa1_registers.h"
#include "baffa1_alu_board.h"
#include "baffa1_alu_bus.h"
#include "hw_tty.h"

class BAFFA1_BUS {
	
public:
	BAFFA1_BYTE data_bus;

	//BAFFA1_BYTE k_bus; // input pra alu k -> y
	//BAFFA1_BYTE w_bus; // input pra alu w -> x
	
	//BAFFA1_ALU_BUS alu_bus;

	/////
	BAFFA1_BYTE bus_tristate(BAFFA1_ALU& baffa1_alu);
	BAFFA1_BYTE bus_rd(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE rd, BAFFA1_BYTE panel_rd);
	BAFFA1_BYTE bus_wr(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE wr, BAFFA1_BYTE panel_wr);
	BAFFA1_BYTE bus_mem_io(BAFFA1_ALU& baffa1_alu, BAFFA1_BYTE mem_io, BAFFA1_BYTE panel_mem_io);
	/////

	void init();
	void reset();
};
#endif