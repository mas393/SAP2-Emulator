#ifndef MEMORY_H
#define MEMORY_H

#include "registers.h"
#define BLOCK_SIZE 8

typedef struct memory
{
    int blocks;
    char *vals;
} memory;

memory* init_mem(int size);

void set_mem(memory *m, char *addr, char *data_in);

void get_mem(memory *m, char *data_out);

void print_mem(memory *m, char *start_addr, int num_blocks);

void del_mem(memory *m);

#endif
