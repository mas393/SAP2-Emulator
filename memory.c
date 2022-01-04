#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

memory* init_mem(int size)
{
    memory *m = (memory*)malloc(sizeof(memory));
    m -> blocks = size;
    m -> vals = (char*)malloc(size);
    return m;
}

void set_mem(memory *m, char *addr, char *data_in)
{
    int block = strtol(addr, 0, 2);
    unsigned char d = strtol(data_in, 0, 2);
    memcpy(&(m -> vals[block]), &d, 1);
}

void get_mem(memory *m, char *addr, char *data_out)
{
    int block = strtol(addr, 0, 2);
    char data = m -> vals[block];

    for (int i = 0; i < BLOCK_SIZE; i++)
	data_out[i] = ((data >> (BLOCK_SIZE - i - 1)) & 0x01) ? '1': '0';
}

void print_mem(memory *m, char *start_addr, int num_blocks)
{
    unsigned int block = strtol(start_addr, 0, 2);
    printf("start addr = %d\n", block);
    for (int i = 0; i < num_blocks; i++) {
	// print addresses
	printf("%d ", block);
	char data = m -> vals[block];
	for (int j = BLOCK_SIZE - 1; j >= 0; j--) {
	    printf("%d", (data >> j & 0x01));
	    if (j % 4 == 0) printf(" ");
	}
	printf("\n");
	block++;	    
    }
}

void del_mem(memory *m)
{
    free(m -> vals);
    free(m);
}
