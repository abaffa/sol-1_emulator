//
// utils.h
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
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "baffa1_defs.h"
#include "baffa1_register_8bit.h"

#ifdef __linux__
int getch(void);
int kbhit();
char* itoa(int value, char* result, int base);
#else
#include "conio.h"
#endif

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define INV_BYTE_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x80 ? '1' : '0')

#define NIBBLE_TO_BINARY(byte)  \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define LSN(w) ((w) & 0x0f)
#define MSN(w) (((w) >> 4) & 0x0f)

#define WORD(l, h) (((h)<<8) | (l))
#define LSB(w) ((w) & 0xff)
#define MSB(w) (((w) >> 8) & 0xff)
#define SET_LSB(Rg, l) Rg = ((Rg & 0xFF00) | ((l) & 0xff))
#define SET_MSB(Rg, h) Rg = ((h)<<8) | (Rg & 0x00FF)
#define SET_WORD(Rg, l, h) Rg = WORD(l, h)

void print_word_bin(char *s, BAFFA1_DWORD n);
void print_word_bin_nibbles(char *s, BAFFA1_DWORD n);

void print_byte_bin(char *s, BAFFA1_BYTE b);

void print_nibble_bin(char *s, BAFFA1_BYTE b);

unsigned int toInt(char c);



void leftpad(char *str1, char *str2, int pad);
char* rightpad(const char *str1, int pad);

unsigned int convert_hexstr_to_value(char *value);

BAFFA1_DWORD set_word_bit(BAFFA1_DWORD v, int bit);

BAFFA1_DWORD get_word_bit(BAFFA1_DWORD v, int bit);
BAFFA1_BYTE get_byte_bit(BAFFA1_BYTE v, int bit);
BAFFA1_BYTE get_byte_bit(BAFFA1_BYTE v, int bit);
BAFFA1_BYTE set_byte_bit(BAFFA1_BYTE v, int bit);

char* loadfile(char *s, const char *filename, long *size);

void save_to_log(char *s, FILE *fa, char *str);

char* strlower(char* s);

BAFFA1_DWORD value_register(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h);
void set_register(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h, BAFFA1_DWORD v);
void reset_register(BAFFA1_REGISTER_8BIT& l, BAFFA1_REGISTER_8BIT& h);


#endif