#include <stdio.h>

#include "mem_index.h"
#include "../../utils/mem2_manager.h"

static u32 _size=0;
void* alloc_index(u32 size)
{
	if(_size>0)
		printf("error alloc_index twice\n");

	if(size==0)
		return (unsigned char *)SYS_GetArena2Hi();

	size+=2*1024*1024; // we reserve 2Mb extra because sometimes the index need to realloc
	size &= ~0x1f;
	_size=size;

	AddMem2Area(size+(16*1024), MEM2_INDEX);
    printf("mplayer alloc_index: %u\n",size);
    return mem2_memalign(32, size, MEM2_INDEX);
}

void free_index()
{
	if(_size==0)
		return;

	RemoveMem2Area(MEM2_INDEX);
	printf("mplayer free_index: %u\n",_size);
	_size=0;
}

u32 mem2_index_size()
{
	return _size;
}
