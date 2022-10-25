CC = riscv64-unknown-elf-gcc
FLAG = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g
DUMP = riscv64-unknown-elf-objdump
QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none
GDB = riscv64-unknown-elf-gdb

#開機程式在編譯a.out時一定要放在第一個
SRCS_ASM = \
	start.s \
	memory.s \

SRCS_C = \
	hello.c \
	uart.c \
	printf.c \
	page.c \
	malloc.c \

OBJS = $(SRCS_ASM:.s=.o)
OBJS += $(SRCS_C:.c=.o)


all: a.out 

#start.o memory.o hello.o uart.o printf.o page.o malloc.o
a.out: $(OBJS)
	$(CC) $(FLAG) -T myos.ld -g $^


%.o : %.c
	$(CC) $(FLAG) -c -o $@ $<

%.o : %.s
	$(CC) $(FLAG) -c -o $@ $<





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
