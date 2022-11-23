#include"os.h"

extern void trap_vector(void);

void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;

    if(cause & 0x80000000)
    {
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n");
            break;

        case 7:
            uart_puts("timer interruption!\n");
            break;

        case 11:
            uart_puts("external interruption!\n");
            break;
        

        default:
            uart_puts("unknown async exception!\n");
            break;
        }
    }
    else
    {        
        printf("Sync exceptions!, code = %d\n", cause_code);
        uart_puts("OOPS! What can I do!\n");
        uart_puts("next time, this task will start at pc where exeception next\n");
        return_pc += 4;
    }
    return return_pc;
}


void trap_test()
{
	*(int *)0x00000000 = 100;
	uart_puts("Yeah! I'm return back from trap!\n");
}