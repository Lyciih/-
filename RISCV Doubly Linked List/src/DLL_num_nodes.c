#include "dllSpec.h"

unsigned int DLL_num_nodes(const dllNode_t *head)//計算List中有幾個node
{
	int count = 0;
	dllNode_t *current = (dllNode_t *)head;
	while(current->next != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}