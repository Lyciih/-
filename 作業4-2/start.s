.global _start          #讓linker可以找到_start

_start:                 #這是編譯器所認的程式進入點名字
    csrr t0, mhartid    #將控制狀態暫存器的hartid讀入t0暫存器
    bnez t0, stop       #因為我們目前只用第0顆CPU，所以如果ID判斷不為0則跳入stop程式


    slli t0, t0, 10     #ID為0的CPU繼續執行這一段，我們的設每顆CPU的記憶體空間為1024，
                        #為了讓日後多顆CPU的堆疊指針起始都可以在自己CPU所擁有的記憶體空間
                        #我們必須取得每顆CPU專屬空間的起始位置
                        #slli將自己往左偏移10bit，等同於乘以1024的效果
                        #(此處為第0顆CPU，乘以1024還是0，是第0顆CPU記憶體的起始位置)
    
    

                        #ID為0的CPU繼續執行這一段，因為我們的設每顆CPU的記憶體空間為1024，
    la  sp, stacks      #stacks這個label將跳到下面的stacks副程式。la這個指令將stacks運作的起始位置返回給sp
    addi sp, sp, 1024   #而堆疊是由數字大的位址開始往下，因此我們要把堆疊指針sp先偏移1024
 
 

    add sp, sp, t0      #將t0(當前CPU記憶體起始位置)加上sp(堆疊指針的相對位置)=該CPU堆疊指針真實的位置
    j main              #跳到kernel(hello.c)中的main
    


stop:                   #ID不是0的CPU會跳來這裡
    wfi                 #這是讓CPU休眠的指令
    j   stop            #如果CPU因為中斷被喚醒，讓它繼續回去睡



stacks:                 #建立記憶體空間的程式(類似開檔)
    .skip 1024 * 8      #每顆CPU給1024，共有8顆，乘以8 
    .end                #結束開檔