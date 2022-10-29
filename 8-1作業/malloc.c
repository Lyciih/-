#include"myos.h"
#define PAGE_SIZE 4096


static uint32_t _malloc_start = 0;  //用來記錄可以被分配的記憶體的起始位置
static uint32_t _malloc_end = 0;    //用來記錄可以被分配的記憶體的終點位置
static uint32_t _num_bytes = 0;     //用來記錄可以被分配的最大記憶體量
void *byte_start = NULL;			//用來作為malloc管理頁起始點的指標
int manage_page;					//紀錄用來管理的page的數量
int use_page;						//紀錄可被分配的page數量
int malloc_init_success = 0;		//紀錄malloc初始化是否成功

//申請給malloc用的page必須是5的倍數，因為管理與分配的比例是 1:4
void malloc_init(int page)
{
	//檢查申請的page是否是5的倍數
	if(page % 5 || page == 0)
	{
		printf("page use for malloc must be a multiple of 5\n");
		printf("malloc init fault\n");
		malloc_init_success = 0;
		return;
	}
	//是的話就將 malloc_init_success 設為1
	else
	{
		malloc_init_success = 1;
	}

	manage_page = page/5;				//算出用來管理的page有幾個
	use_page = page - manage_page;		//算出實際可分配的page有幾個

    byte_start = page_alloc(page);		//把申請到的page地址設給起始點指標
    _num_bytes = page * PAGE_SIZE ;	//全部malloc包含管理頁的記憶體大小
	//輸出基本數據
    printf("\nmalloc management start = %x, can be malloc_size = %x, all malloc use bytes = %d\n", byte_start, use_page * PAGE_SIZE, _num_bytes);
	
	//將malloc會用到的空間都初始化為0
	uint8_t *byte = (uint8_t *)byte_start;
	for (int i = 0; i < _num_bytes; i++) {
		*byte = 0;
		byte++;	
	} 

	//設定malloc可分配空間的起始點與終點
    _malloc_start = (uint32_t)byte_start + manage_page * PAGE_SIZE;
    _malloc_end = _malloc_start + use_page * PAGE_SIZE;

	//輸出malloc可分配空間的起始點與終點
	printf("malloc range:   0x%x -> 0x%x\n\n", _malloc_start, _malloc_end);
	return;
}

void * malloc(size_t size)
{
	//使用malloc前要先檢查malloc初始化是否成功，malloc_init_success 是 0 ，就代表失敗並退出
	if(malloc_init_success != 1)
	{
		printf("malloc: malloc init fault or not yet\n");
		return NULL;
	}

	//設定尋找可用空間時用來記錄偏移量及數量的各項變數
	uint32_t count = (uint32_t)byte_start;	//重設當前檢查到的管理byte的位置
	int found = 0;							//紀錄已被找到的連續空間的數量
	uint32_t start = 0;						//紀錄第一個被找到的空位所代表的管理byte
	uint8_t offset = 1;						//偏移遮罩，初始化為1
	int offset_count = 0;					//偏移計數器，初始化為0
	uint8_t start_offset = 0;				//紀錄第一個被找到的空位當下的偏移遮罩
	int start_offset_count = 0;				//紀錄第一個被找到的空位當下的偏移計數器
	int start_i = 0;						//紀錄找到第一個空位時已經檢查過幾個byte了
	uint8_t temp = 0;						//暫存當下檢查的管理byte的值，避免檢查時更動到管理byte
	
	
	
	//對管理區的每個byte中的bit進行檢查
	for(int i = 0; i <= ( use_page * PAGE_SIZE)-size; i++)
	{
		//每次前進一個byte，就把偏移遮罩及偏移計數器重置
		offset = 1;
		offset_count = 0;

		//進入while中，使用偏移遮罩對byte中的每兩個bit進行檢查
		while(offset)
		{
			//byte與遮罩進行 and 運算，如果發現已被使用，則把found重設為0
			temp = *(uint8_t *)count & offset;
			if(temp)
			{
				found = 0;
			}
			//如果發現空位，則把found+1
			else
			{
				found++;
				//如果found剛好是1，代表這是這段空間的起始點，紀錄這個位置及偏移量，如果最後成功分配，就換算出位置並返回給使用者
				if(found == 1)
				{
					start_i = i;						//紀錄找到第一個空位時已經檢查過幾個byte了
					start = count;						//紀錄第一個被找到的空位所代表的管理byte
					start_offset = offset;				//紀錄第一個被找到的空位當下的偏移遮罩
					start_offset_count = offset_count;	//紀錄第一個被找到的空位當下的偏移計數器
					
				}
				//如果found剛好是要申請的數量，就進入設定階段，把被分配的空間在管理區代表的兩個bit設為01，並把最後一個byte代表的兩個bit設為11
				if(found == size)
				{
					//輸出被分配的位置的起始地址
					printf("malloc  %x -> ",  _malloc_start + start_i*4 + start_offset_count);

					//初始化分配計數器
					int setcount = 0;

					//當分配計數器小於要申請的空間時就不停往前設定
					while(size > setcount)
					{
						//如果分配用的遮罩是0，就將它重置為1
						if(start_offset == 0)
						{
							start_offset = 1;
						}
						
						//利用分配遮罩對一個byte中的bit進行設定
						while(start_offset)
						{
							if(start_offset == 0)
							{
								start_offset = 1;
							}
						
							//當前的管理byte與遮照做 or 運算，將被分配的byte所代表的兩個bit設為01
							*((uint8_t *)start) |= start_offset;
							//分配計數器+1
							setcount++;

							//如果分配計數器剛好等於要分配的數量
							if(setcount == size)
							{
								//將遮罩左移1個bit
								start_offset = (start_offset << 1);
								//當前的管理byte與遮照做 or 運算，	將被分配的最後一個byte所代表的兩個bit設為11			
								*((uint8_t *)start) |= start_offset;
								//設定完成，離開迴圈
								break;
							}
							else
							{
								//如果分配計數器尚未到達所要的量，將遮罩左移兩個bit，繼續下一輪
								start_offset = (start_offset << 2);
							}
						}
						//當遮罩設定完一個byte而分配數量還沒到達時，這時遮罩會因為左移而變為0，離開while
						//將紀錄地址的start往前1位，下一輪開始設定下一個管理byte
						start++;							
					}

					//當全部都設定完後，輸出被分配位置的終點，並且計算起始位置返回給使用者
					printf("%x\n", _malloc_start + start_i*4 + start_offset_count + size);
					return (void *)(_malloc_start + start_i*4 + start_offset_count);
				}
			}
			//每檢查完2個bit，就將遮罩左移兩位，偏移計數器加1
			offset = (offset << 2);
			offset_count++;
		}
		//每檢查完1個管理byte，就將地址加1，前進到下一個byte
		count++;		
	}
	//如果所有malloc可分配空間都檢查完卻找不到可用的區段，就輸出空間不足的訊息
	printf("not enough space");
}



void free(void *ptr)
{
	//使用free前要先檢查malloc初始化是否成功，malloc_init_success 是 0 ，就代表失敗並退出
	if(malloc_init_success != 1)
	{
		printf("free: malloc init fault or not yet\n");
		return;
	}
	
	int byte_offset = (uint32_t)ptr - _malloc_start;	//計算出要釋放的位置在malloc可分配空間的偏移量
	int quotient = byte_offset / 4;						//計算出要釋放的位置在malloc管理區中byte的偏移量
	int remainder = byte_offset % 4 ;					//計算出要釋放的位置在malloc管理區中byte的遮罩偏移量
	void *start = byte_start + quotient ;				//設定要重設的管理byte的起始位置
	uint8_t offset = (1 << 2*remainder);				//初始化用來重設的遮罩為1
	uint8_t offset_end = (3 << 2*remainder);			//初始化用來重設最後兩個bit的遮罩為3 (當成二進位時是11)
	int free_count = 0;									//紀錄釋放了多少byte

	printf("free  %x -> ", ptr);						//輸出被釋放的起始位置
	
	//進入無窮迴圈，只有當重設完所有管理用的byte及bit才退出
	while(1)
	{
		//如果重設用的遮罩是0，就將它重置為1，終點遮罩也重置為3 (當成二進位時是11)
		if(offset == 0)
		{
			offset = 1;
			offset_end = 3;
		}		
		
		//利用遮罩重設被釋放的byte所對映的兩個bit
		while(offset)
		{
			//如果當前重設的兩個bit剛好是11
			if(offset_end == (offset_end & *((uint8_t *)start)))
			{
				//把當前的管理byte與遮罩做 XOR 運算，把兩個bit設為0
				*((uint8_t *)start) ^= offset_end;
				//釋放計數器加1
				free_count++;
				//計算並輸出最後一個被釋放的byte所在的位置
				printf("%x\n", ptr + free_count);
				//將釋放指針清除為NULL
				ptr = NULL;
				//離開無窮迴圈
				return;
			}

			//如果當前重設的兩個bit不是11
			//把當前的管理byte與遮罩做 XOR 運算，把兩個bit設為00
			*((uint8_t *)start) = (*((uint8_t *)start) ^ offset);
			//釋放計數器加1
			free_count++;
			//遮罩左移兩個bit
			offset = (offset << 2);
			//終點遮罩左移兩個bit
			offset_end = (offset_end << 2);
		}	
		start++;	//重設完一個byte，前進到下一個byte
	}
}
