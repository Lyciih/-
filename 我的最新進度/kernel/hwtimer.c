#include"os.h"


#define TIMER_INTERVAL 10000000

void timer_load(int interval)
{
    int id = r_mhartid();
    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init(void)
{    
    timer_load(TIMER_INTERVAL);
    w_mie(r_mie() | MIE_MTIE);
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}



int seconds = 0;
int mintues = 0;
int hours = 0;



void tick()
{
    seconds++;
    if(seconds == 60)
    {
        mintues++;
        seconds = 0;
        if(mintues == 60)
        {
            hours++;
            mintues = 0;
            if(hours == 24)
            {
                hours = 0;
            }
        }
    }
}


void clock()
{
    if(hours < 10)
    {printf("0%d : ", hours);}else{printf("%d : ", hours);}

    if(mintues < 10)
    {printf("0%d : ", mintues);}else{printf("%d : ", mintues);}

    if(seconds < 10)
    {printf("0%d ", seconds);}else{printf("%d ", seconds);}

    printf("\n");
}

void timer_handle(void)
{
    tick();
    timer_load(TIMER_INTERVAL);
}