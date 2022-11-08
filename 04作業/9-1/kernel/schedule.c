#include"os.h"

extern void sys_switch(struct context *old, struct context *new);
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024



uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];
uint8_t context_priority[MAX_TASKS];
uint8_t task_finish[MAX_TASKS];

 int _top = 0;


void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

void task_exit()
{
    switch_to(&kernel_context);
}

//第1個task
void user_task0(char* sentence)
{
    printf("Task 0: Running...|%s\n", sentence);
    uart_puts("                  |...\n");
    uart_puts("                     |...\n");
    uart_puts("                        |...\n");
    task_delay(6000);
    task_exit();
}
//第2個task
void user_task1(char* sentence)
{
    printf("Task 1: Running............|.........%s\n", sentence);
    uart_puts("                        ...|\n");
    uart_puts("                     ...|\n");
    uart_puts("                  ...|\n");
    task_delay(6000);
    task_exit();
}

//第3個task
void user_task2(char* sentence)
{
    printf("                    Task 2: Running...|%s\n", sentence);
    uart_puts("                                   |...\n");
    uart_puts("                                      |...\n");
    uart_puts("                                         |...\n");
    task_delay(6000);
    task_exit();
}
//第4個task
void user_task3(char* sentence)
{
    printf("                    Task 3: Running............|.........%s\n", sentence);
    uart_puts("                                         ...|\n");
    uart_puts("                                      ...|\n");
    uart_puts("                                   ...|\n");
    task_delay(6000);
    task_exit();
}

//第5個task
void user_task4(char* sentence)
{
    printf("                                        Task 4: Running...|%s\n", sentence);
    uart_puts("                                                       |...\n");
    uart_puts("                                                          |...\n");
    uart_puts("                                                             |...\n");
    task_delay(6000);
    task_exit();
}
//第6個task
void user_task5(char* sentence)
{
    printf("                                        Task 5: Running............|.........%s\n", sentence);
    uart_puts("                                                             ...|\n");
    uart_puts("                                                          ...|\n");
    uart_puts("                                                       ...|\n");
    task_delay(6000);
    task_exit();
}

//第7個task
void user_task6(char* sentence)
{
    printf("                                                            Task 6: Running...|%s\n", sentence);
    uart_puts("                                                                           |...\n");
    uart_puts("                                                                              |...\n");
    uart_puts("                                                                                 |...\n");
    task_delay(6000);
    task_exit();
}
//第8個task
void user_task7(char* sentence)
{
    printf("                                                            Task 7: Running............|.........%s\n", sentence);
    uart_puts("                                                                                 ...|\n");
    uart_puts("                                                                              ...|\n");
    uart_puts("                                                                           ...|\n");
    task_delay(6000);
    task_exit();
}

//第9個task
void user_task8(char* sentence)
{
    printf("                                                                                Task 8: Running...|%s\n", sentence);
    uart_puts("                                                                                               |...\n");
    uart_puts("                                                                                                  |...\n");
    uart_puts("                                                                                                     |...\n");
    task_delay(6000);
    task_exit();
}

//第10個task
void user_task9(char* sentence)
{
    printf("                                                                               Task 9: Running............|.........%s\n", sentence);
    uart_puts("                                                                                                  ...|\n");
    uart_puts("                                                                                               ...|\n");
    uart_puts("                                                                                            ...|\n");
    task_delay(6000);
    task_exit();
}



//將所有task初始化
void schedule_init()
{  
    task_create(user_task0, "a", 2);
    task_create(user_task1, "b", 3);
    task_create(user_task2, "c", 7);
    task_create(user_task3, "d", 9);
    task_create(user_task4, "e", 5);
    task_create(user_task5, "f", 4);
    task_create(user_task6, "g", 6);
    task_create(user_task7, "h", 7);
    task_create(user_task8, "i", 8);
    task_create(user_task9, "j", 0);
}

void schedule()
{
    int max = 0;
    int temp = 0;
    int final = 0;


    printf("\n|   task   ");
    for(int i = 0; i< MAX_TASKS; i++)
    {
        //顯示所有task編號
        printf("| task %d ", i);
    }
    printf("|\n------------------------------------------------------------------------------------------------------");
    printf("\n| priority ");
    //掃描整個 context_priority[] 陣列
    for(int i = 0; i< MAX_TASKS; i++)
    {
        //顯示所有task的優先級
        printf("|   %d    ", context_priority[i]);
        //找到優先度最高的task，並且必須是沒有被做過的
        if( context_priority[i] >= temp && task_finish[i] != 1)
        {
            temp = context_priority[i];
            //將最優先要做的task編號設給 max 變數
            max = i;
        }
    }
    printf("|\n------------------------------------------------------------------------------------------------------");
    printf("\n|  finish  ");
    for(int i = 0; i< MAX_TASKS; i++)
    {
        //顯示所有task是否被執行過的狀態
        printf("|   %d    ", task_finish[i]);
    }
    printf("|\n\n");


    for(int i = 0; i< MAX_TASKS; i++)
    {
        //找到優先度最低的task
        if( context_priority[i] <= temp )
        {
            temp = context_priority[i];
            //將最低的task編號設給 final 變數
            final = i;
        }
    }
    

    //如果 context_priority[final] 為0 且 task_finish[final] 也是0，代表所有任務都已經輪過一遍
    if(context_priority[final] == 0 && task_finish[final] == 1)
    {
        //將所有優先度不為 0 的 task 的完成狀態都重設為 0
        for(int i = 0; i< MAX_TASKS; i++)
        {
            if(context_priority != 0)
            {
                task_finish[i] = 0 ;
            }
        }
        printf("***************************  task reset  **********************************\n");
    }
    //不是的話代表還有task沒做完，於是將下個 task 的 context 指針設為 max 所代表的task 然後switch_to()
    else
    {
        printf("max = task %d\n\n\n", max);
        struct context *next = &ctx_tasks[max];
        task_finish[max] = 1; //將該task的task_finish設為1，代表已經做了
	    switch_to(next);        
    }

}



int task_create(void (*start_routin)(char *param), char *param, uint8_t priority)
{
	if (_top < MAX_TASKS) {
        //以當前的TOP值來分配給函數指標指向的task一個context
        //並設定好它的stack point 和 raturn address
		ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
		ctx_tasks[_top].ra = (reg_t) start_routin;
        //將要給該task的參數設在該context的 a0 暫存器，這樣 context_switch 完就能使用
        ctx_tasks[_top].a0 = (reg_t) param;
        //把該 task 的 priority 存在對應的陣列位置中
        context_priority[_top] = priority;
		_top++;
		return 0;
	} else {
		return -1;
	}
}


