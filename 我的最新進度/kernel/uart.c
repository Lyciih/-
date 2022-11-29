#include"os.h"

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
void uart_init(void)
{
    
    *IER = 0x00;                    //存取IER，把所有中斷關掉
    *LCR = *LCR | ( 1 << 7 );       //存取LCR，與10000000(二進位)做or運算，把最左邊的bit設為1，打開DLL、DLM的設定模式
    *DLL = 0x03;                    //將DLL設為3，代表38.4k
    *DLM = 0x00;                    //DLM則不設定上限

    *LCR = 0;                       //設定完DLL、DLM，把LCR全部設為0，關閉速率設定模式
    *LCR = *LCR | (3 << 0);         //將LCR設為3(二進位00000011)，代表每次傳輸字長8個bit

    *IER = *IER | (1 << 0);
}


//將傳輸資料寫入THR的函數
int uart_putc(char ch)
{
    while(*LSR & (1 << 5) == 0);    //判斷LSR中記錄傳輸暫存器是否為空的bit，當它為1時離開迴圈往下執行
    return *THR = ch;               //將資料寫入THR
}


//將一整串字串依序寫入THR的函數
int uart_puts(char* s)
{
    while(*s)                       //當字串指針所指的值不為0時(代表還有字沒傳完)就持續執行
    {
        uart_putc(*s++); 
    }                  //使用uart_c函數將當前指到的字寫入THR，再將指針往下一個字移動
}


//Transmit Holding Register用來存放要送出的資料的暫存器，與Receive Holding Register、DLL共用同個位址
#define RHR  ((volatile unsigned char *)(0x10000000L + 0))

void uart_r(void)
{
    char ch;
    //因為是使用輪詢的方式，待會會進入無窮迴圈。為了能夠在輸入exit時離開迴圈，用一個字串來储存比對命令的字元
    char temp[255];       
    int count = 0;
    while(1)            //進入無窮輪詢迴圈
    {
        while(*LSR & (1 << 0))        //檢查LSR暫存器的第一個bit是否為1，1代表接收字元完成
        {
            ch = *RHR;                  //讀取RHR中收到的字元到ch變數
            if(ch == '\r')              //如果ch中的字元是 Enter (也就是 \r )
            {
                uart_putc('\n');           //則使用uart_c傳送一個換行符號 ( \n )

                //下面的if，比對用戶輸入 Enter 之前所輸入的字串是不是exit
                if(temp[0] == 'e' && temp[1] == 'x' && temp[2] == 'i' && temp[3] == 't')        
                {
                    //用戶輸入是exit的話，用uart_puts()，傳送 quit 到用戶的螢幕上
                    uart_puts("quit\n"); 
                    //離開迴圈
                    break;                      
                }

                for(int i=1;i<count;i++)        //離開迴圈前，把temp[]裡面的字都清空
                {
                    temp[i] = ' ';
                }
                count = 0;                      //count重置為0，給輸入Enter後的下一行使用
            }
            else                        //如果不是 Enter 符號 ( \r )
            {
                uart_putc(ch);             //就把收到的資料用 uart_c(ch) 傳回使用者的螢幕上
                temp[count] = ch;       //另外也把 ch 存到 temp[255] 中
                count++;                //count做為 temp[255] 的位置計數器，往前進一個，用來存下個字元
            }
        }
    }
}

int uart_getc(void)
{
    if(*LSR & (1 << 0))
    {
        return *RHR;
    }
    else
    {
        return -1;
    }
}


char trap_ch;
char trap_temp[255];
int trap_count = 0;

void uart_isr(void)
{

    while(*LSR & (1 << 0))        //檢查LSR暫存器的第一個bit是否為1，1代表接收字元完成
    {
        trap_ch = *RHR;                  //讀取RHR中收到的字元到ch變數
        if(trap_ch == '\r')              //如果ch中的字元是 Enter (也就是 \r )
        {
            uart_putc('\n');           //則使用uart_c傳送一個換行符號 ( \n )

            //下面的if，比對用戶輸入 Enter 之前所輸入的字串是不是exit
            if(trap_temp[0] == 'e' && trap_temp[1] == 'x' && trap_temp[2] == 'i' && trap_temp[3] == 't' && trap_temp[4] == ' ')        
            {
                //用戶輸入是exit的話，用uart_puts()，傳送 quit 到用戶的螢幕上
                uart_puts("quit\n");
                for(int i = 0 ;i < trap_count ; i++)        //離開迴圈前，把temp[]裡面的字都清空
                {
                    trap_temp[i] = '\0';
                }
                trap_count = 0; 
                //離開迴圈
                break;                      
            }

            for(int i = 0 ;i < trap_count ; i++)        //離開迴圈前，把temp[]裡面的字都清空
            {
                trap_temp[i] = '\0';
            }
            trap_count = 0;                      //count重置為0，給輸入Enter後的下一行使用
        }
        else                        //如果不是 Enter 符號 ( \r )
        {
            trap_temp[trap_count] = trap_ch;       //另外也把 ch 存到 temp[255] 中
            trap_count++;                //count做為 temp[255] 的位置計數器，往前進一個，用來存下個字元
        }
    }
}

