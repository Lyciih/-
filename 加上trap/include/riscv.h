#ifndef __RISCV_H_
#define __RISCV_H_

#include"types.h"

static inline void w_mtvec(reg_t x)
{
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

#endif