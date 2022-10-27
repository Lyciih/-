#include"myos.h"
#define PAGE_SIZE 4096


static uint32_t _malloc_start = 0;   //用來記錄可以被分配的記憶體的起始位置
static uint32_t _malloc_end = 0;     //用來記錄可以被分配的記憶體的終點位置
static uint32_t _num_bytes = 0;     //用來記錄可以被分配的最大記憶體量

#define BYTE_TAKEN (uint8_t)(1 << 0)
#define BYTE_LAST  (uint8_t)(1 << 1)

typedef struct m_byte {
	uint8_t flags;
}m_byte;


static inline void _clear(struct m_byte *byte)
{
	byte->flags = 0;
}

static inline int _is_free(struct m_byte *byte)
{
	if (byte->flags & BYTE_TAKEN) {
		return 0;
	} else {
		return 1;
	}
}

static inline void _set_flag(struct m_byte *byte, uint8_t flags)
{
	byte->flags |= flags;
}


static inline int _is_last(struct m_byte *byte)
{
	if (byte->flags & BYTE_LAST) {
		return 1;
	} else {
		return 0;
	}
}


void *byte_start = NULL;

void malloc_init()
{
    byte_start = page_alloc(5);
    _num_bytes = 5 * PAGE_SIZE ;
    printf("\nmalloc_start = %x, malloc_size = %x, num of bytes = %d\n", byte_start, 5*PAGE_SIZE, _num_bytes);

	struct m_byte *byte = (struct m_byte *)byte_start;
	for (int i = 0; i < _num_bytes; i++) {
		_clear(byte);
		byte++;	
	} 

    _malloc_start = (uint32_t)byte_start + PAGE_SIZE;
    _malloc_end = _malloc_start + 4 * PAGE_SIZE;

	printf("malloc:   0x%x -> 0x%x\n", _malloc_start, _malloc_end);


}

void * malloc(int nbytes)
{
	uint32_t count = (uint32_t)byte_start;
	int found = 0;
	uint32_t start = 0;
	uint8_t start_offset = 0;
	int start_offset_count = 0;
	int start_i = 0;
	uint8_t temp = 0; 
	
	for(int i = 0; i <= ( 4 * PAGE_SIZE)-nbytes; i++)
	{
		
		uint8_t offset = 1;
		int offset_count = 0;
		while(offset)
		{

			temp = *(uint8_t *)count & offset;
			if(temp)
			{
				found = 0;
				printf("use %x %x %x %x %x\n",*((m_byte *)(byte_start+4)), *((m_byte *)(byte_start+3)), *((m_byte *)(byte_start+2)), *((m_byte *)(byte_start+1)), *((m_byte *)byte_start));
			}
			else
			{
				printf("found\n");
				found++;
				if(found == 1)
				{
					start_i = i;
					start = count;
					start_offset = offset;
					start_offset_count = offset_count;
				}
				if(found == nbytes)
				{
					int setcount = 0;
					while(nbytes > setcount)
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
							printf("get %x %x\n", *((uint8_t *)start), start_offset);
							*((uint8_t *)start) |= start_offset;
							setcount++;
							printf("get %x %x\n", *((uint8_t *)start), start_offset);
							printf("---------------\n");
							if(setcount == nbytes)
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
					printf("END\n\n");
					return (void *)(_malloc_start + start_i*4 + start_offset_count);
				}
			}
			offset = (offset << 2);
			offset_count++;
		}
		count++;
		printf("NEXT %x\n\n", count);		
	}
}

void free(void *p)
{
	int offset = 0 ;
	offset = (uint32_t)p - _malloc_start;
	printf("%x\n",offset );
}