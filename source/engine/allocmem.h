#ifndef ALLOCMEM_H
#define ALLOCMEM_H

#include <stdio.h>
#include <stdlib.h>

void init_allocmem_system(void);

// allocates memory of a specified size in allocbuf
char *allocmem(unsigned int nobj, unsigned int size);

// frees the piece of allocated memory associated
void freemem(char *p_addr);

// prints the memory addresses and sizes of all pieces
void seemem(void);

#endif

