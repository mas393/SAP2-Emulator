#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include "registers.h"

typedef struct arithmetic_logic_unit
{
    reg *Accumulator;
    reg *TMP;
    int zero_flag;
    int sign_flag;
} arithmetic_logic_unit;

arithmetic_logic_unit* init_arithmetic_logic_unit(reg *a, reg *tmp);
void del_arithmetic_logic_unit(arithmetic_logic_unit *alu);

void addition(arithmetic_logic_unit *alu, char *result);
void subtraction(arithmetic_logic_unit *alu, char *result);
void and(arithmetic_logic_unit *alu, char *results);
void or(arithmetic_logic_unit *alu, char *results);
void xor(arithmetic_logic_unit *alu, char *results);

void complement(arithmetic_logic_unit *alu, char *results);
void rotate_right(arithmetic_logic_unit *alu, char *results);
void rotate_left(arithmetic_logic_unit *alu, char *results);
void increment(arithmetic_logic_unit *alu, char *results);
void decrement(arithmetic_logic_unit *alu, char *results);

#endif
