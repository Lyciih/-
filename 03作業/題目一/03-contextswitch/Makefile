#因為把這個資料夾獨立拉出來，所以把之前include的上層common內容直接貼過來
CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g
QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none
GDB = riscv64-unknown-elf-gdb
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump


#把最後的編譯目標修改為objs資料夾底下的.o檔
OBJS = \
	./objs/start.o \
	./objs/mem.o \
	./objs/entry.o \
	./objs/kernel.o \
	./objs/uart.o \
	./objs/printf.o \
	./objs/page.o \
	./objs/sched.o \


.DEFAULT_GOAL := all
all: os.elf

#設定各個資料夾下的目標與依賴關係
os.elf: ${OBJS}
	${CC} ${CFLAGS} -T os.ld -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

./objs/%.o : ./startup/%.s
	$(CC) $(CFLAGS) -c -o $@ $<

./objs/%.o : ./asm/%.s
	$(CC) $(CFLAGS) -c -o $@ $<

./objs/%.o : ./kernel/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

./objs/%.o : ./lib/%.c
	$(CC) $(CFLAGS) -c -o $@ $<





run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean

#將objs/*.o 加到clean 偽目標中
clean:
	rm -rf *.o *.bin *.elf objs/*.o

