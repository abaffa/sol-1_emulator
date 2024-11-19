//
// BAFFA1_INTERRUPTIONS_BUS.h
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
#ifndef BAFFA1INTERRUPTIONSBUS_H
#define BAFFA1INTERRUPTIONSBUS_H

#include "baffa1_defs.h"

class BAFFA1_INTERRUPTIONS_BUS {

public:

	// Interrupções
	BAFFA1_BYTE int_vector; // register, int
	BAFFA1_BYTE int_status;// register, int
	BAFFA1_BYTE int_request; //Memory	Int/Clk
	BAFFA1_BYTE int_req; // Int/Clk

	void init();
	void reset();
};

#endif