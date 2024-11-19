//
// baffa1_interruptions.cpp
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
#include "baffa1_interruptions.h"
#include <stdlib.h>
#include <assert.h>
#include "baffa1_utils.h"


void BAFFA1_INTERRUPTIONS::init()
{
	this->reset();
}

void BAFFA1_INTERRUPTIONS::reset()
{
	this->interruptions_bus.reset();
}


void BAFFA1_INTERRUPTIONS::refresh_int(BAFFA1_CONTROLLER_BUS& controller_bus, BAFFA1_BYTE INT_MASKS_value) {

	BAFFA1_BYTE int_req_0 = get_byte_bit(interruptions_bus.int_req, 0);
	BAFFA1_BYTE int_req_1 = get_byte_bit(interruptions_bus.int_req, 1);
	BAFFA1_BYTE int_req_2 = get_byte_bit(interruptions_bus.int_req, 2);
	BAFFA1_BYTE int_req_3 = get_byte_bit(interruptions_bus.int_req, 3);
	BAFFA1_BYTE int_req_4 = get_byte_bit(interruptions_bus.int_req, 4);
	BAFFA1_BYTE int_req_5 = get_byte_bit(interruptions_bus.int_req, 5);
	BAFFA1_BYTE int_req_6 = get_byte_bit(interruptions_bus.int_req, 6);
	BAFFA1_BYTE int_req_7 = get_byte_bit(interruptions_bus.int_req, 7);

	BAFFA1_BYTE int_vector_0 = get_byte_bit(interruptions_bus.int_vector, 0); // 74ls138 disable = 1
	BAFFA1_BYTE int_vector_1 = get_byte_bit(interruptions_bus.int_vector, 1);
	BAFFA1_BYTE int_vector_2 = get_byte_bit(interruptions_bus.int_vector, 2);
	BAFFA1_BYTE int_vector_3 = get_byte_bit(interruptions_bus.int_vector, 3);
	BAFFA1_BYTE int_vector_4 = get_byte_bit(interruptions_bus.int_vector, 4);
	BAFFA1_BYTE int_vector_5 = get_byte_bit(interruptions_bus.int_vector, 5);
	BAFFA1_BYTE int_vector_6 = get_byte_bit(interruptions_bus.int_vector, 6);
	BAFFA1_BYTE int_vector_7 = get_byte_bit(interruptions_bus.int_vector, 7);
	
	BAFFA1_BYTE int_status_0 = get_byte_bit(interruptions_bus.int_status, 0);
	BAFFA1_BYTE int_status_1 = get_byte_bit(interruptions_bus.int_status, 1);
	BAFFA1_BYTE int_status_2 = get_byte_bit(interruptions_bus.int_status, 2);
	BAFFA1_BYTE int_status_3 = get_byte_bit(interruptions_bus.int_status, 3);
	BAFFA1_BYTE int_status_4 = get_byte_bit(interruptions_bus.int_status, 4);
	BAFFA1_BYTE int_status_5 = get_byte_bit(interruptions_bus.int_status, 5);
	BAFFA1_BYTE int_status_6 = get_byte_bit(interruptions_bus.int_status, 6);
	BAFFA1_BYTE int_status_7 = get_byte_bit(interruptions_bus.int_status, 7);

	BAFFA1_BYTE not_clear_all_ints = get_byte_bit(~controller_bus.clear_all_ints, 0x00);

	if (controller_bus.int_ack == 0x01) {

		BAFFA1_BYTE out_vector = (get_byte_bit(interruptions_bus.int_vector, 3) << 2) |
			(get_byte_bit(interruptions_bus.int_vector, 2) << 1) |
			(get_byte_bit(interruptions_bus.int_vector, 1) << 0);

		if (out_vector == 0)
			int_vector_0 = 0;
		else if (out_vector == 1)
			int_vector_1 = 0;
		else if (out_vector == 2)
			int_vector_2 = 0;
		else if (out_vector == 3)
			int_vector_3 = 0;
		else if (out_vector == 4)
			int_vector_4 = 0;
		else if (out_vector == 5)
			int_vector_5 = 0;
		else if (out_vector == 6)
			int_vector_6 = 0;
		else if (out_vector == 7)
			int_vector_7 = 0;

		//int_vector_0 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 0), 0);
		//int_vector_1 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 1), 0);
		//int_vector_2 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 2), 0);
		//int_vector_3 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 3), 0);
		//int_vector_4 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 4), 0);
		//int_vector_5 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 5), 0);
		//int_vector_6 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 6), 0);
		//int_vector_7 = get_byte_bit(~get_byte_bit(interruptions_bus.int_vector, 7), 0);
	}

	BAFFA1_BYTE int_clr_0 = (int_vector_0 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_1 = (int_vector_1 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_2 = (int_vector_2 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_3 = (int_vector_3 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_4 = (int_vector_4 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_5 = (int_vector_5 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_6 = (int_vector_6 & not_clear_all_ints);
	BAFFA1_BYTE int_clr_7 = (int_vector_7 & not_clear_all_ints);


	if (int_clr_0 == 0x01 && get_byte_bit(interruptions_bus.int_req, 0))
		int_status_0 = 1;// get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 0), 0);
	else if (int_clr_0 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11111110;

	if (int_clr_1 == 0x01 && get_byte_bit(interruptions_bus.int_req, 1))
		int_status_1 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 1), 0);
	else if (int_clr_1 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11111101;

	if (int_clr_2 == 0x01 && get_byte_bit(interruptions_bus.int_req, 2))
		int_status_2 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 2), 0);
	else if (int_clr_2 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11111011;

	if (int_clr_3 == 0x01 && get_byte_bit(interruptions_bus.int_req, 3))
		int_status_3 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 3), 0);
	else if (int_clr_3 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11110111;

	if (int_clr_4 == 0x01 && get_byte_bit(interruptions_bus.int_req, 4))
		int_status_4 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 4), 0);
	else if (int_clr_4 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11101111;

	if (int_clr_5 == 0x01 && get_byte_bit(interruptions_bus.int_req, 5))
		int_status_5 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 5), 0);
	else if (int_clr_5 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b11011111;

	if (int_clr_6 == 0x01 && get_byte_bit(interruptions_bus.int_req, 6))
		int_status_6 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 6), 0);
	else if (int_clr_6 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b10111111;

	if (int_clr_7 == 0x01 && get_byte_bit(interruptions_bus.int_req, 7))
		int_status_7 = 1;//get_byte_bit(~get_byte_bit(interruptions_bus.int_status, 7), 0);
	else if (int_clr_7 == 0x00)
		interruptions_bus.int_req = interruptions_bus.int_req & 0b01111111;


	interruptions_bus.int_status = (int_status_7 << 7) | (int_status_6 << 6) | (int_status_5 << 5) | (int_status_4 << 4) | (int_status_3 << 3) | (int_status_2 << 2) | (int_status_1 << 1) | (int_status_0 << 0);

	BAFFA1_BYTE masked_status = (~(interruptions_bus.int_status & INT_MASKS_value)) & 0b11111111;

	interruptions_bus.int_request = (masked_status != 0b11111111) ? 0x1 : 0x0;

	if (controller_bus.int_vector_wrt == 0x00) {

		BAFFA1_BYTE int_out = 0;

		if (get_byte_bit(masked_status, 0) == 0)
			int_out = 7;
		else if (get_byte_bit(masked_status, 1) == 0)
			int_out = 6;
		else if (get_byte_bit(masked_status, 2) == 0)
			int_out = 5;
		else if (get_byte_bit(masked_status, 3) == 0)
			int_out = 4;
		else if (get_byte_bit(masked_status, 4) == 0)
			int_out = 3;
		else if (get_byte_bit(masked_status, 5) == 0)
			int_out = 2;
		else if (get_byte_bit(masked_status, 6) == 0)
			int_out = 1;
		else if (get_byte_bit(masked_status, 7) == 0)
			int_out = 0;

		if (interruptions_bus.int_request != 0x00)
			interruptions_bus.int_vector = ((~int_out) & 0b111) << 1;
	}
}


#pragma endregion
