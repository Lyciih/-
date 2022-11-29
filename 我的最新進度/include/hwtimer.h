#ifndef __HWTIMER_H_
#define __HWTIMER_H_

#define TIMER_INTERVAL 10000000
#define CLINT_BASE 0x2000000L
#define CLINT_MTIME (CLINT_BASE + 0xBFF8)
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))


#endif