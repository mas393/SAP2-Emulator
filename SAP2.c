#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registers.h"
#include "memory.h"
#include "instructions.h"
#include "controller_sequencer.h"
#include "arithmetic_logic_unit.h"
#include "SAP2_components.h"

#define MEMSIZE 0xFFFF
#define STORE_LOC 0xFFFD //store addr at 0xFFFD and 0xFFFE b/c mem starts at block 0

//TODO: write ALU logic functions
//TODO: make alu functions reset and modify alu flags
//TODO: check load_computer for each instruction

typedef struct computer
{
    program_counter PC;
    reg *Accumulator;
    reg *B;
    reg *C;
    reg *TMP;
    reg *MAR;
    reg *MDR;
    port *Input1;
    port *Input2;
    port *Output3;
    port *Output4;
    instruction_reg *ir;
    memory *Mem;
    bus *WBus;
    bus *ControlBus;
    arithmetic_logic_unit *ALU;
    controller_sequencer *cs;
} computer;

void boot_computer(computer*);
void shutdown_computer(computer*);
int load_instructions(char*, computer*);
int machine_cycle(computer*);
void run_program(computer*);

int main(int argc, char **argv)
{
    if (argc < 2) {printf("Error: please enter a file for SAP-2 to run\n"); return 1;}
    char *program = argv[1];
    computer *SAP2 = (computer*)malloc(sizeof(computer));
    boot_computer(SAP2);
    if(load_instructions(program, SAP2)) return 1;
    //run_program(SAP2);
    shutdown_computer(SAP2);
    
    return 0;
}

void boot_computer(computer *c)
{
    //shold we initialize all registers to zero?
    c -> PC = 0;
    c -> Accumulator = init_reg(8);
    c -> B = init_reg(8);
    c -> C = init_reg(8);
    c -> TMP = init_reg(8);
    c -> Input1 = init_reg(8);
    c -> Input2 = init_reg(8);
    c -> Output3 = init_reg(8);
    c -> Output4 = init_reg(8);
    c -> MAR = init_reg(16);
    c -> MDR = init_reg(8);
    c -> ir = init_instruction_reg();
    c -> Mem = init_mem(MEMSIZE);
    c -> WBus = init_reg(16);
    c -> ControlBus = init_reg(CONTROL_WORD_SIZE); ///perhaps use bitfield for control bus
    c -> ALU = init_arithmetic_logic_unit(c -> Accumulator, c -> TMP);
    c -> cs = init_controller_sequencer(c -> ir, c -> ALU);
}

void shutdown_computer(computer *c)
{
    del_reg(c -> Accumulator);
    del_reg(c -> B);
    del_reg(c -> C);
    del_reg(c -> TMP);
    del_reg(c -> Input1);
    del_reg(c -> Input2);
    del_reg(c -> Output3);
    del_reg(c -> Output4);
    del_reg(c -> MAR);
    del_reg(c -> MDR);
    del_instruction_reg(c -> ir);
    del_mem(c -> Mem);
    del_reg(c -> WBus);
    del_reg(c -> ControlBus);
    del_arithmetic_logic_unit(c -> ALU);
    del_controller_sequencer(c -> cs);
    free(c);
}

int load_instructions(char *filename, computer *c)
{   
    FILE *fin = fopen(filename, "r");
    fseek(fin, 0, SEEK_END);
    int fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    struct label_addr{
	char *label;
	int addr;
    };

    struct label_addr *label_list = (struct label_addr*)malloc(fsize * sizeof(struct label_addr));
    int label_list_len = 0;

    int addr = 0x0800;
    char *line = (char*)malloc(50);
    char *line2 = (char*)malloc(50);
    char label_t[2] = ":";
    char *label;
    char ins_t[2] = " ";
    char *instruction;// = (char*)malloc(5);
    char args_t[2] = ",";
    char *args;
    char *arg1, *arg2;
    char *opcode = (char*)malloc(8);
    char *data8 = (char*)malloc(8);
    char *data16 = (char*)malloc(8);
    int data_flag = 0;
    int line_num = 1;
    
    while (fgets(line, 50, fin)) {
	memcpy(line2, line, 50);
	label = strtok(line2, label_t);
	//	printf("label = %s, line2 = %s\n", label, line2);
	if (strlen(label) < strlen(line)) {
	    //must store label and addr pair for lookup
	    char *label_cpy = (char *)malloc(sizeof(label));
	    memcpy(label_cpy, label, sizeof(label));
	    struct label_addr la;
	    la.label = label_cpy;
	    la.addr = addr;
	    label_list[label_list_len] = la;
	    label_list_len++;
	
	    //printf("label = %s\n", label);
	    //line += strlen(label);
	    line = strtok(NULL, label_t); // the instruction will be the second token iff label
	}
	line[strlen(line) - 1] = '\0'; //strip newline character
	instruction = strtok(line, ins_t);
	args = strtok(NULL, ins_t);
	//perhaps have to inspect first argument to see if it is key in label key/val pairs and replace arg1
	arg1 = strtok(args, args_t); //first argument
	arg2 = strtok(NULL, args_t);

	printf("ins = %s\n", instruction); 

	if (!strcmp(instruction, "ADD")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "B")) memcpy(opcode, ADD_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, ADD_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }
	}
	else if (!strcmp(instruction, "ANA")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "B")) memcpy(opcode, ANA_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, ANA_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }
	}
	else if (!strcmp(instruction, "ANI")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, ANI, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0xFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}
	else if (!strcmp(instruction, "CALL")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, CALL, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);

	    for (int i = 0; i < label_list_len; i++) {
		if (!strcmp(arg1, label_list[i].label)) temp = label_list[i].addr;
	    }

	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	    //  printf("temp = %d, DATA8 = %s, data16 = %s\n", temp, data8, data16);
	}
	else if (!strcmp(instruction, "CMA")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, CMA, 8);
	}
	else if (!strcmp(instruction, "DCR")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "A")) memcpy(opcode, DCR_A, 8);
	    else if (!strcmp(arg1, "B")) memcpy(opcode, DCR_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, DCR_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }
	}
	else if (!strcmp(instruction, "HLT")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, HLT, 8);
	}
	else if (!strcmp(instruction, "IN")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, IN, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0x4) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}
	else if (!strcmp(instruction, "INR")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "A")) memcpy(opcode, INR_A, 8);
	    else if (!strcmp(arg1, "B")) memcpy(opcode, INR_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, INR_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }
	}
	else if (!strcmp(instruction, "JM")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, JM, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);

	    for (int i = 0; i < label_list_len; i++) {
		if (!strcmp(arg1, label_list[i].label)) temp = label_list[i].addr;
	    }
	    
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "JMP")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, JMP, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);

	    for (int i = 0; i < label_list_len; i++) {
		if (!strcmp(arg1, label_list[i].label)) temp = label_list[i].addr;
	    }
	    
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "JNZ")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, JNZ, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);

	    for (int i = 0; i < label_list_len; i++) {
		if (!strcmp(arg1, label_list[i].label)) temp = label_list[i].addr;
	    }
	    
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "JZ")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, JZ, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);

	    for (int i = 0; i < label_list_len; i++) {
		if (!strcmp(arg1, label_list[i].label)) temp = label_list[i].addr;
	    }
	    
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "LDA")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, LDA, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "MOV")) {
	    if (!strcmp(arg1, "A") && !(strcmp(arg2, "B"))) memcpy(opcode, MOV_AB, 8);
	    else if (!strcmp(arg1, "A") && !(strcmp(arg2, "C"))) memcpy(opcode, MOV_AC, 8);
	    else if (!strcmp(arg1, "B") && !(strcmp(arg2, "A"))) memcpy(opcode, MOV_BA, 8);
	    else if (!strcmp(arg1, "B") && !(strcmp(arg2, "C"))) memcpy(opcode, MOV_BC, 8);
	    else if (!strcmp(arg1, "C") && !(strcmp(arg2, "A"))) memcpy(opcode, MOV_CA, 8);
	    else if (!strcmp(arg1, "C") && !(strcmp(arg2, "B"))) memcpy(opcode, MOV_CB, 8);
	    else {
		printf("%d: parsing Error: arguments %s, %s not recognized for instruction %s\n",
		       line_num, arg1, arg2, instruction);
		return 1;
	    }
	}
	else if (!strcmp(instruction, "MVI")) {
	    //perhaps check imidiate val first
	    if (!strcmp(arg1, "A")) memcpy(opcode, MVI_A, 8);
	    else if (!strcmp(arg1, "B")) memcpy(opcode, MVI_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, MVI_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }
	    int temp;
	    sscanf(arg2, "%xH", &temp);
	    if (temp > 0xFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}
	else if (!strcmp(instruction, "NOP")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, NOP, 8);
	}
	else if (!strcmp(instruction, "ORA")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "B")) memcpy(opcode, ORA_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, ORA_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }	    	    
	}
	else if (!strcmp(instruction, "ORI")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, ORI, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0xFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}
	else if (!strcmp(instruction, "OUT")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, OUT, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0x4) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}
	else if (!strcmp(instruction, "RAL")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, RAL, 8);
	}
	else if (!strcmp(instruction, "RAR")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, RAR, 8);
	}
	else if (!strcmp(instruction, "RET")) {
	    if (arg1 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, RET, 8);
	}
	else if (!strcmp(instruction, "STA")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, STA, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0xFFFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    bit_string_from_int((temp & 0xFF), data8, 8);
	    bit_string_from_int((temp >> 8 & 0xFF), data16, 8);
	    data_flag = 2;
	}
	else if (!strcmp(instruction, "SUB")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "B")) memcpy(opcode, SUB_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, SUB_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }	    	    
	}
	else if (!strcmp(instruction, "XRA")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    if (!strcmp(arg1, "B")) memcpy(opcode, XRA_B, 8);
	    else if (!strcmp(arg1, "C")) memcpy(opcode, XRA_C, 8);
	    else {
		printf("%d: parsing Error: arguments %s not recognized for instruction %s\n",
		       line_num, arg1, instruction);
		return 1;
	    }	    	    
	}
	else if (!strcmp(instruction, "XRI")) {
	    if (arg2 != NULL) {
		printf("%d: parsing error: too many arguments for instruction %s\n", line_num, instruction);
		return 1;
	    }
	    memcpy(opcode, XRI, 8);
	    int temp;
	    sscanf(arg1, "%xH", &temp);
	    if (temp > 0xFF) {
		printf("%d: parsing error: data byte is too large for instruction %s\n", line_num, instruction);
		return 1;
	    }				
	    bit_string_from_int(temp, data8, 8);
	    data_flag = 1;
	}

	char *addr_str = malloc(16);
	bit_string_from_int(addr, addr_str, 16);
	printf("opcode = %s\n", opcode);
	set_mem(c -> Mem, addr_str, opcode);
	if (data_flag == 2) {
	    addr++;
	    bit_string_from_int(addr, addr_str, 16);
	    set_mem(c -> Mem, addr_str, data16);
	    addr++;
	    bit_string_from_int(addr, addr_str, 16);
	    set_mem(c -> Mem, addr_str, data8);
	}
	else if (data_flag == 1) {
	    addr++;
	    bit_string_from_int(addr, addr_str, 16);
	    set_mem(c -> Mem, addr_str, data8);
	}	
	data_flag = 0;
	addr++;
	line_num++;
    }
    //    free(line);
    //    free(label);
    //    free(instruction);
    //    free(args);
    //    free(arg1);
    //    free(arg2);
    //    free(opcode);
    //    free(data);

    fclose(fin);
    char *add_r = malloc(16);
    bit_string_from_int(0x0800, add_r, 16);
    print_mem(c -> Mem, add_r, 10);
    return 0;
}

void clock_tick_up(computer *c)
{
    if (get_reg_bit(c -> ControlBus, C_PC)) c -> PC++;
    
    //set bus
    char *bus = (char*)malloc(sizeof(c -> WBus));
    memcpy(bus, c -> WBus, sizeof(c -> WBus));
    //should the actual bus be copied into the bus declared above to preserve the bits?
    //there mus be a better way than repeated if else checking controlbus
    int offset = 0;
    if (get_reg_bit(c -> ControlBus, Upper_Enable)) offset = 8;
    
    if (get_reg_bit(c -> ControlBus, Enable_PC)) get_PC(c -> PC, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_MDR)) get_reg(c -> MDR, bus, offset);
    else if (get_reg_bit(c -> ControlBus, Enable_IN)) {
	int p = get_reg_bit(c -> cs -> port, 0)
	    + get_reg_bit(c -> cs -> port, 1)*2
	    + get_reg_bit(c -> cs -> port, 2)*4;
	switch(p) {
	case 1:
	    get_reg(c -> Input1, bus, offset);
	    break;
	case 2:
	    get_reg(c -> Input2, bus, offset);
	    break;
	default:
	    printf("input register does not exist\n");
	}
    }
    else if (get_reg_bit(c -> ControlBus, Enable_A)) get_reg(c -> Accumulator, bus, offset);
    else if (get_reg_bit(c -> ControlBus, Enable_TMP)) get_reg(c -> TMP, bus, offset);
    else if (get_reg_bit(c -> ControlBus, Enable_B)) get_reg(c -> B, bus, offset);
    else if (get_reg_bit(c -> ControlBus, Enable_C)) get_reg(c -> C, bus, offset);
    // would there be a better way of checking all the ALU functions as regardless of function the ALU outputs to the bus
    else if (get_reg_bit(c -> ControlBus, Enable_ADD)) addition(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_SUB)) subtraction(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_INC)) increment(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_DEC)) decrement(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_AND)) and(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_OR)) or(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_XOR)) xor(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_RL)) rotate_right(c -> ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_RR)) rotate_left(c ->ALU, bus);
    else if (get_reg_bit(c -> ControlBus, Enable_CM)) complement(c -> ALU, bus);
    //else bus is the same as last time
    set_reg(c -> WBus, bus);
    free(bus);
    
    //get bus
    char *r = (char*)malloc(sizeof(c -> Accumulator)); // we know most  registers are 8 bits
    offset = 0;
    if (get_reg_bit(c -> ControlBus, Upper_Load)) offset = 8;
    get_reg(c -> WBus, r, offset);
    
    if (get_reg_bit(c -> ControlBus, Load_PC)) {
	r = realloc(r, sizeof(c -> PC));
	get_reg(c -> WBus, r, 0);
	set_PC(c -> PC, r);
    }
    else if (get_reg_bit(c -> ControlBus, Load_PC_S)) set_PC_int(c -> PC, STORE_LOC);
    else if (get_reg_bit(c -> ControlBus, Load_MAR)) {
	r = realloc(r, sizeof(c -> MAR));
	get_reg(c -> WBus, r, 0);
	set_reg(c -> MAR, r);
    }
    else if (get_reg_bit(c -> ControlBus, Load_MDR)) set_reg(c -> MDR, r);
    else if (get_reg_bit(c -> ControlBus, Load_ir)) set_reg(c -> ir -> instruction, r);
    else if (get_reg_bit(c -> ControlBus, Load_ir_P)) set_reg(c -> ir -> port, r);
    else if (get_reg_bit(c -> ControlBus, Load_A)) set_reg(c -> Accumulator, r);
    else if (get_reg_bit(c -> ControlBus, Load_TMP)) set_reg(c -> TMP, r);
    else if (get_reg_bit(c -> ControlBus, Load_B)) set_reg(c -> B, r);
    else if (get_reg_bit(c -> ControlBus, Load_C)) set_reg(c -> C, r);
    else if (get_reg_bit(c -> ControlBus, Load_OUT)) {
	int p = get_reg_bit(c -> cs -> port, 0)
	    + get_reg_bit(c -> cs -> port, 1)*2
	    + get_reg_bit(c -> cs -> port, 2)*4;
	switch(p) {
	case 3:
	    set_reg(c -> Output3, r);
	    break;
	case 4:
	    set_reg(c -> Output4, r);
	    break;
	default:
	    printf("output register does not exist\n");
	}
    }
    free(r);
								    
}

int machine_cycle(computer *c)
{
    char *updated_cw = (char*)malloc(CONTROL_WORD_SIZE);
    memset(updated_cw, '0', CONTROL_WORD_SIZE);
    if (c -> cs -> ring_counter < 3) get_control_word_fetch(c -> cs, updated_cw);
    else {
	char *ins = (char*)malloc(sizeof(c -> cs -> instruction));
	get_reg(c -> cs -> instruction, ins, 0);
	if (!strcmp(ins, HLT)) {
	    free(ins);
	    free(updated_cw);
	    return 1;
	}
	else if (!strcmp(ins, ADD_B))  get_control_word_ADD_B(c -> cs, updated_cw);
	else if (!strcmp(ins, ADD_C))  get_control_word_ADD_C(c -> cs, updated_cw);
	else if (!strcmp(ins, ANA_C))  get_control_word_ANA_C(c -> cs, updated_cw);
	else if (!strcmp(ins, ANI))    get_control_word_ANI(c -> cs, updated_cw);
	else if (!strcmp(ins, CALL))   get_control_word_CALL(c -> cs, updated_cw);
	else if (!strcmp(ins, CMA))    get_control_word_CMA(c -> cs, updated_cw);
	else if (!strcmp(ins, DCR_A))  get_control_word_DCR_A(c -> cs, updated_cw);
	else if (!strcmp(ins, DCR_B))  get_control_word_DCR_B(c -> cs, updated_cw);
	else if (!strcmp(ins, DCR_C))  get_control_word_DCR_C(c -> cs, updated_cw);
	else if (!strcmp(ins, IN))     get_control_word_IN(c -> cs, updated_cw);
	else if (!strcmp(ins, INR_A))  get_control_word_INR_A(c -> cs, updated_cw);
	else if (!strcmp(ins, INR_B))  get_control_word_INR_B(c -> cs, updated_cw);
	else if (!strcmp(ins, INR_C))  get_control_word_INR_C(c -> cs, updated_cw);
	else if (!strcmp(ins, JM))     get_control_word_JM(c -> cs, updated_cw);
	else if (!strcmp(ins, JMP))    get_control_word_JMP(c -> cs, updated_cw);
	else if (!strcmp(ins, JNZ))    get_control_word_JNZ(c -> cs, updated_cw);
	else if (!strcmp(ins, JZ))     get_control_word_JZ(c -> cs, updated_cw);
	else if (!strcmp(ins, LDA))    get_control_word_LDA(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_AB)) get_control_word_MOV_AB(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_AC)) get_control_word_MOV_AC(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_BA)) get_control_word_MOV_BA(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_BC)) get_control_word_MOV_BC(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_CA)) get_control_word_MOV_CA(c -> cs, updated_cw);
	else if (!strcmp(ins, MOV_CB)) get_control_word_MOV_BC(c -> cs, updated_cw);
	else if (!strcmp(ins, MVI_A))  get_control_word_MVI_A(c -> cs, updated_cw);
	else if (!strcmp(ins, MVI_B))  get_control_word_MVI_B(c -> cs, updated_cw);
	else if (!strcmp(ins, MVI_C))  get_control_word_MVI_C(c -> cs, updated_cw);
	else if (!strcmp(ins, NOP))    get_control_word_NOP(c -> cs, updated_cw);
	else if (!strcmp(ins, ORA_B))  get_control_word_ORA_B(c -> cs, updated_cw);
	else if (!strcmp(ins, ORA_C))  get_control_word_ORA_C(c -> cs, updated_cw);
	else if (!strcmp(ins, ORI))    get_control_word_ORI(c -> cs, updated_cw);
	else if (!strcmp(ins, OUT))    get_control_word_OUT(c -> cs, updated_cw);
	else if (!strcmp(ins, RAL))    get_control_word_RAL(c -> cs, updated_cw);
	else if (!strcmp(ins, RAR))    get_control_word_RAR(c -> cs, updated_cw);
	else if (!strcmp(ins, RET))    get_control_word_RET(c -> cs, updated_cw);
	else if (!strcmp(ins, STA))    get_control_word_STA(c -> cs, updated_cw);
	else if (!strcmp(ins, SUB_B))  get_control_word_SUB_B(c -> cs, updated_cw);
	else if (!strcmp(ins, SUB_C))  get_control_word_SUB_C(c -> cs, updated_cw);
	else if (!strcmp(ins, XRA_B))  get_control_word_XRA_B(c -> cs, updated_cw);
	else if (!strcmp(ins, XRA_C))  get_control_word_XRA_C(c -> cs, updated_cw);
	else if (!strcmp(ins, XRI))    get_control_word_XRI(c -> cs, updated_cw);
	else printf("Error in machine cycle: instruction %s is unknown\n", ins);
	free(ins);
    }
    set_reg(c -> ControlBus, updated_cw);
    free(updated_cw);
    clock_tick_up(c);
    c -> cs -> ring_counter++;
    
    return 0;
}

void run_program(computer *c)
{
    for(;;) if (machine_cycle(c)) break;
    //perhaps add functions that take inputs here
}

