#include <stdio.h>
#include <stdlib.h>
#include "registers.h"

reg* init_reg(int size)
{
    reg *r = (reg*)malloc(sizeof(reg));
    r -> size = size;
    r -> vals = (char*)malloc(size/8 + 1);
    return r;
}

void set_reg(reg *r, char *bits_in)
{
    for (int i = 0; i < r -> size; i += 8)
	*(r -> vals + i/8) = (strtol(bits_in, 0, 2) >> i) & 0xff;
}

int get_reg_bit(reg *r, int bit_num)
{
    return (r -> vals)[bit_num/8] >> (bit_num % 8) & 0x01;
}

void get_reg(reg *r, char *bits_out, int offset)
{
    int s = sizeof(bits_out);
    for (int i = s - 1; i >= 0; i--)
	bits_out[s - i - 1] = get_reg_bit(r, i - offest) ? '1': '0';
    *(out + s) = '\0'; //not sure if needed with passing in strings
}

void print_reg(reg *r)
{
    for (int i = r -> size - 1; i >= 0; i--){
	    printf("%d", get_reg_bit(r, i));
	    if (i % 4 == 0) printf(" ");
    }
}

void del_reg(reg *r)
{
    free(r -> vals);
    free(r);
}
