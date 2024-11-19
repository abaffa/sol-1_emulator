//
// hw_timer.h
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
#ifndef HWTIMER_H
#define HWTIMER_H
#include "baffa1_defs.h"
#include "baffa1_config.h"

struct hw_timer {
	BAFFA1_BYTE data[4];
};

//BAFFA1_DWORD hw_timer_current_datetime();


void hw_timer_tick_c0(struct hw_timer* hw_timer);
void hw_timer_tick_c1(struct hw_timer* hw_timer);

void hw_timer_set_c1(struct hw_timer* hw_timer, BAFFA1_BYTE c);
void hw_timer_set_c0(struct hw_timer* hw_timer, BAFFA1_BYTE c);

void hw_timer_init(struct hw_timer* hw_timer);
void hw_timer_print(struct hw_timer* hw_timer, char *dir, int changed, char *print);

#endif
