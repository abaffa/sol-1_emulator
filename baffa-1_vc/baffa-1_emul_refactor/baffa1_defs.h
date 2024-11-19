#ifndef DEF_H
#define DEF_H

#define BAFFA1_BYTE				unsigned char // 8bit
#define BAFFA1_MWORD				unsigned int //Memory Word (int 32bit, short 16bit, char 8bit)
#define BAFFA1_DWORD				unsigned int //Data Word

// FLAG - msw-h - bits
#define MSWh_ZF 0x00 // ZeroFlag
#define MSWh_CF 0x01 // Carry Flag
#define MSWh_SF 0x02 // Overflow Flag
#define MSWh_OF 0x03 // Sign Flag
#define MSWh_12 0x04
#define MSWh_13 0x05
#define MSWh_14 0x06
#define MSWh_15 0x07

// STATUS FLAGS - msw-l bits
#define MSWl_DMA_ACK 0x00
#define MSWl_INTERRUPT_ENABLE 0x01
#define MSWl_CPU_MODE 0x02
#define MSWl_PAGING_EN 0x03
#define MSWl_HALT 0x04
#define MSWl_DISPLAY_REG_LOAD 0x05
#define MSWl_14 0x06
#define MSWl_DIR 0x07

#endif