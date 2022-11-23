#include"os.h"

void plic_init(void)
{
    int hart = r_tp();

    *(uint32_t *)PLIC_PRIORITY(UART0_IRQ) = 1;

    *(uint32_t *)PLIC_MENABLE(hart) = (1 << UART0_IRQ);

    *(uint32_t *)PLIC_MTHRESHOLD(hart) = 0;

    w_mie(r_mie() | MIE_MEIE);

}