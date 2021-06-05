#ifndef SOL1CPU_H
#define SOL1CPU_H

#include "config.h"
#include "sol1_rom.h"
#include "sol1_memory.h"
#include "sol1_microcode.h"
#include "sol1_registers.h"
#include "sol1_alu.h"
#include "hw_tty.h"
#include <stddef.h>

class SOL1_CPU
{

public:

	SOL1_MEMORY memory;
	SOL1_REGISTERS registers;
	SOL1_MICROCODE microcode;
	SOL1_ROM rom;

	struct sol1_alu_8bit alu;

	char last_op_desc[512];

	SOL1_MWORD BKPT; //breakpoint (FFFF = disable)

	int DEBUG_MICROCODE;
	int DEBUG_UADDRESSER;
	int DEBUG_UADDER;
	int DEBUG_UFLAGS;
	
	int DEBUG_BUSES;
	int DEBUG_ALU;

	int DEBUG_RDREG;
	int DEBUG_WRREG;
	int DEBUG_REGISTERS;


	int DEBUG_RDMEM;
	int DEBUG_WRMEM;
	int DEBUG_MEMORY;

	int DEBUG_UART;
	int DEBUG_IDE;
	int DEBUG_RTC;
	int DEBUG_TIMER;

	int DEBUG_LOG_OPCODE;
	int DEBUG_LITE;
	int DEBUG_LITE_CYCLES;
	int SERVER;
	int WEB_SERVER;

	int display_reg_load;


	void init(HW_TTY& hw_tty);

	void reset();
	void display_registers(HW_TTY& hw_tty);
	void display_registers_lite(HW_TTY& hw_tty);
	void memory_display(HW_TTY& hw_tty);


	void mswh_flags_desc(HW_TTY& hw_tty);
	void mswl_status_desc(HW_TTY& hw_tty);

	void load_microcode_from_rom();




	SOL1_BYTE refresh_MSWh_ZF(SOL1_BYTE z_bus, SOL1_BYTE zf_in_src);
	SOL1_BYTE refresh_MSWh_CF(SOL1_BYTE z_bus, SOL1_BYTE cf_in_src);
	SOL1_BYTE refresh_MSWh_SF(SOL1_BYTE z_bus, SOL1_BYTE sf_in_src);
	SOL1_BYTE refresh_MSWh_OF(SOL1_BYTE z_bus, SOL1_BYTE u_sf, SOL1_BYTE of_in_src);


	SOL1_BYTE *get_current_memory();
	int get_current_memory_size();

	void mc_seq_update(HW_TTY& hw_tty);

};
#endif