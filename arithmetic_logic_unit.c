#include <stdlib.h>
#include <stdio.h>
#include "arithmetic_logic_unit.h"
#include "registers.h"

arithmetic_logic_unit* init_arithmetic_logic_unit(reg *a, reg *tmp)
{
    arithmetic_logic_unit *alu = (arithmetic_logic_unit*)malloc(sizeof(arithmetic_logic_unit));
    alu -> Accumulator = a;
    alu -> TMP = tmp;
    alu -> zero_flag = 0;
    alu -> sign_flag = 0;
    return alu;
}

void del_arithmetic_logic_unit(arithmetic_logic_unit *alu)
{
    free(alu);
}

char add_bits(reg *r1, reg *r2, int bit_loc, char *carry)
{
    char res = '0';

    switch(get_reg_bit(r1, bit_loc) + get_reg_bit(r2, bit_loc) + (int)*carry)
	{
	case 0:
	    *carry = 0x0;
	    break;
	case 1:
	    *carry = 0x0;
	    res = '1'; 
	    break;
	case 2:
	    *carry = 0x1;
	    break;
	case 3:
	    *carry = 0x1;
	    res = '1';
	    break;
	default:
	    printf("here some how");    
	};
    
    return res;
}

void addition(arithmetic_logic_unit *alu, char *result)
{
    char carry = 0x0;
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++){
	char v = add_bits(alu -> Accumulator, alu -> TMP, i, &carry);
	result[size - i - 1] = v;
    }
    //something about negative and zero values
}

void subtraction(arithmetic_logic_unit *alu, char *result)
{
    int add_flag = 1;
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	int loc = size - i - 1;
	if (add_flag) {
	    result[loc] = '0';
	    if (get_reg_bit(alu -> TMP, i)) {
		add_flag = 0;
		result[loc] = '1';
	    }
	}
	else {
	    if (get_reg_bit(alu -> TMP, i)) result[loc] = '0';
	    else result[loc] = '1';
	}
    }
    set_reg(alu -> TMP, result);
    addition(alu, result);
    //something about flags
}

void and(arithmetic_logic_unit *alu, char *results)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if ((get_reg_bit(alu -> Accumulator, i) && get_reg_bit(alu -> TMP, i))) results[i] = '1';
	else results[i] = '0';
    }    
}

void or(arithmetic_logic_unit *alu, char *results)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if ((get_reg_bit(alu -> Accumulator, i) || get_reg_bit(alu -> TMP, i))) results[i] = '1';
	else results[i] = '0';
    }
}
void xor(arithmetic_logic_unit *alu, char *results)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if ((get_reg_bit(alu -> Accumulator, i) && !(get_reg_bit(alu -> TMP, i)))) results[i] = '1';
	if ((!(get_reg_bit(alu -> Accumulator, i)) && get_reg_bit(alu -> TMP, i))) results[i] = '1';
	else results[i] = '0';
    }    
}

//all functions operating on a single register will be performed on the TMP register
//  pointed to by the arithmetic_logic_unit struct
void complement(arithmetic_logic_unit *alu, char *results){
    int size = alu -> TMP -> size;
    for (int i = 0; i < size; i++) {
	if (get_reg_bit(alu -> TMP, i)) results[i] = '0';
	else results[i] = '1';
    }
}

// get_reg_bit(,i) == reg[size - i - 1]
void rotate_right(arithmetic_logic_unit *alu, char *results)
{
    int size = alu -> TMP -> size;
    int temp = get_reg_bit(alu -> TMP, 0);
    for (int i = 0; i < size-1; i++)
	results[size-i-1] = (get_reg_bit(alu -> TMP, i+1)) ? '1':'0';
    results[0] = temp ? '1':'0';
}
void rotate_left(arithmetic_logic_unit *alu, char *results)
{
    int size = alu -> TMP -> size;
    int temp = get_reg_bit(alu -> TMP, size-1);
    for (int i = 0; i < size-1; i++)
	results[i] = (get_reg_bit(alu -> TMP, size-i-1)) ? '1':'0';
    results[size - 1] = temp ? '1':'0';
}
void increment(arithmetic_logic_unit *alu, char *results)
{   
    printf("implement increment\n");
}
void decrement(arithmetic_logic_unit *alu, char *results)
{
    printf("implement decrement\n");
}
