#include <stdlib.h>
#include "SAP2_components.h"

instruction_reg* init_instruction_reg()
{
    instruction_reg *r = (instruction_reg*)malloc(sizeof(instruction_reg));
    r -> instruction = init_reg(8);
    r -> port = init_reg(8);
    return r;
}

void del_instruction_reg(instruction_reg *r)
{
    del_reg(r -> instruction);
    del_reg(r -> port);
    free(r);
}

void bit_string_from_int(unsigned int val, char *bits_out)
{
    int size = sizeof(bits_out);
    char v = (char)val;
    for (int i = 0; i < size; i++) bits_out[size-i-1] = (v >> i & 0x1 ? '1': '0');
}

void get_PC(program_counter pc, char *bits_out)
{
    bit_string_from_int(pc, bits_out);
}

void set_PC(program_counter pc, char *bits_in)
{
    pc = strtol(bits_in, 0, 2);
}

void set_PC_int(program_counter pc, int val)
{
    pc = val;
}
