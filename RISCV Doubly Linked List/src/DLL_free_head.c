#include "dllSpec.h"

void DLL_free_head(dllNode_t *head)
{
	free(head);
	head = NULL;
	printf(">>> The head of the source list has been freed (response from DLL_free_head )\n");
}