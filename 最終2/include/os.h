#ifndef __MYOS_H__
#define __MYOS_H__

#define STACK_SIZE 1024

//作業系統會用到的變數類型
#include"types.h" 


//uart
extern char trap_temp[255];
void uart_init(void);
int uart_putc(char ch);
int uart_puts(char *s);
void uart_r(void);
void uart_isr(void);



//printf會用到的庫
#include <stddef.h>
#include <stdarg.h>
int printf(const char* s, ...);

/* memory management */
void page_init(void);
void page_test(void);
void *page_alloc(int npages);
void page_free(void *p);

/* malloc */
void malloc_init(int page);
void * malloc(int nbytes);
void free(void *p);


#include<dllSpec.h>

/* schedule */          
void schedule_init(void);
void schedule(void);
int task_create(void (*start_routin)(char *param), char *param, uint8_t priority);
void task_exit(void);





typedef struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
}context;

void switch_to(struct context *next, reg_t pasue_location);

#include"hwtimer.h"
void timer_init(void);
void timer_handle(void);
void clock(void);
void timer_load(int interval);

typedef struct pcb{
	dllNode_t node;
	context context;
	uint8_t stack[STACK_SIZE];
	int ID;
	int priority;
	int finish;
	int waiting;
	int error;
	reg_t pasue_address;	
}pcb_t;

extern dllNode_t * pcb_list;
extern dllNode_t * current_task;
extern pcb_t * manage;
extern struct context trap_context;


#include"riscv.h"
void trap_init(void);
void trap_test(void);

#include"plic.h"

void plic_init(void);
int plic_claim(void);
void plic_complete(int irq);


void showPCB();
void software_interrupt();

typedef struct alarm{
    dllNode_t node;
    int tickets;
	int ID;
    dllNode_t * owner;
}alarm_t;

extern dllNode_t * alarm_list;
void alarm_init();
void create_alarm(int tickets);
void show_alarm();
void alarm_handle();

#endif