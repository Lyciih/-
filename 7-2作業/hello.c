extern void uart_init(void);            //extern void 告訴編譯器 uart_init(void) 函數在其他檔案已被宣告
extern void uart_puts(char* s);         //extern void 告訴編譯器 uart_puts(char* s) 函數在其他檔案已被宣告
extern void uart_r();

void main(void)
{
    uart_init();                        //執行uart_init函數初始化UART
    uart_puts("hello\n");               //使用uart_puts函數傳輸 hello 字串
    uart_puts("please input\n");
    uart_r();
    
    while (1) {};                       //進入迴圈
}