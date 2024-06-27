INCLUDES= -I ./include
FLAGS = -O3 -std=c++11
#FLAGS2 = -g

OBJECTS=./build/utils.o ./build/tasm_opcode.o ./build/baffa1_rom.o ./build/baffa1_register_8bit.o ./build/baffa1_registers.o ./build/baffa1_microcode.o ./build/baffa1_memory.o ./build/baffa1_alu_4bit.o ./build/baffa1_alu.o ./build/queue_ll.o ./build/linkedlist.o ./build/hw_web.o ./build/hw_uart.o ./build/hw_tty.o ./build/hw_timer.o ./build/hw_rtc.o ./build/hw_ide.o ./build/debugmenu_roms.o ./build/debugmenu_main.o ./build/baffa1_cpu.o ./build/baffa1_bus.o ./build/baffa1_computer.o

all: ${OBJECTS} 
	gcc ${FLAGS} ${INCLUDES} ./src/main.cpp ${OBJECTS} -L ./lib -lmingw32 -lstdc++ -lSDL2main -lSDL2 -lm -lncurses -pthread -o ./bin/main.exe	

./build/baffa1_computer.o:src/baffa1_computer.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_computer.cpp -c -o ./build/baffa1_computer.o

./build/baffa1_bus.o:src/baffa1_bus.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_bus.cpp -c -o ./build/baffa1_bus.o

./build/baffa1_cpu.o:src/baffa1_cpu.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_cpu.cpp -c -o ./build/baffa1_cpu.o

./build/debugmenu_main.o:src/debugmenu_main.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/debugmenu_main.cpp -c -o ./build/debugmenu_main.o

./build/debugmenu_roms.o:src/debugmenu_roms.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/debugmenu_roms.cpp -c -o ./build/debugmenu_roms.o

./build/hw_ide.o:src/hw_ide.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_ide.cpp -c -o ./build/hw_ide.o

./build/hw_rtc.o:src/hw_rtc.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_rtc.cpp -c -o ./build/hw_rtc.o

./build/hw_timer.o:src/hw_timer.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_timer.cpp -c -o ./build/hw_timer.o

./build/hw_tty.o:src/hw_tty.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_tty.cpp -c -o ./build/hw_tty.o

./build/hw_uart.o:src/hw_uart.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_uart.cpp -c -o ./build/hw_uart.o

./build/hw_web.o:src/hw_web.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/hw_web.cpp -c -o ./build/hw_web.o

./build/linkedlist.o:src/linkedlist.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/linkedlist.cpp -c -o ./build/linkedlist.o

./build/queue_ll.o:src/queue_ll.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/queue_ll.cpp -c -o ./build/queue_ll.o

./build/baffa1_alu.o:src/baffa1_alu.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_alu.cpp -c -o ./build/baffa1_alu.o

./build/baffa1_alu_4bit.o:src/baffa1_alu_4bit.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_alu_4bit.cpp -c -o ./build/baffa1_alu_4bit.o

./build/baffa1_memory.o:src/baffa1_memory.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_memory.cpp -c -o ./build/baffa1_memory.o

./build/baffa1_microcode.o:src/baffa1_microcode.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_microcode.cpp -c -o ./build/baffa1_microcode.o

./build/baffa1_registers.o:src/baffa1_registers.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_registers.cpp -c -o ./build/baffa1_registers.o

./build/baffa1_register_8bit.o:src/baffa1_register_8bit.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_register_8bit.cpp -c -o ./build/baffa1_register_8bit.o

./build/baffa1_rom.o:src/baffa1_rom.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/baffa1_rom.cpp -c -o ./build/baffa1_rom.o

./build/tasm_opcode.o:src/tasm_opcode.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/tasm_opcode.cpp -c -o ./build/tasm_opcode.o

./build/utils.o:src/utils.cpp
	gcc ${FLAGS} ${INCLUDES} ./src/utils.cpp -c -o ./build/utils.o


clean:
	del build\* /q
	del bin\main.exe /q
	del bin\main2.exe /q
	del bin\test.exe /q