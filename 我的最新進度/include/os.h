#ifndef __MYOS_H__
#define __MYOS_H__

#include "types.h" //作業系統會用到的變數類型

//printf會用到的庫
#include <stddef.h>
#include <stdarg.h>

extern int uart_putc(char ch);
extern int uart_puts(char *s);

extern int printf(const char* s, ...);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);


#endif