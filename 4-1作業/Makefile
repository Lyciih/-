CC = riscv64-unknown-elf-gcc
FLAG = -march=rv32ima -mabi=ilp32
CFLAGS = -nostdlib -fno-builtin -mcmodel=medany -march=rv32ilsa -mabi=ilp32
DUMP = riscv64-unknown-elf-objdump

all: hello.o
hello.o:
	$(CC) $(FLAG) -c -g hello.c


clean:
	rm -f *.o
	rm -f *.exe

dump:
	$(DUMP) -S hello.o
