#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

static int negative_check(reg *r){
    int s = r -> size;
	return (get_reg_bit(r, s-1));
}

static void update_flags(arithmetic_logic_unit *alu, char *result)
{
    if (result[0] == '1') {
	printf("NEGATIVE\n");
	alu -> sign_flag = 1;
    }
    else alu -> sign_flag = 0;

    if (!strcmp(result, "00000000")) {
	printf("ZERO\n");
	alu -> zero_flag = 1;
    }
    else alu -> zero_flag = 0;
}

static char add_bits(reg *r1, reg *r2, int bit_loc, char *carry)
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
    printf("result = %s\n", result);
    for (int i = 0; i < size; i++){
	printf("i = %d, carry = %d ", i, carry);
	char v = add_bits(alu -> Accumulator, alu -> TMP, i, &carry);
	printf(" v = %c, carry = %d\n", v, carry);
	result[16 - i - 1] = v;
	printf("result = %s\n", result);
    }
    printf("result = %s\n", result);
    update_flags(alu, result);
}


static void twos_complement(reg *r, char *result)
{
    int add_flag = 1;
    int size = r -> size;
    
    for (int i = 0; i < size; i++) {
	int loc = 16 - i - 1;
	if (add_flag) {
	    if (get_reg_bit(r, i)) {
		add_flag = 0;
		result[loc] = '1';		
	    }
	    else result[loc] = '0';
	}
	else {
	    if (get_reg_bit(r, i)) result[loc] = '0';
	    else result[loc] = '1';
	}
    }
}

void subtraction(arithmetic_logic_unit *alu, char *result)
{
    //    if (negative_check(alu -> TMP)) {
    //	printf("NEGATIVE\n");
    //	for (int i = 0; i < alu -> TMP -> size; i++) printf("i= %d get_reg_bit(i) = %d\n", i, get_reg_bit(alu->TMP, i));
    //	twos_complement(alu -> TMP, result);
	
    //    }
    twos_complement(alu -> TMP, result);
    set_reg(alu -> TMP, result); 
    addition(alu, result);
    update_flags(alu, result);
}

void and(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if ((get_reg_bit(alu -> Accumulator, i) && get_reg_bit(alu -> TMP, i))) result[16 - i - 1] = '1';
	else result[16 - i - 1] = '0';

    }
    update_flags(alu, result);
}

void or(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if (get_reg_bit(alu -> Accumulator, i) || get_reg_bit(alu -> TMP, i)) result[16 - i - 1] = '1';
	else result[16 - i - 1] = '0';
	printf("%d %s\n", i, result);
    }
    update_flags(alu, result);
}
void xor(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if ((get_reg_bit(alu -> Accumulator, i) && !(get_reg_bit(alu -> TMP, i)))) result[16 - i - 1] = '1';
	else if ((!(get_reg_bit(alu -> Accumulator, i)) && (get_reg_bit(alu -> TMP, i)))) result[16 - i - 1] = '1';
	else result[16 - i - 1] = '0';
    }
    update_flags(alu, result);
}

void complement(arithmetic_logic_unit *alu, char *result){ //ones complement
    int size = alu -> Accumulator -> size;
    for (int i = 0; i < size; i++) {
	if (get_reg_bit(alu -> Accumulator, i)) result[16 - i - 1] = '0';
	else result[16 - i - 1] = '1';
    }
}

// get_reg_bit(,i) == reg[size - i - 1]
void rotate_right(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> Accumulator -> size;
    int temp = get_reg_bit(alu -> Accumulator, 0);
    for (int i = 0; i < size-1; i++)
	result[16-i-1] = (get_reg_bit(alu -> Accumulator, i+1)) ? '1':'0';
    result[size] = temp ? '1':'0';
}
void rotate_left(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> Accumulator -> size;
    int temp = get_reg_bit(alu -> Accumulator, size-1);
    for (int i = 0; i < size-1; i++)
	result[size + i] = (get_reg_bit(alu -> Accumulator, size-i-2)) ? '1':'0';
    result[16 - 1] = temp ? '1':'0';
}
void increment(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> TMP -> size;
    memset(result, '0', 16);
    result[16-1] = '1';
    printf("result %s\n",result);
    set_reg(alu -> TMP, result);
    result[16-1] = '0';
    addition(alu, result);
    update_flags(alu, result);
}
void decrement(arithmetic_logic_unit *alu, char *result)
{
    int size = alu -> TMP -> size;
    memset(result, '0', 16);
    result[16-1] = '1';
    set_reg(alu -> TMP, result);
    result[16-1] = '0';
    subtraction(alu, result);
    update_flags(alu, result);
}
