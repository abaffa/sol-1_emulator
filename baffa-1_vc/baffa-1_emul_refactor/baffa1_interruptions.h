//
// baffa1_interruptions.h
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
#ifndef BAFFA1INTERRUPTIONS_H
#define BAFFA1INTERRUPTIONS_H

#include <stdio.h>
#include "baffa1_defs.h"
//#include "baffa1_memory.h"
//#include "baffa1_register_8bit.h"
#include "baffa1_controller_bus.h"
#include "baffa1_interruptions_bus.h"

#include "baffa1_config.h"

class BAFFA1_INTERRUPTIONS
{
public:

	BAFFA1_CONFIG cpu_config;

	BAFFA1_INTERRUPTIONS_BUS interruptions_bus;

	void init();
	void reset();

	void refresh_int(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_BYTE INT_MASKS_value);
//private:


};
#endif