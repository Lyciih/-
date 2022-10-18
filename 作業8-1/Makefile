CC = riscv64-unknown-elf-gcc
FLAG = -nostdlib -march=rv32ima -mabi=ilp32 -g
DUMP = riscv64-unknown-elf-objdump
QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none
GDB = riscv64-unknown-elf-gdb


all: a.out 
#開機程式在編譯a.out時一定要放在第一個
a.out: start.o uart.o hello.o 
	$(CC) $(FLAG) -Ttext=0x80000000 -g start.o uart.o hello.o 

start.o: start.S
	$(CC) $(FLAG) -c start.S

hello.o: hello.c
	$(CC) $(FLAG) -c hello.c

uart.o: uart.c
	$(CC) $(FLAG) -c uart.c


.PHONY: clean
clean:
	rm -f *.o
	rm -f *.exe
	rm -f *.out

.PHONY: dump
dump:
	$(DUMP) -D a.out



.PHONY: qemu
qemu:
	${QEMU} ${QFLAGS} -kernel a.out 


.PHONY: gdb
gdb:
	${QEMU} ${QFLAGS} -kernel a.out -s -S &
	$(GDB) a.out -q -x gdbinit