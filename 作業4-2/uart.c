//首先，我們先把會用到的uart暫存器位址define成好用的名字

//Interrupt Enable Register 是用來設定中斷模式的暫存器
#define IER  ((volatile unsigned char *)(0x10000000L + 1))

//Line Control Register 用來控制傳輸的速率、校驗方式、字節長度
#define LCR  ((volatile unsigned char *)(0x10000000L + 3))

//divisor latch less 用來設定最小傳輸速率的暫存器(與Receive Holding Register共用同個位址，只在LCR的第8個bit設為1時才可存取)
#define DLL  ((volatile unsigned char *)(0x10000000L + 0))

//divisor latch most 用來設定最大傳輸速率的暫存器(與Interrupt Enable Register共用同個位址，只在LCR的第8個bit設為1時才可存取)
#define DLM  ((volatile unsigned char *)(0x10000000L + 1))

//Transmit Holding Register用來存放要送出的資料的暫存器，與Receive Holding Register、DLL共用同個位址
#define THR  ((volatile unsigned char *)(0x10000000L + 0))

//Line Status Register 存放uart傳輸狀態的暫存器
#define LSR  ((volatile unsigned char *)(0x10000000L + 5))

//將uart初始化的函數
int uart_init()
{
    
    *IER = 0x00;                    //存取IER，把所有中斷關掉
    *LCR = *LCR | ( 1 << 7 );       //存取LCR，與10000000(二進位)做or運算，把最左邊的bit設為1，打開DLL、DLM的設定模式
    *DLL = 0x03;                    //將DLL設為3，代表38.4k
    *DLM = 0x00;                    //DLM則不設定上限

    *LCR = 0;                       //設定完DLL、DLM，把LCR全部設為0，關閉速率設定模式
    *LCR = *LCR | (3 << 0);         //將LCR設為3(二進位00000011)，代表每次傳輸字長8個bit

}
//將傳輸資料寫入THR的函數
int uart_c(char ch)
{
    while(*LSR & (1 << 5) == 0);    //判斷LSR中記錄傳輸暫存器是否為空的bit，當它為1時離開迴圈往下執行
    return *THR = ch;               //將資料寫入THR
}
//將一整串字串依序寫入THR的函數
int uart_puts(char* s)
{
    while(*s)                       //當字串指針所指的值不為0時(代表還有字沒傳完)就持續執行
    {
        uart_c(*s++); 
    }                  //使用uart_c函數將當前指到的字寫入THR，再將指針往下一個字移動
}
