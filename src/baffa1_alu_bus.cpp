//
// baffa1_alu_bus.cpp
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
#include "baffa1_alu_bus.h"

void BAFFA1_ALU_BUS::init() {
	this->reset();

	// flags do alu
	this->alu_zf = 0x00; // ZeroFlag
	this->alu_cf = 0x00; // Carry Flag
	this->alu_of = 0x00; // Overflow Flag

	this->alu_final_cf = 0x00;
	this->alu_output = 0x00;
}

void BAFFA1_ALU_BUS::reset() {
	this->x_bus = 0b00000000; //alu entrada
	this->y_bus = 0b00000000; //alu entrada
	this->z_bus = 0b00000000; //alu saida
}