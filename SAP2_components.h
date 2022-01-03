#ifndef SAP2_COMPONENTS_H
#define SAP2_COMPONENTS_H

#include "registers.h"

typedef reg port;
typedef reg bus;

typedef struct instruction_reg {
    reg *instruction;
    reg *port;
} instruction_reg;

instruction_reg* init_instruction_reg();
void del_instruction_reg(instruction_reg *r);

typedef unsigned int program_counter;

void bit_string_from_int(unsigned int val, char *bits_out);

void get_PC(program_counter pc, char *bits_out);
void set_PC(program_counter pc, char *bits_in);
void set_PC_int(program_counter pc, int val);

#endif
