#include"os.h"

//將比較暫存器 CLINT_MTIMECMP 設置為下次發生中斷的時間
void timer_load(int interval)
{
    int id = r_mhartid();
    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

//初始化下次發生時間中斷的時間，打開時間中斷，打開全域中斷
void timer_init(void)
{    
    timer_load(TIMER_INTERVAL);
    w_mie(r_mie() | MIE_MTIE);
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}


//宣告紀錄時間的參數
int seconds = 0;
int mintues = 0;
int hours = 0;


//時間進位的邏輯
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

//顯示時間的函數
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


//時間中斷處理函數，這裡每一次都加1秒，並且更新下次要發生的時間
void timer_handle(void)
{
    tick();
    timer_load(TIMER_INTERVAL);
}