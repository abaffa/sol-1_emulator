//
// baffa1_alu_4bit.h
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
#ifndef BAFFA1ALU4BIT_H
#define BAFFA1ALU4BIT_H

#include "baffa1_defs.h"

struct baffa1_alu_4bit
{
	BAFFA1_BYTE F;
	BAFFA1_BYTE COUT; //carry out

	BAFFA1_BYTE EQ; //EQ
	BAFFA1_BYTE P;
	BAFFA1_BYTE G;
};

BAFFA1_BYTE baffa1_alu_4bit_op(struct baffa1_alu_4bit* alu, BAFFA1_BYTE A, BAFFA1_BYTE B, BAFFA1_BYTE CIN, BAFFA1_BYTE S, BAFFA1_BYTE M);

#endif