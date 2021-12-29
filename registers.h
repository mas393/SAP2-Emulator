#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct reg
{
    int size;
    char *vals;
} reg;

reg* init_reg(int size);

void set_reg(reg *r, char *bits_in);

int get_reg_bit(reg *r, int bit_num);

void get_reg(reg *r, char *bits_out, int offset);

void print_reg(reg *r);

void del_reg(reg *)

#endif
