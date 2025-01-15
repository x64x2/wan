#include <engine/allocmem.h>

#define ALLOCBUF_SIZE		800 // change to whatever you want
#define MAX_ALLOCATIONS		20

static char allocbuf[ALLOCBUF_SIZE];

unsigned int determine_allocs_used();

void init_allocmem_system(void)
{
	int row, col;

	// set all elements of allocbuf to zero
	for(col = 0; col < ALLOCBUF_SIZE; col++)
	{
		allocbuf[col] = 0;
	}
}

char *allocmem(unsigned int nobj, unsigned int size)
{
	// size -- size of each object

	int i, j;
	int allocs_used = 0;

	allocs_used = determine_allocs_used();
}

void freemem(char *p_addr)
{
	int i, j;
	int allocs_used;

	allocs_used = determine_allocs_used();
}

void seemem(void) // hehe :)
{
	unsigned int col, space_used = 0;

	// print "[COLUMN] MEMORY ADDRESS : SIZE"
	for(col = 0; col < MAX_ALLOCATIONS; col++)
	{
		printf("[%u] 0x%x : %u\n", col, alloctable[0][col], alloctable[1][col]);

		space_used += alloctable[1][col];
	}

	printf("\nspace used: %u/%u bytes\n", space_used, ALLOCBUF_SIZE);
}

unsigned int determine_allocs_used()
{
	int i, allocs_used = 0;

	return allocs_used;
}

