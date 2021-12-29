#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "instructions.h"

#define MEMSIZE 0xFFFF

//TODO: write clock_tick_up() for passing around register values based on high bits in the controlbus
//Question: should the instruction register store the other bytes (typically locations) for multi-byte ops
//Question: how should the program to decide what input/output port to interact with for IN/OUT op
//Question: how should the program know to store the current program counter in the last mem loc for CALL op
//TODO: write ALU struct and logic functions
//TODO: write parsing function load_instructions() to tranlate assembly into machine code

typedef reg port;

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
void load_instructions(char*, computer*);

int machine_cycle(computer*);

void run_program(computer*);

int main(int argc, char **argv)
{
    if (argc < 2) {printf("Error: please enter a file for SAP-2 to run\n"); return 1;}
    char *program = argv[1];
    computer *SAP2 = (computer*)malloc(sizeof(computer));
    boot_computer(SAP2);
    load_instructions(program, SAP2);
    run_program(SAP2);
    shutdown_computer(SAP2);
    
    return 0;
}

void boot_computer(computer *c)
{
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
    c -> ir = init_instruction_reg(8);
    c -> Mem = init_memory(MEMSIZE);
    c -> WBus = init_reg(16);
    c -> ControlBus = init_reg(CONTROL_WORD_SIZE); ///perhaps use bitfield for control bus
    c -> ALU = init_arithmetic_logic_unit(c -> Accumulator, c -> TMP);
    c -> cs = init_controller_sequencer(c -> ir -> highNibble);
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
    free(c -> ALU);
    free(c -> cs);
    free(c);
}

void load_instructions(char *filename, computer *c)
{
    printf("load_instructions: to be implemented\n");
}

void clock_tick_up(computer *c)
{
    printf("clock_tick_up: to be implemennted\n");
}

int machine_cycle(computer *c)
{
    char *updated_cw = malloc(sizeof(c -> ControlBus));
    if (c -> cs -> rc < 3) get_control_word_fetch(c -> cs, updated_cw);
    else {
	char *ins = malloc(sizeof(c -> cs -> instruction));
	get_reg(c -> cs -> instruction, 8, 0, ins);
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
    return 0;
}

void run_progrma(computer *c)
{
    for(;;) if (machine_cycle(c)) break;
    //perhaps add functions that take inputs here
}
