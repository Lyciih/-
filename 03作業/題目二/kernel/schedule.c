#include"os.h"

extern void sys_switch(struct context *old, struct context *new);
extern void switch_to(struct context *next);

#define STACK_SIZE 1024

//為兩個task產生context，和他們各自的stack
struct context ctx_task1;
struct context ctx_task2;
uint8_t task1_stack[STACK_SIZE];
uint8_t task2_stack[STACK_SIZE];


void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

//第一個task
void user_task1(void)
{
    uart_puts("Task 1: Running...|\n");
    uart_puts("                  |...\n");
    uart_puts("                     |...\n");
    uart_puts("                        |...\n");
    task_delay(6000);
    sys_switch(&ctx_task1, &ctx_task2);
}
//第二個task
void user_task2(void)
{
    uart_puts("Task 2: Running............|.........\n");
    uart_puts("                        ...|\n");
    uart_puts("                     ...|\n");
    uart_puts("                  ...|\n");
    task_delay(6000);
    sys_switch(&ctx_task2, &ctx_task1);
}

//將兩個task初始化
void schedule_init()
{
    ctx_task1.sp = (reg_t) &task1_stack[STACK_SIZE-1];
    ctx_task1.ra = (reg_t) user_task1;
    ctx_task2.sp = (reg_t) &task2_stack[STACK_SIZE-1];
    ctx_task2.ra = (reg_t) user_task2;
}

void schedule()
{
	struct context *next = &ctx_task1;
	switch_to(next);
}