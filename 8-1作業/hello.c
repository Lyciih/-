extern void uart_init(void);            //extern void 告訴編譯器 uart_init(void) 函數在其他檔案已被宣告
extern void uart_puts(char* s);         //extern void 告訴編譯器 uart_puts(char* s) 函數在其他檔案已被宣告
extern void uart_r(void);                   //extern void 告訴編譯器 uart_puts(char* s) 函數在其他檔案已被宣告

extern void page_init(void);
extern void page_test(void);
extern void malloc_init(void);
extern void * malloc(int nbytes);
extern void free(void *p);

void main(void)
{
    int i;
    uart_init();                        //執行uart_init函數初始化UART
    uart_puts("hello\n");               //使用uart_puts函數傳輸 hello 字串
    uart_puts("please input\n");        //使用uart_puts函數傳輸 please input 字串

    page_init();
    page_test();
    malloc_init();
    void *test1 = malloc(4);
    void *test2 = malloc(1);
    void *test3 = malloc(2);
    void *test4 = malloc(3);
    void *test5 = malloc(2);
    void *test6 = malloc(7);
    void *test7 = malloc(8);

    free(test1);
    free(test2);
    free(test3);
    free(test4);
    free(test5);
    free(test6);
    free(test7);
    
    uart_r();                           //使用uart_r函數進入輪詢狀態，取得用戶輸入的字，並將它回傳到用戶的終端機
    


    
    while (1) {};                       //進入迴圈
}