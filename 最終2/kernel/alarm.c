#include"os.h"

dllNode_t * alarm_list = NULL;

void alarm_init()
{
    alarm_list = DLL_init();
}


void create_alarm(int tickets)
{
    alarm_t * new = (alarm_t *)malloc(sizeof(alarm_t));
    new->node.next = NULL;
    new->node.prev = NULL;
    new->owner = current_task;
    new->tickets = tickets;
    new->ID = ((pcb_t *)current_task)->ID;

    dllNode_t * temp = alarm_list;
    int before = 0;
    int after = 0;
    while (1)
    {
        if(temp->next == NULL)
        {
            DLL_addto_next(&new->node, temp);
            new->tickets -= after;
            return;
        }
        else
        {            
            temp = temp->next;
            before += after;
            after += ((alarm_t *)temp)->tickets;
            if(new->tickets < after)
            {
                DLL_addto_prev(&new->node, temp);
                new->tickets -= before;
                ((alarm_t *)temp)->tickets -= new->tickets;
                return;
            }
             
        }

    }
    


    
}

void show_alarm()
{
    dllNode_t * target = alarm_list;

    printf("alarm : ");
    if(target != NULL)
    {
        while(target->next != NULL)
        {
            target = DLL_next_node(target);
            //顯示所有task的優先級
            printf(" <--- %d (process ID : %d)", ((alarm_t *)target)->tickets, ((alarm_t *)target)->ID);
        }
    }
    
    printf("\n");
}

void alarm_handle()
{
    dllNode_t * temp = DLL_next_node(alarm_list);
    if(temp != NULL)
    {
        ((alarm_t *)temp)->tickets--;
        if(((alarm_t *)temp)->tickets == 0)
        {
            ((pcb_t *)(((alarm_t *)temp)->owner))->waiting = 0;
            DLL_delete(temp);
        }
    }
}