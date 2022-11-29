#include"os.h"


void main(void)
{
    int i;
    uart_init();                                                    //執行uart_init函數初始化UART
    uart_puts("\n\n-----------  Hello!  ------------------\n");     //使用uart_puts函數傳輸 hello 字串

    printf("\n----- page init -----\n");
    page_init();

    printf("\n----- page test -----\n");
    page_test();

    printf("\n----- malloc init -----\n");
    malloc_init(10);                                                //申請malloc初始化所用到的page數，必須是5的倍數

    printf("\n-----malloc test-----\n");                            //測試malloc跟free的功能是否正常
    void *test1 = malloc(4);
    void *test2 = malloc(1);
    void *test3 = malloc(2);
    void *test4 = malloc(3);
    free(test3);
    void *test5 = malloc(2);
    free(test5);
    void *test6 = malloc(7);
    free(test4);
    free(test6);
    void *test7 = malloc(8);

    free(test1);
    free(test2);   
    free(test7);

    test1 = malloc(4);
    test2 = malloc(1);
    test3 = malloc(2);
    test4 = malloc(3);
    test5 = malloc(2);
    test6 = malloc(7);
    test7 = malloc(8);
    free(test1);
    free(test2);
    free(test3);
    free(test4);
    free(test5);
    free(test6);
    free(test7);

    printf("\n----- malloc test success -----\n");
    printf("\n\n\n----- schedule_init -----\n");

    printf("\n----- trap -----\n");
    trap_init();

    printf("\n----- plic -----\n");
    plic_init();

    printf("\n----- timer -----\n");
    timer_init();

    schedule_init();
    printf("\n\n\n----- schedule start -----\n\n\n");
    schedule();





    printf("\n----- uart polling input -----\n");
    uart_puts("please input , and use Enter to newline , or use exit to quit\n");        //使用uart_puts函數傳輸 please input 字串
    uart_r();                           //使用uart_r函數進入輪詢狀態，取得用戶輸入的字，並將它回傳到用戶的終端機
    


    
    while (1)                           //進入迴圈
    {
        uart_puts("don't go here\n");
    };                       
}