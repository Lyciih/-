#include"myos.h"
#define PAGE_SIZE 4096


static uint32_t _malloc_start = 0;   //用來記錄可以被分配的記憶體的起始位置
static uint32_t _malloc_end = 0;     //用來記錄可以被分配的記憶體的終點位置
static uint32_t _num_bytes = 0;     //用來記錄可以被分配的最大記憶體量
void *byte_start = NULL;

void malloc_init()
{
    byte_start = page_alloc(5);
    _num_bytes = 5 * PAGE_SIZE ;
    printf("\nall_malloc_start = %x, malloc_size = %x, num of bytes = %d\n", byte_start, 5*PAGE_SIZE, _num_bytes);

	uint8_t *byte = (uint8_t *)byte_start;
	for (int i = 0; i < _num_bytes; i++) {
		*byte = 0;
		byte++;	
	} 

    _malloc_start = (uint32_t)byte_start + PAGE_SIZE;
    _malloc_end = _malloc_start + 4 * PAGE_SIZE;

	printf("malloc range:   0x%x -> 0x%x\n\n", _malloc_start, _malloc_end);


}

void * malloc(size_t size)
{
	uint32_t count = (uint32_t)byte_start;
	int found = 0;
	uint32_t start = 0;
	uint8_t start_offset = 0;
	int start_offset_count = 0;
	int start_i = 0;
	uint8_t temp = 0; 
	
	for(int i = 0; i <= ( 4 * PAGE_SIZE)-size; i++)
	{
		
		uint8_t offset = 1;
		int offset_count = 0;
		while(offset)
		{

			temp = *(uint8_t *)count & offset;
			if(temp)
			{
				found = 0;
			}
			else
			{
				found++;
				if(found == 1)
				{
					start_i = i;
					start = count;
					start_offset = offset;
					start_offset_count = offset_count;
					
				}
				if(found == size)
				{
					printf("malloc  %x -> ",  _malloc_start + start_i*4 + start_offset_count);

					int setcount = 0;
					while(size > setcount)
					{
						if(start_offset == 0)
						{
							start_offset = 1;
						}
						
						while(start_offset)
						{
							if(start_offset == 0)
							{
								start_offset = 1;
							}

							*((uint8_t *)start) |= start_offset;
							setcount++;
							if(setcount == size)
							{
								start_offset = (start_offset << 1);						
								*((uint8_t *)start) |= start_offset;
								break;
							}
							else
							{
								start_offset = (start_offset << 2);
							}
						}
						start++;											
					}
					printf("%x\n", _malloc_start + start_i*4 + start_offset_count + size);
					return (void *)(_malloc_start + start_i*4 + start_offset_count);
				}
			}
			offset = (offset << 2);
			offset_count++;
		}
		count++;		
	}
	printf("not enough space");
}



void free(void *ptr)
{
	
	int byte_offset = (uint32_t)ptr - _malloc_start; ;
	int quotient = byte_offset / 4;
	int remainder = byte_offset % 4 ;
	void *start = byte_start + quotient ;
	uint8_t offset = (1 << 2*remainder);
	uint8_t offset_end = (3 << 2*remainder);
	int free_count = 0;

	printf("free  %x -> ", ptr);
	
	while(1)
	{
		if(offset == 0)
		{
			offset = 1;
			offset_end = 3;
		}		
		
		while(offset)
		{
			if(offset_end == (offset_end & *((uint8_t *)start)))
			{
				*((uint8_t *)start) ^= offset_end;
				free_count++;
				printf("%x\n", ptr + free_count);
				ptr = NULL;
				return;
			}
			*((uint8_t *)start) = (*((uint8_t *)start) ^ offset);
			free_count++;
			offset = (offset << 2);
			offset_end = (offset_end << 2);
		}
		start++;
	}

}
