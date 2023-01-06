#include"os.h"


void showPCB()
{
    int task_count = DLL_num_nodes(pcb_list);
    printf("\033[?25h");
    uart_puts("\E[H\E[J");
    dllNode_t * target = pcb_list;
    printf("\n| task ID  ");
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        printf("|   %d    ", ((pcb_t *)target)->ID);
        
    }
    printf("|\n");
    printf("------------");
    for(int i = 0 ; i < task_count ; i++)
    {
        printf("---------");
    }
    printf("\n| priority ");
    target = pcb_list;
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        //顯示所有task的優先級
        printf("|   %d    ", ((pcb_t *)target)->priority);
    }
    printf("|\n");
    printf("------------");
    for(int i = 0 ; i < task_count ; i++)
    {
        printf("---------");
    }
    printf("\n|  finish  ");
    target = pcb_list;
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        if(((pcb_t *)target)->finish == 1)
        {
            printf("|\033[1;31;1m   1    \033[0;37;1m");
        }
        else
        {
        //顯示所有task是否被執行過的狀態
        printf("|   0    ");
        }
    }
    printf("|\n");
    printf("------------");
    for(int i = 0 ; i < task_count ; i++)
    {
        printf("---------");
    }

    printf("\n|  waiting ");
    target = pcb_list;
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        if(((pcb_t *)target)->waiting == 1)
        {
            printf("|\033[1;31;1m   1    \033[0;37;1m");
        }
        else
        {
        //顯示所有task是否被執行過的狀態
        printf("|   0    ");
        }
    }
    printf("|\n");
    printf("------------");
    for(int i = 0 ; i < task_count ; i++)
    {
        printf("---------");
    }

    printf("\n|  error   ");
    target = pcb_list;
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        if(((pcb_t *)target)->error == 1)
        {
            printf("|\033[1;31;1m   1    \033[0;37;1m");
        }
        else
        {
        //顯示所有task是否被執行過的狀態
        printf("|   0    ");
        }
    }
    printf("|\n\n");
    clock();
    printf(":%s", trap_temp);
}