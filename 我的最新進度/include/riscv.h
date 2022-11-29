#ifndef __RISCV_H_
#define __RISCV_H_

#include"types.h"

static inline reg_t r_mhartid()
{
    
    reg_t x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
    printf("123\n");
}


static inline void w_mtvec(reg_t x)
{
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

static inline reg_t r_tp()
{
    reg_t x;
    asm volatile("mv %0, tp" : "=r" (x) );
    return x;
}

#define MSTATUS_MPP (3 << 11)
#define MSTATUS_SPP (1 << 8)

#define MSTATUS_MPIE (1 << 7)
#define MSTATUS_SPIE (1 << 5)
#define MSTATUS_UPIE (1 << 4)

#define MSTATUS_MIE (1 << 3)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_UIE (1 << 0)


static inline void w_mie(reg_t x)
{
    asm volatile("csrw mie, %0" : : "r" (x));
}

static inline reg_t r_mie()
{
    reg_t x;
    asm volatile("csrr %0 , mie" : "=r" (x));
    return x;
}

#define MIE_MEIE (1 << 11)
#define MIE_MTIE (1 << 7)
#define MIE_MSIE (1 << 3)


static inline reg_t r_mstatus()
{
    reg_t x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

static inline void w_mstatus(reg_t x)
{
    asm volatile("csrw mstatus, %0" : : "r" (x));
}



#endif