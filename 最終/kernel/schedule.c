#include"os.h"


int process_ID = 0;

dllNode_t * pcb_list;
dllNode_t * current_task = NULL;
pcb_t * manage;
struct context trap_context;



void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

void task_exit()
{
    ((pcb_t *)current_task)->finish = 1;
    switch_to( &(manage->context), &trap_context);
}


void software_interrupt()
{
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 1;
}

void task_wait()
{
    ((pcb_t *)current_task)->waiting = 1;
    ((pcb_t *)current_task)->finish = 0;
    software_interrupt();
}

//第1個task
void user_task1(char* santams)
{    
    printf("Task 1: Running...|%s\n", santams);
    uart_puts("                  |...\n");
    uart_puts("                     |...\n");
    uart_puts("                        |...\n");
    asm volatile("wfi");
    task_exit();
}
//第2個task
void user_task2(char* santams)
{
    printf("Task 2: Running............|.........%s\n", santams);
    uart_puts("                        ...|\n");
    uart_puts("                     ...|\n");
    uart_puts("                  ...|\n");
    trap_test();
    asm volatile("wfi");
    task_exit();
}

//第3個task
void user_task3(char* santams)
{
    printf("                    Task 3: Running...|%s\n", santams);
    uart_puts("                                   |...\n");
    uart_puts("                                      |...\n");
    uart_puts("                                         |...\n");
    task_wait();
    asm volatile("wfi");
    task_exit();
}
//第4個task
void user_task4(char* santams)
{
    printf("                    Task 4: Running............|.........%s\n", santams);
    uart_puts("                                         ...|\n");
    uart_puts("                                      ...|\n");
    uart_puts("                                   ...|\n");
    asm volatile("wfi");
    task_exit();
}

//第5個task
void user_task5(char* santams)
{
    printf("                                        Task 5: Running...|%s\n", santams);
    uart_puts("                                                       |...\n");
    uart_puts("                                                          |...\n");
    uart_puts("                                                             |...\n");
    asm volatile("wfi");
    task_exit();
}
//第6個task
void user_task6(char* santams)
{
    printf("                                        Task 6: Running............|.........%s\n", santams);
    uart_puts("                                                             ...|\n");
    uart_puts("                                                          ...|\n");
    uart_puts("                                                       ...|\n");
    asm volatile("wfi");
    task_exit();
}

//第7個task
void user_task7(char* santams)
{
    printf("                                                            Task 7: Running...|%s\n", santams);
    uart_puts("                                                                           |...\n");
    uart_puts("                                                                              |...\n");
    uart_puts("                                                                                 |...\n");
    asm volatile("wfi");
    task_exit();
}
//第8個task
void user_task8(char* santams)
{
    printf("                                                            Task 8: Running............|.........%s\n", santams);
    uart_puts("                                                                                 ...|\n");
    uart_puts("                                                                              ...|\n");
    uart_puts("                                                                           ...|\n");
    asm volatile("wfi");
    task_exit();
}

//第9個task
void user_task9(char* santams)
{
    printf("                                                                                Task 9: Running...|%s\n", santams);
    uart_puts("                                                                                               |...\n");
    uart_puts("                                                                                                  |...\n");
    uart_puts("                                                                                                     |...\n");
    asm volatile("wfi");
    task_exit();
}


//將所有task初始化
void schedule_init()
{
    pcb_list = DLL_init();
    current_task = NULL;

    manage = (pcb_t *)malloc(sizeof(pcb_t));
	if (manage != NULL) {
        manage->ID = process_ID;
        manage->finish = 0;
        manage->node.next = NULL;
        manage->node.prev = NULL;
        manage->priority = 0;
        manage->context.sp = (reg_t) &(manage->stack[STACK_SIZE - 1]);
        manage->context.ra = (reg_t) schedule;
        process_ID++;
        DLL_add_tail(&manage->node, pcb_list);
    }
    task_create(user_task1, "a", 2);
    task_create(user_task2, "b", 3);
    task_create(user_task3, "c", 7);
    task_create(user_task4, "d", 9);
    task_create(user_task5, "e", 5);
    task_create(user_task6, "f", 4);
    task_create(user_task7, "g", 6);
    task_create(user_task8, "h", 7);
    task_create(user_task9, "i", 8);
}



void schedule()
{
    /* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
    int task_count = DLL_num_nodes(pcb_list);
    dllNode_t * max = NULL;
    dllNode_t * target = pcb_list;
    int temp = 0;
    while(target->next != NULL)
    {
        target = DLL_next_node(target);
        //找到優先度最高的task，並且必須是沒有被做過的
        if(((pcb_t *)target)->priority >= temp && ((pcb_t *)target)->finish != 1 && ((pcb_t *)target)->error == 0 && ((pcb_t *)target)->waiting == 0)
        {
            //將最優先要做的task編號設給 max 變數
            temp = ((pcb_t *)target)->priority;
            max = target;
        }
    }
  

    //如果 ((pcb_t *)max)->priority 為 0 且 ((pcb_t *)max)->finish 也是 0，代表系統目前沒有其他任務要執行
    if(((pcb_t *)max)->priority == 0 && ((pcb_t *)max)->finish == 0)
    {
        printf("here\n");
        /*
        current = pcb_list;
        while(current->next != NULL)
        {
            current = DLL_next_node(current);
            if(((pcb_t *)current)->error == 1 || ((pcb_t *)current)->finish == 1)
            {
                DLL_delete(current);
                free(current);
            }
        }
        */

        asm volatile("wfi");
    }
    //不是的話代表還有task沒做完，於是將下個 task 的 context 指針設為 max 所代表的task 然後switch_to()
    else
    {
        printf("max = task %d\n\n\n", ((pcb_t *)max)->ID);
        struct context *next = &(((pcb_t *)max)->context);
        //將該task的task_finish設為1，代表已經做了
        //((pcb_t *)max)->finish = 1;
        target = max;
        current_task = max;
	    switch_to(next,  &trap_context);        
    }    
    
}




int task_create(void (*start_routin)(char *param), char *param, uint8_t priority)
{
    pcb_t * porcess = (pcb_t *)malloc(sizeof(pcb_t));
	if (porcess != NULL) {
        porcess->ID = process_ID;
        porcess->finish = 0;
        porcess->waiting = 0;
        porcess->node.next = NULL;
        porcess->node.prev = NULL;
        porcess->priority = priority;
        porcess->context.sp = (reg_t) &(porcess->stack[STACK_SIZE -1]);
        porcess->context.ra = (reg_t) start_routin;;
        porcess->context.a0 = (reg_t) param;
        process_ID++;
        DLL_add_tail(&porcess->node, pcb_list);
		return 0;
	} else {
		return -1;
	}
}



