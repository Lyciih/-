#ifndef __HWTIMER_H_
#define __HWTIMER_H_

//每次發生時間中斷的間隔
#define TIMER_INTERVAL 10000000
//存取 CLINT 暫存器的基址
#define CLINT_BASE 0x2000000L
//存取 CLINT 中時鐘暫存器的地址
#define CLINT_MTIME (CLINT_BASE + 0xBFF8)
//存取 CLINT 中用來比較時間的暫存器的地址
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))


#endif