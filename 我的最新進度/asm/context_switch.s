.macro reg_save base
	sw ra, 0(\base)
	sw sp, 4(\base)
	sw gp, 8(\base)
	sw tp, 12(\base)
	sw t0, 16(\base)
	sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
.endm

.macro reg_restore base
	lw ra, 0(\base)
	lw sp, 4(\base)
	lw gp, 8(\base)
	lw tp, 12(\base)
	lw t0, 16(\base)
	lw t1, 20(\base)
	lw t2, 24(\base)
	lw s0, 28(\base)
	lw s1, 32(\base)
	lw a0, 36(\base)
	lw a1, 40(\base)
	lw a2, 44(\base)
	lw a3, 48(\base)
	lw a4, 52(\base)
	lw a5, 56(\base)
	lw a6, 60(\base)
	lw a7, 64(\base)
	lw s2, 68(\base)
	lw s3, 72(\base)
	lw s4, 76(\base)
	lw s5, 80(\base)
	lw s6, 84(\base)
	lw s7, 88(\base)
	lw s8, 92(\base)
	lw s9, 96(\base)
	lw s10, 100(\base)
	lw s11, 104(\base)
	lw t3, 108(\base)
	lw t4, 112(\base)
	lw t5, 116(\base)
	lw t6, 120(\base)
.endm




.text


.globl sys_switch				#從kernel跳到task的程式
.align 4
sys_switch:						# a0 代表第一個參數，是指向第一個task的context的指針
	csrw	mscratch, a0		#把 a0 也寫入暫存用的 mscratch 暫存器，讓等下的task可以拿到自己context的指針
	mv	t6, a0					#把 a0 寫入t6，用來當作載入第一個task的context的基址			
	reg_restore t6				#利用 t6 中的地址載入第一個task的context
    ret							#用ra覆蓋PC，跳到task的起始位置開始執行


.globl switch_to				#從task跳到task的程式
.align 4
switch_to:
	csrrw	t6, mscratch, t6	#用 t6 與 mscratch 做交換，得到自己context的地址
	reg_save t6					#利用 t6 中的地址儲當前的暫存器內容到自己的context
	mv	t5, t6					#因為 t5 已經存過了，複寫沒關係，把 t6 中的地址寫到t5中
	csrr	t6, mscratch		#把 t6 跟 mscratch 交換前的值換回來
	sw	t6, 120(t5)				#利用 t5 中的地址儲存換回來的 t6 到自己context的 t6 位置

								#以上完成存context的動作，下一階段開始載入目標task的context

								# a0 代表第一個參數，是指向下一個task的context的指針
	csrw	mscratch, a0		#把 a0 寫入暫存用的 mscratch 暫存器，讓下一個task要離開時可以拿到自己context的指針
	mv	t6, a0					#把 a0 寫入t6
	reg_restore t6				#用 t6 中的地址載入下一個task的context
    ret							#用ra覆蓋PC，跳到下一個task的起始位置開始執行
.end


