#include <stdio.h>
#include "smblib.h"
#include "list.h"

int allocate_heap(void *heap, int size)
{
	heap = malloc(size * sizeof(smb_mem_item_t));
}

int deallocate_heap(void *heap, int size)
{
	free heap;
}

// -----------------------------------

int find_freeslot(void *heap, int size)
{
	smb_mem_item_t *list;
	int i;

	list = heap;

	for (i=0;i<size;i++)
	{
		if ( !list[i]->used )
			return i;
	}

	fprintf(stderr,"find_freeslot(): out of memory\n");

	return -1;
}

int free_slot(void *heap, int slot)
{
	smb_mem_item_t *list;
	list = heap;

	list[slot]->used = 0;

}

int alloc_slot(void *heap, int size, int slot)
{

	smb_mem_item_t *list;
	int idx;
	list = heap;

	idx=find_freeslot(heap, size);

	

}

// ----------------------------------
