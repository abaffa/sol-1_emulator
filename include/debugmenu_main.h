//
// debugmenu_main.h
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
#ifndef DEBUGMENU_MAIN_H
#define DEBUGMENU_MAIN_H

#include "baffa1_cpu.h"

void debugmenu_main_menu(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);

void debugmenu_main_set_mem_bank(BAFFA1_MEMORY& memory, HW_TTY& hw_tty);
void debugmenu_main_set_mem_offset(BAFFA1_MEMORY& memory, HW_TTY& hw_tty);
void debugmenu_main_disassemble_mem(HW_TTY& hw_tty);
void debugmenu_main_edit_mem(BAFFA1_MEMORY& memory, HW_TTY& hw_tty);
void debugmenu_main_fill_mem(BAFFA1_MEMORY& memory, HW_TTY& hw_tty);
void debugmenu_main_load_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);
void debugmenu_main_display_registers(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);
void debugmenu_main_edit_register(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);
void debugmenu_main_edit_breakpoint(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);
void debugmenu_main_edit_programcounter(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);
void debugmenu_main_reset_cpu(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);


void debugmenu_nextpage_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty, int size);
void debugmenu_lastpage_mem(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty, int size);

int debugmenu_main(BAFFA1_CPU& baffa1_cpu, HW_TTY& hw_tty);

#endif