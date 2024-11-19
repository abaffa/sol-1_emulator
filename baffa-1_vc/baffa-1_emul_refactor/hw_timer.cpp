//
// hw_timer.cpp
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
#include "hw_timer.h"
#include "baffa1_utils.h"

#include <time.h>

void hw_timer_init(struct hw_timer* hw_timer) {
	int i = 0;
	for (i = 0; i < 4; i++) {
		hw_timer->data[i] = 0x00;
	}
}


void hw_timer_set_c0(struct hw_timer* hw_timer, BAFFA1_BYTE c) {
	hw_timer->data[0] = c;
	if (hw_timer->data[0] == 0xFF) {
		hw_timer->data[3] = hw_timer->data[3] | 0b00100000;
	}
	else if (hw_timer->data[0] == 0x00) {
		hw_timer->data[3] = hw_timer->data[3] & 0b11011111;
	}
}

void hw_timer_set_c1(struct hw_timer* hw_timer, BAFFA1_BYTE c) {
	hw_timer->data[1] = c;
	if (get_byte_bit(hw_timer->data[3], 6) == 0x01)
	{
		hw_timer->data[1]++;
		if (hw_timer->data[1] == 0xFF) {
			hw_timer->data[3] = hw_timer->data[3] | 0b10000000;
		}
		else if (hw_timer->data[1] == 0x00) {
			hw_timer->data[3] = hw_timer->data[3] & 0b01111111;
		}
	}
}


void hw_timer_tick_c0(struct hw_timer* hw_timer)
{
	if (get_byte_bit(hw_timer->data[3], 4) == 0x01)
	{
		hw_timer->data[0]++;
		if (hw_timer->data[0] == 0xFF) {
			hw_timer->data[3] = hw_timer->data[3] | 0b00100000;
		}
		else if (hw_timer->data[0] == 0x00) {
			hw_timer->data[3] = hw_timer->data[3] & 0b11011111;
		}
	}

}




void hw_timer_tick_c1(struct hw_timer* hw_timer)
{
	if (get_byte_bit(hw_timer->data[3], 6) == 0x01)
	{
		hw_timer->data[1]++;
		if (hw_timer->data[1] == 0xFF) {
			hw_timer->data[3] = hw_timer->data[3] | 0b10000000;
		}
		else if (hw_timer->data[1] == 0x00) {
			hw_timer->data[3] = hw_timer->data[3] & 0b01111111;
		}
	}

}


void hw_timer_print(struct hw_timer* hw_timer, char *dir, int changed, char *print) {


	int i = 0;
	sprintf(print, ">>> TIMER [%s]:", dir);
	for (i = 0; i < 4; i++) {
		if (changed == i)
			sprintf(print + strlen(print), "[%02x", hw_timer->data[i]);
		else if (changed == i - 1)
			sprintf(print + strlen(print), "]%02x", hw_timer->data[i]);
		else
			sprintf(print + strlen(print), " %02x", hw_timer->data[i]);
	}
	if (changed == 3)
		sprintf(print + strlen(print), "]\n");
	else
		sprintf(print + strlen(print), "\n");
}
