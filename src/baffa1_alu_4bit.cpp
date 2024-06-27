//
// baffa1_alu_4bit.cpp
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
#include "baffa1_alu_4bit.h"
#include "utils.h"

// INVERTER
static BAFFA1_BYTE IC_74LS04(BAFFA1_BYTE A) {
	return get_byte_bit(~A, 0);
}

static BAFFA1_BYTE IC74LS181_B1(BAFFA1_BYTE nA, BAFFA1_BYTE nB, BAFFA1_BYTE S, int block) {

	BAFFA1_BYTE S0 = get_byte_bit(S, 0);
	BAFFA1_BYTE S1 = get_byte_bit(S, 1);

	BAFFA1_BYTE andA = get_byte_bit(nA, block);
	BAFFA1_BYTE andB = get_byte_bit(nB, block) & S0;
	BAFFA1_BYTE andC = S1 & IC_74LS04(get_byte_bit(nB, block));

	return IC_74LS04(andA | andB | andC);
}

static BAFFA1_BYTE IC74LS181_B2(BAFFA1_BYTE nA, BAFFA1_BYTE nB, BAFFA1_BYTE S, int block) {

	BAFFA1_BYTE S2 = get_byte_bit(S, 2);
	BAFFA1_BYTE S3 = get_byte_bit(S, 3);

	BAFFA1_BYTE andA = IC_74LS04(get_byte_bit(nB, block)) & S2 & get_byte_bit(nA, block);
	BAFFA1_BYTE andB = get_byte_bit(nA, block) & get_byte_bit(nB, block) & S3;

	return IC_74LS04(andA | andB);

}

static BAFFA1_BYTE IC74LSL181(BAFFA1_BYTE nA, BAFFA1_BYTE nB, BAFFA1_BYTE S, BAFFA1_BYTE M, BAFFA1_BYTE Cn,
	BAFFA1_BYTE *AeqB, BAFFA1_BYTE *Cn4, BAFFA1_BYTE *nP, BAFFA1_BYTE *nG) {

	BAFFA1_BYTE B0_1 = IC74LS181_B1(nA, nB, S, 0);
	BAFFA1_BYTE nB0_1 = IC_74LS04(B0_1);
	BAFFA1_BYTE B0_2 = IC74LS181_B2(nA, nB, S, 0);

	BAFFA1_BYTE B1_1 = IC74LS181_B1(nA, nB, S, 1);
	BAFFA1_BYTE nB1_1 = IC_74LS04(B1_1);
	BAFFA1_BYTE B1_2 = IC74LS181_B2(nA, nB, S, 1);

	BAFFA1_BYTE B2_1 = IC74LS181_B1(nA, nB, S, 2);
	BAFFA1_BYTE nB2_1 = IC_74LS04(B2_1);
	BAFFA1_BYTE B2_2 = IC74LS181_B2(nA, nB, S, 2);

	BAFFA1_BYTE B3_1 = IC74LS181_B1(nA, nB, S, 3);
	BAFFA1_BYTE nB3_1 = IC_74LS04(B3_1);
	BAFFA1_BYTE B3_2 = IC74LS181_B2(nA, nB, S, 3);

	BAFFA1_BYTE nM = IC_74LS04(M);

	BAFFA1_BYTE nF0 = IC_74LS04(Cn & nM) ^ (nB0_1 & B0_2); // (B0_1 ^ B0_2); 

	BAFFA1_BYTE nF1 = IC_74LS04((nM & B0_1) | (nM & B0_2 & Cn)) ^ (nB1_1 & B1_2); //(B1_1 ^ B1_2);

	BAFFA1_BYTE nF2 = IC_74LS04((nM & B1_1) | (nM & B0_1 & B1_2) | (nM & Cn & B0_2 & B1_2)) ^ (nB2_1 & B2_2);  //(B2_1 ^ B2_2); 

	BAFFA1_BYTE nF3 = IC_74LS04((nM & B2_1) | (nM & B1_1 & B2_2) | (nM & B0_1 & B1_2 & B2_2) | (nM & Cn & B0_2 & B1_2 & B2_2)) ^ (nB3_1 & B3_2); // (B3_1 ^ B3_2)

	*AeqB = get_byte_bit(nF0 & nF1 & nF2 & nF3, 0);
	*nP = get_byte_bit(IC_74LS04(B0_2 & B1_2 & B2_2 & B3_2), 0);
	*nG = get_byte_bit(IC_74LS04((B0_1 & B1_2 & B2_2 & B3_2) | (B1_1 & B2_2 & B3_2) | (B2_1 & B3_2) | B3_1), 0);

	*Cn4 = get_byte_bit((Cn & B0_2 & B1_2 & B2_2 & B3_2) | IC_74LS04(*nG), 0);
	BAFFA1_BYTE nF = (nF3 << 3) | (nF2 << 2) | (nF1 << 1) | (nF0);
	return nF;
}

static void baffa1_alu_4bit_reset(struct baffa1_alu_4bit *alu) {

	alu->F = 0x00;

	alu->COUT = 0x00; //carry in

	alu->EQ = 0x00;
	alu->P = 0x00;
	alu->G = 0x00;

}

//74LS181
BAFFA1_BYTE baffa1_alu_4bit_op(struct baffa1_alu_4bit* alu, BAFFA1_BYTE A, BAFFA1_BYTE B, BAFFA1_BYTE CIN, BAFFA1_BYTE S, BAFFA1_BYTE M) {

	baffa1_alu_4bit_reset(alu);

	alu->F = IC74LSL181(A, B, S, M, CIN, &alu->EQ, &alu->COUT, &alu->P, &alu->G);

	return alu->F;
}
