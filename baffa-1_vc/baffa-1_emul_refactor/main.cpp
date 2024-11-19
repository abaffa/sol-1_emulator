//
// main.cpp
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
/*
#include "baffa1_computer.h"

int main(int argc, char** argv) {

	BAFFA1_COMPUTER baffa1_computer;
	baffa1_computer.init();
	baffa1_computer.run();

	return 0;
}
*/


#include <stdio.h>
#include <stdlib.h>
#include "baffa1_computer.h"

//#define __debug__

#if defined(__debug__) 
#include "hw_tty.h"
#include "config.h"
#include "baffa1_alu_4bit.h"
#include "baffa1_alu_board.h"

int main(int argc, char** argv) {

	/*
	baffa1_alu_4bit alu;

	BAFFA1_BYTE A = 1;
	BAFFA1_BYTE B = 0;
	BAFFA1_BYTE CIN = 0;
	BAFFA1_BYTE S = 0;
	BAFFA1_BYTE M = 1;

	printf("   S  M  A  B        F EQ CO  G  P\n");
	for (int i = 0; i <= 0b1111; i++) {
		S = i;
		baffa1_alu_4bit_op(&alu, A, B, CIN, S, M);
			printf("%c%c%c%c %02x %02x %02x %c%c%c%c%c%c%c%c %02x %02x %02x %02x\n", NIBBLE_TO_BINARY(S), M, A, B, BYTE_TO_BINARY(alu.F), alu.EQ, alu.COUT, alu.G, alu.P);
	}
	*/


	BAFFA1_ALU alu;
	BAFFA1_REGISTERS registers;
	BAFFA1_MICROCODE microcode;
	BAFFA1_BUS bus;
	BAFFA1_CONFIG config;
	HW_TTY _hw_tty;

	config.DEBUG_ALU = true;
	config.DEBUG_UFLAGS = true;

	bus.init();
	alu.baffa1_alu_init();
	microcode.init(_hw_tty);



	microcode.controller_bus.alu_cf_in_src = 0;
	//alu_cf_in_src	0	0b0		vcc		alu_cf_in_inv = 0
	//alu_cf_in_src	1	0b1		cf		alu_cf_in_inv = 0
	//alu_cf_in_src	2	0b10	u_cf	alu_cf_in_inv = 0
	//alu_cf_in_src	3	0b11	*unused	alu_cf_in_inv = 0
	//alu_cf_in_src	0	0b0		gnd		alu_cf_in_inv = 1
	//alu_cf_in_src	1	0b1		~cf		alu_cf_in_inv = 1
	//alu_cf_in_src	2	0b10	~u_cf	alu_cf_in_inv = 1

	microcode.controller_bus.alu_cf_in_inv = 1;
	//alu_cf_in_inv	0	0	Carry - in not inverted
	//alu_cf_in_inv	1	1	Carry - in inverted

	microcode.controller_bus.alu_cf_out_inv = 1;
	//alu_cf_out_inv	0	0	Carry - out not inverted
	//alu_cf_out_inv	1	1	Carry - out inverted


	microcode.controller_bus.alu_mode = 0; //Mode
	//alu_mode	0	0	ALU Operation   (Arithmetic Operations) - check Cin
	//alu_mode	1	1	ALU Operation	(Logic Functions)

	microcode.controller_bus.alu_op = 0b1001; //Selection
	//alu_op	0	0	ALU Operation	alu_mode = (~Cn=1  A  | ~Cn=0  A + 1)
	//alu_op	0	0	* **unused	alu_mode = 0
	//alu_op	9	1001	plus	alu_mode = 0  (~Cn=1  A + B | ~Cn=0  A + B + 1)
	//alu_op	6	110		minus	alu_mode = 0  (~Cn=1  A - B - 1 | ~Cn=0  A - B)
	//alu_op	11	1011	and		alu_mode = 1
	//alu_op	14	1110	or		alu_mode = 1
	//alu_op	6	110		xor		alu_mode = 1
	//alu_op	15	1111	A		alu_mode = 1
	//alu_op	10	1010	B		alu_mode = 1
	//alu_op	0	0		not A	alu_mode = 1
	//alu_op	5	101		not B	alu_mode = 1
	//alu_op	4	100		nand	alu_mode = 1 
	//alu_op	1	1		nor		alu_mode = 1
	//alu_op	9	1001	nxor	alu_mode = 1


	microcode.controller_bus.shift_src = 0;
	//shift_src	0	0	gnd
	//shift_src	1	1	uCF
	//shift_src	2	10	CF
	//shift_src	3	11	ALU Result[0]
	//shift_src	4	100	ALU Result[7]


	//flags
	//uof_in_src	0	0 * **unchanged
	//uof_in_src	1	1	ALU_OF
	//usf_in_src	0	0 * **unchanged
	//usf_in_src	1	1	Z_BUS_7
	//ucf_in_src	0	0 * **unchanged
	//ucf_in_src	1	1	ALU Final CF
	//ucf_in_src	2	10	ALU_OUTPUT_0
	//ucf_in_src	3	11	ALU_OUTPUT_7
	//uzf_in_src	0	0 * **unchanged
	//uzf_in_src	1	1	ALU_ZF
	//uzf_in_src	2	10	ALU_ZF && uZF
	//uzf_in_src	3	11	gnd

	bus.alu_bus.x_bus = 0b11;
	bus.alu_bus.y_bus = 0b11;

	//microcode.controller_bus.alu_mode = 0
	//microcode.controller_bus.alu_op = 0b1001; //plus
	//microcode.controller_bus.alu_op = 0b110; //minus

	microcode.controller_bus.alu_mode = 0;
	microcode.controller_bus.alu_op = 0b0; // A
	//microcode.controller_bus.alu_op = 0b1001; //nxor - equal
	microcode.controller_bus.alu_cf_in_inv = 0;

	microcode.controller_bus.zbus_out_src = 2;
	//zbus_out_src	0	0	Normal ALU Result
	//zbus_out_src	1	1	Shifted Right
	//zbus_out_src	2	10	Shifted Left
	//zbus_out_src	3	11	Sign Extend



	alu.ALU_EXEC(&microcode.controller_bus, bus.alu_bus, alu.u_cf, get_byte_bit(registers.MSWh.value(), MSWh_CF), config, _hw_tty);
	alu.u_flags_refresh(&microcode.controller_bus, registers.MSWl.value(), registers.MSWh.value(), bus.alu_bus, config, _hw_tty);

	char str_out[255];
	/*
	for(int i = 0;i <=0b1111;i++){
		microcode.controller_bus.alu_op = i;
		sprintf(str_out, "M:%02x", microcode.controller_bus.alu_op); _hw_tty.print(str_out);
		_hw_tty.print("\n");

		alu.ALU_EXEC(&microcode.controller_bus, bus.alu_bus, alu.u_cf, get_byte_bit(registers.MSWh.value(), MSWh_CF), config, _hw_tty);
	}
	*/
}

#elif defined(__linux__) || defined(__MINGW32__)

int main(int argc, char** argv) {



	if (PANEL_ENABLED == 0x00) {

		BAFFA1_COMPUTER baffa1_computer;

		baffa1_computer.init();
		baffa1_computer.run();

	}

	return 0;
}
#elif _MSC_VER        

#include <windows.h>
#include <chrono>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "baffa1_utils.h"

DWORD WINAPI run_thread(LPVOID vargp) {

	BAFFA1_COMPUTER* baffa1_computer = (BAFFA1_COMPUTER*)vargp;

	baffa1_computer->init();
	baffa1_computer->run();

	return NULL;
}




void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx*dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}


SDL_Color getColor(BAFFA1_BYTE v, int bit) {
	SDL_Color led_on = { 255, 0,  0 };
	SDL_Color led_off = { 90, 0,  0 };

	return get_byte_bit(v, bit) == 0x00 ? led_off : led_on;
}



SDL_Color getInvColor(BAFFA1_BYTE v, int bit) {
	SDL_Color led_on = { 255, 0,  0 };
	SDL_Color led_off = { 90, 0,  0 };

	return get_byte_bit(v, bit) != 0x00 ? led_off : led_on;
}


SDL_Color getColorWord(BAFFA1_DWORD v, int bit) {
	SDL_Color led_on = { 255, 0,  0 };
	SDL_Color led_off = { 90, 0,  0 };

	return get_word_bit(v, bit) == 0x00 ? led_off : led_on;
}


BAFFA1_DWORD get_mem_bit(long v, int bit) {
	if (bit == 0)
		return v & 0b0000000000000000000001;

	return (v & (0b0000000000000000000001 << bit)) >> bit;
}


SDL_Color getColorMem(BAFFA1_DWORD v, int bit) {
	SDL_Color led_on = { 255, 0,  0 };
	SDL_Color led_off = { 90, 0,  0 };

	return get_mem_bit(v, bit) == 0x00 ? led_off : led_on;
}


typedef struct {
	SDL_Rect draw_rect;    // dimensions of button
	struct {
		Uint8 r, g, b, a;
	} colour;

	bool pressed;
} button_t;

static bool button(SDL_Renderer *r, button_t *btn) {
	// draw button
	SDL_SetRenderDrawColor(r, btn->colour.r, btn->colour.g, btn->colour.b, btn->colour.a);
	SDL_RenderFillRect(r, &btn->draw_rect);

	// if button press detected - reset it so it wouldn't trigger twice
	if (btn->pressed) {
		btn->pressed = false;
		return true;
	}
	return false;
}


static void button_process_event(button_t *btn, const SDL_Event *ev) {
	// react on mouse click within button rectangle by setting 'pressed'
	if (ev->type == SDL_MOUSEBUTTONDOWN) {
		if (ev->button.button == SDL_BUTTON_LEFT &&
			ev->button.x >= btn->draw_rect.x &&
			ev->button.x <= (btn->draw_rect.x + btn->draw_rect.w) &&
			ev->button.y >= btn->draw_rect.y &&
			ev->button.y <= (btn->draw_rect.y + btn->draw_rect.h)) {
			btn->pressed = true;
		}
	}
}

#include "BAFFA1_ALU_BUS.h"
#include "baffa1_controller_bus.h"
int main(int argc, char** argv) {



	if (PANEL_ENABLED == 0x00) {

		BAFFA1_COMPUTER baffa1_computer;

		baffa1_computer.init();
		baffa1_computer.run();

	}
	else {

		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_Window* window = SDL_CreateWindow(
			EMULATOR_WINDOW_TITLE,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			800,
			600,
			SDL_WINDOW_SHOWN
		);

		//TTF_Init();
		//TTF_Font* sans = TTF_OpenFont("arial.ttf", 24); //this opens a font style and sets a size

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		Uint64 NOW = SDL_GetPerformanceCounter();
		Uint64 LAST = 0;


		SDL_Surface * image = IMG_Load("painel.png");

		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);


		BAFFA1_COMPUTER baffa1_computer;

#ifdef __MINGW32__
		pthread_t tid;
		pthread_create(&tid, NULL, run_thread, (void *)&z80);
#elif _MSC_VER        
		DWORD tid;
		HANDLE myHandle = CreateThread(0, 0, run_thread, (void *)&baffa1_computer, 0, &tid);
#endif


		while (1) {

			int i = 0;


			button_t t_reset;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.cpu_reset, 0) == 0x00)
				t_reset = { {540,300,20,40}, {128,128,128,128} };
			else
				t_reset = { {540,300,20,40}, {66, 66, 255, 255} };

			button_t t_restart;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.cpu_restart, 0) == 0x00)
				t_restart = { {60,110,20,40}, {128,128,128,128} };
			else
				t_restart = { {60,110,20,40}, {66, 66, 255, 255} };


			button_t t_regsel[5];

			for (i = 4; i >= 0; i--)
				if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_regsel, i) == 0x00)
					t_regsel[i] = { {310 + (30 * (4 - i)),410,20,40}, {128,128,128,128} };
				else
					t_regsel[i] = { {310 + (30 * (4 - i)),410,20,40} , {66, 66, 255, 255} };


			button_t t_mem_io;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_mem_io, 0) != 0x00)
				t_mem_io = { {690,300,20,40}, {128,128,128,128} };
			else
				t_mem_io = { {690,300,20,40}, {66, 66, 255, 255} };

			button_t t_wr;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_wr, 0) == 0x00)
				t_wr = { {660,300,20,40}, {128,128,128,128} };
			else
				t_wr = { {660,300,20,40}, {66, 66, 255, 255} };

			button_t t_rd;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_rd, 0) == 0x00)
				t_rd = { {630,300,20,40}, {128,128,128,128} };
			else
				t_rd = { {630,300,20,40}, {66, 66, 255, 255} };

			button_t t_dma_req;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.dma_req, 0) == 0x00)
				t_dma_req = { {600,300,20,40}, {128,128,128,128} };
			else
				t_dma_req = { {600,300,20,40}, {66, 66, 255, 255} };



			button_t t_address_value[22];

			for (i = 21; i >= 0; i--)
				if (get_mem_bit(baffa1_computer.cpu.controller_bus.panel_address, i) == 0x00)
					t_address_value[i] = { {60 + (30 * (21 - i)),520,20,40}, {128,128,128,128} };
				else
					t_address_value[i] = { {60 + (30 * (21 - i)),520,20,40} , {66, 66, 255, 255} };



			button_t t_data_value[8];

			for (i = 7; i >= 0; i--)
				if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_data, i) == 0x00)
					t_data_value[i] = { {480 + (30 * (7 - i)),410,20,40}, {128,128,128,128} };
				else
					t_data_value[i] = { {480 + (30 * (7 - i)),410,20,40} , {66, 66, 255, 255} };


			button_t t_int_req[8];

			for (i = 0; i <= 7; i++)
				if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_req, i) == 0x00)
					t_int_req[i] = { {480 + (30 * (i)),230,20,40}, {128,128,128,128} };
				else
					t_int_req[i] = { {480 + (30 * (i)),230,20,40} , {66, 66, 255, 255} };

			//button_t t_int_request;

			/*
			if (get_byte_bit(baffa1_computer.cpu.controller_bus.int_request, 0) == 0x00)
				t_int_request = { {450,230,20,40}, {128,128,128,128} };
			else
				t_int_request = { {450,230,20,40} , {66, 66, 255, 255} };
				*/

			button_t t_wait;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.wait, 0) == 0x00)
				t_wait = { {330,230,20,40}, {128,128,128,128} };
			else
				t_wait = { {330,230,20,40}, {66, 66, 255, 255} };

			button_t t_ext_input;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.ext_input, 0) == 0x00)
				t_ext_input = { {360,230,20,40}, {128,128,128,128} };
			else
				t_ext_input = { {360,230,20,40}, {66, 66, 255, 255} };



			button_t t_panel_microcodestep;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_microcodestep, 0) == 0x00)
				t_panel_microcodestep = { {390,230,20,40}, {128,128,128,128} };
			else
				t_panel_microcodestep = { {390,230,20,40}, {66, 66, 255, 255} };

			button_t t_panel_run;

			if (get_byte_bit(baffa1_computer.cpu.controller_bus.panel_run, 0) == 0x00)
				t_panel_run = { {420,230,20,40}, {128,128,128,128} };
			else
				t_panel_run = { {420,230,20,40}, {66, 66, 255, 255} };



			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				// quit on close, window close, or 'escape' key hit
				if (event.type == SDL_QUIT ||
					(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) ||
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {

					SDL_DestroyTexture(texture);
					SDL_FreeSurface(image);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					return 0;
				}



				for (i = 4; i >= 0; i--)
					button_process_event(&t_regsel[i], &event);

				button_process_event(&t_mem_io, &event);
				button_process_event(&t_wr, &event);
				button_process_event(&t_rd, &event);
				button_process_event(&t_dma_req, &event);


				for (i = 7; i >= 0; i--)
					button_process_event(&t_int_req[i], &event);


				for (i = 7; i >= 0; i--)
					button_process_event(&t_data_value[i], &event);

				for (i = 21; i >= 0; i--)
					button_process_event(&t_address_value[i], &event);


				button_process_event(&t_reset, &event);
				button_process_event(&t_restart, &event);

				button_process_event(&t_wait, &event);
				button_process_event(&t_ext_input, &event);

				//button_process_event(&t_int_request, &event);

				button_process_event(&t_panel_microcodestep, &event);
				button_process_event(&t_panel_run, &event);

			}

			SDL_SetRenderDrawColor(renderer, 200, 200, 255, 0);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

			SDL_RenderCopy(renderer, texture, NULL, NULL);



			long  address_bus = baffa1_computer.cpu.memory_bios.read_address_bus(
				baffa1_computer.cpu.controller_bus,
				baffa1_computer.cpu.alu.MSWl.value(),
				baffa1_computer.cpu.registers.MARl.value(),
				baffa1_computer.cpu.registers.MARh.value(),
				baffa1_computer.cpu.registers.MDRl.value(),
				baffa1_computer.cpu.registers.MDRh.value()
				);

			
			BAFFA1_BYTE data_bus = baffa1_computer.cpu.data_bus.data;
			BAFFA1_BYTE ir = baffa1_computer.cpu.microcode.IR.value();
			BAFFA1_BYTE w = baffa1_computer.cpu.registers.registers_bus.w_bus;


			BAFFA1_BYTE marl = baffa1_computer.cpu.registers.MARl.value();
			BAFFA1_BYTE marh = baffa1_computer.cpu.registers.MARh.value();

			BAFFA1_BYTE flags = baffa1_computer.cpu.alu.MSWh.value();
			BAFFA1_BYTE status = baffa1_computer.cpu.alu.MSWl.value();

			BAFFA1_BYTE int_status = baffa1_computer.cpu.interruptions_bus.int_status;


			draw_circle(renderer, 70 + (30 * 0), 200, 10, getColor(flags, 0));
			draw_circle(renderer, 70 + (30 * 1), 200, 10, getColor(flags, 1));
			draw_circle(renderer, 70 + (30 * 2), 200, 10, getColor(flags, 2));
			draw_circle(renderer, 70 + (30 * 3), 200, 10, getColor(flags, 3));
			draw_circle(renderer, 70 + (30 * 4), 200, 10, getInvColor(baffa1_computer.buffer_rd(), 0));
			draw_circle(renderer, 70 + (30 * 5), 200, 10, getInvColor(baffa1_computer.buffer_wr(), 0));
			draw_circle(renderer, 70 + (30 * 6), 200, 10, getInvColor(baffa1_computer.buffer_mem_io(), 0));
			draw_circle(renderer, 70 + (30 * 9), 200, 10, getColor(baffa1_computer.cpu.controller_bus.wait, 0));
			draw_circle(renderer, 70 + (30 * 10), 200, 10, getColor(baffa1_computer.cpu.controller_bus.ext_input, 0));

			draw_circle(renderer, 70 + (30 * 17), 330, 10, getColor(baffa1_computer.cpu.controller_bus.clk, 0)); //clk

			draw_circle(renderer, 70 + (30 * 11), 200, 10, getColor(baffa1_computer.cpu.controller_bus.panel_microcodestep, 0));
			if (button(renderer, &t_panel_microcodestep)) {
				//printf("\npanel_microcodestep\n");
				baffa1_computer.cpu.controller_bus.panel_microcodestep = get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_microcodestep), 0);
			}
			draw_circle(renderer, 70 + (30 * 12), 200, 10, getColor(baffa1_computer.cpu.controller_bus.panel_run, 0));
			if (button(renderer, &t_panel_run)) {
				//printf("\npanel_run\n");

				baffa1_computer.cpu.controller_bus.panel_run = get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_run), 0);
			}


			if (button(renderer, &t_reset)) {
				//printf("\nreset\n");

				baffa1_computer.cpu.controller_bus.cpu_reset = get_byte_bit((~baffa1_computer.cpu.controller_bus.cpu_reset), 0);
			}
			if (button(renderer, &t_restart)) {
				//printf("\nrestart\n");

				baffa1_computer.cpu.controller_bus.cpu_restart = get_byte_bit((~baffa1_computer.cpu.controller_bus.cpu_restart), 0);
			}

			if (button(renderer, &t_wait)) {
				//printf("\nwait\n");

				baffa1_computer.cpu.controller_bus.wait = get_byte_bit((~baffa1_computer.cpu.controller_bus.wait), 0);
			}

			if (button(renderer, &t_ext_input)) {
				//printf("\next_input\n");

				baffa1_computer.cpu.controller_bus.ext_input = get_byte_bit((~baffa1_computer.cpu.controller_bus.ext_input), 0);
			}



			draw_circle(renderer, 460, 200, 10, getColor(baffa1_computer.cpu.interruptions_bus.int_request, 0));
			draw_circle(renderer, 460, 250, 10, getColor(baffa1_computer.cpu.controller_bus.int_ack, 0));

			/*if (button(renderer, &t_int_request)) {
				//printf("\nint_request\n");

				baffa1_computer.cpu.controller_bus.int_request = get_byte_bit((~baffa1_computer.cpu.controller_bus.int_request), 0);
			}
			*/


			draw_circle(renderer, 70 + (30 * 0), 250, 10, getColor(status, 1));
			draw_circle(renderer, 70 + (30 * 1), 250, 10, getColor(status, 2));
			draw_circle(renderer, 70 + (30 * 2), 250, 10, getColor(status, 3));
			draw_circle(renderer, 70 + (30 * 3), 250, 10, getColor(status, 4));
			draw_circle(renderer, 70 + (30 * 4), 250, 10, getColor(status, 5));
			//draw_circle(renderer, 20 + (20 * 6), 250, 10, getColor(status, 6));
			draw_circle(renderer, 70 + (30 * 5), 250, 10, getColor(status, 7));


			draw_circle(renderer, 70 + (30 * 7), 200, 10, getInvColor(baffa1_computer.cpu.controller_bus.page_writable, 0));
			draw_circle(renderer, 70 + (30 * 7), 250, 10, getInvColor(baffa1_computer.cpu.controller_bus.page_present, 0));


			draw_circle(renderer, 70 + (30 * 8), 200, 10, getColor(baffa1_computer.cpu.controller_bus.dma_req, 0));
			draw_circle(renderer, 70 + (30 * 8), 250, 10, getColor(status, 0));


			for (i = 0; i <= 7; i++) {
				draw_circle(renderer, 490 + (30 * i), 200, 10, getColor(int_status, i));


				if (button(renderer, &t_int_req[i])) {
					//printf("\nint_req_%d\n", i);

					/*
					baffa1_computer.cpu.controller_bus.panel_req =
						(baffa1_computer.cpu.controller_bus.panel_req & (0b11111111 & (~(0x00000001 << i)))) |
						(get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_req), i) << i);
						*/

					baffa1_computer.cpu.interruptions_bus.int_req =
						(baffa1_computer.cpu.interruptions_bus.int_req & (0b11111111 & (~(0x00000001 << i)))) |
						(get_byte_bit((~baffa1_computer.cpu.interruptions_bus.int_req), i) << i);
				}


			}


			for (i = 4; i >= 0; i--)
				if (button(renderer, &t_regsel[i])) {
					//printf("\nreg_sel_%d\n", i);

					baffa1_computer.cpu.controller_bus.panel_regsel =
						(baffa1_computer.cpu.controller_bus.panel_regsel & (0b11111 & (~(0x00001 << i)))) |
						(get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_regsel), i) << i);
				}

			if (button(renderer, &t_mem_io)) {
				//printf("\npanel mem io\n");
				baffa1_computer.cpu.controller_bus.panel_mem_io = get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_mem_io), 0);
			}

			if (button(renderer, &t_wr)) {
				//printf("\npanel wr\n");
				baffa1_computer.cpu.controller_bus.panel_wr = get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_wr), 0);
			}

			if (button(renderer, &t_rd)) {
				//printf("\npanel rd\n");
				baffa1_computer.cpu.controller_bus.panel_rd = get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_rd), 0);
			}

			if (button(renderer, &t_dma_req)) {
				//printf("\ndma req\n");
				baffa1_computer.cpu.controller_bus.dma_req = get_byte_bit((~baffa1_computer.cpu.controller_bus.dma_req), 0);
			}


			for (i = 7; i >= 0; i--)
				draw_circle(renderer, 70 + (30 * (7 - i)), 330, 10, getColor(marh, i));

			for (i = 7; i >= 0; i--)
				draw_circle(renderer, 310 + (30 * (7 - i)), 330, 10, getColor(marl, i));


			for (i = 7; i >= 0; i--)
				draw_circle(renderer, 70 + (30 * i), 380, 10, getColor(ir, 7 - i));


			for (i = 7; i >= 0; i--)
				draw_circle(renderer, 70 + (30 * i), 430, 10, getColor(w, 7 - i));


			for (i = 7; i >= 0; i--) {

				draw_circle(renderer, 490 + (30 * (7 - i)), 380, 10, getColor(data_bus, i));

				if (button(renderer, &t_data_value[i])) {
					//printf("\ndata_value_%d\n", i);

					baffa1_computer.cpu.controller_bus.panel_data =
						(baffa1_computer.cpu.controller_bus.panel_data & (0b11111111 & (~(0x1 << i)))) |
						(get_byte_bit((~baffa1_computer.cpu.controller_bus.panel_data), i) << i);
				}
			}


			for (i = 21; i >= 0; i--) {

				draw_circle(renderer, 70 + (30 * (21 - i)), 490, 10, getColorMem(address_bus, i));

				if (button(renderer, &t_address_value[i])) {
					//printf("\naddress_value_%d\n", i);

					baffa1_computer.cpu.controller_bus.panel_address =
						(baffa1_computer.cpu.controller_bus.panel_address & (0b1111111111111111111111 & (~(0x1 << i)))) |
						(get_mem_bit((~baffa1_computer.cpu.controller_bus.panel_address), i) << i);
				}
			}
			SDL_RenderPresent(renderer);


			if (baffa1_computer.cpu.controller_bus.bus_tristate(baffa1_computer.cpu.alu.MSWl.value()) != 0x00) {
				baffa1_computer.cpu.data_bus.data = baffa1_computer.cpu.controller_bus.panel_data;


				// aqui verificar
				baffa1_computer.cpu.interruptions_bus.int_req = baffa1_computer.cpu.interruptions_bus.int_req | baffa1_computer.cpu.controller_bus.panel_req;
			}
		}

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

	}
	return 0;
}


#endif