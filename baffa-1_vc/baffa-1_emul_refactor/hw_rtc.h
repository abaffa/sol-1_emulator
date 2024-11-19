//
// hwrtc.h
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
#ifndef HWRTC_H
#define HWRTC_H
#include "baffa1_config.h"
#include "baffa1_defs.h"

struct hw_rtc{
	BAFFA1_BYTE data[16];
};

BAFFA1_DWORD hw_rtc_current_datetime();


void hw_rtc_init(struct hw_rtc* hw_rtc);
void hw_rtc_print(struct hw_rtc* hw_rtc, char *dir, int changed, char *print);

void hw_rtc_start_clock(struct hw_rtc* hw_rtc);

#endif
