#ifndef CONTROLLER_SEQUENCER_H
#define CONTROLLER_SEQUENCER_H

#include "registers.h"
#include "arithmetic_logic_unit.h"
#include "SAP2_components.h"
#define CONTROL_WORD_SIZE 32

enum control_word_bits {
    //Enable puts value on WBus (lower 8 bits)
    //Load takes value from WBus (lower 8 bits unless loading to PC or MAR, then all 16 bits)
    Upper_Load,    //0     for enabling or loading 
    Upper_Enable,  
    C_PC,          //     increment pc
    Load_PC,       
    Load_PC_S,     //4
    Enable_PC,     
    Load_MAR,
    Load_MDR,
    Enable_MDR,    //8
    Enable_IN,
    Load_ir,
    Load_ir_P,
    Load_A,        //12
    Enable_A,
    Load_TMP,
    Enable_TMP,
    Load_B,        //16
    Enable_B,
    Load_C,
    Enable_C,
    Load_OUT,      //20
    Enable_ADD,
    Enable_SUB,
    Enable_INC,
    Enable_DEC,    //24
    Enable_AND,
    Enable_OR,
    Enable_XOR,
    Enable_RL,     //28    rotate left
    Enable_RR,     //      rotate right
    Enable_CM,     //      complement accumulator
};

typedef struct controller_sequencer
{
    int ring_counter;
    int *sign_signal;
    int *zero_signal;
    reg *instruction;
    reg *port;
} controller_sequencer;

controller_sequencer* init_controller_sequencer(instruction_reg *ir, arithmetic_logic_unit *alu);
void del_controller_sequencer(controller_sequencer *cs);

void get_control_word_fetch(controller_sequencer *cs, char *control_word);
void get_control_word_ADD_B(controller_sequencer *cs, char *control_word);
void get_control_word_ADD_C(controller_sequencer *cs, char *control_word);
void get_control_word_ANA_B(controller_sequencer *cs, char *control_word);
void get_control_word_ANA_C(controller_sequencer *cs, char *control_word);
void get_control_word_ANI(controller_sequencer *cs, char *control_word);
void get_control_word_CALL(controller_sequencer *cs, char *control_word);
void get_control_word_CMA(controller_sequencer *cs, char *control_word);
void get_control_word_DCR_A(controller_sequencer *cs, char *control_word);
void get_control_word_DCR_B(controller_sequencer *cs, char *control_word);
void get_control_word_DCR_C(controller_sequencer *cs, char *control_word);
//void get_control_word_HLT(controller_sequencer *cs, char *control_word);
void get_control_word_IN(controller_sequencer *cs, char *control_word);
void get_control_word_INR_A(controller_sequencer *cs, char *control_word);
void get_control_word_INR_B(controller_sequencer *cs, char *control_word);
void get_control_word_INR_C(controller_sequencer *cs, char *control_word);
void get_control_word_JM(controller_sequencer *cs, char *control_word);
void get_control_word_JMP(controller_sequencer *cs, char *control_word);
void get_control_word_JNZ(controller_sequencer *cs, char *control_word);
void get_control_word_JZ(controller_sequencer *cs, char *control_word);
void get_control_word_LDA(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_AB(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_AC(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_BA(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_BC(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_CA(controller_sequencer *cs, char *control_word);
void get_control_word_MOV_CB(controller_sequencer *cs, char *control_word);
void get_control_word_MVI_A(controller_sequencer *cs, char *control_word);
void get_control_word_MVI_B(controller_sequencer *cs, char *control_word);
void get_control_word_MVI_C(controller_sequencer *cs, char *control_word);
void get_control_word_NOP(controller_sequencer *cs, char *control_word);
void get_control_word_ORA_B(controller_sequencer *cs, char *control_word);
void get_control_word_ORA_C(controller_sequencer *cs, char *control_word);
void get_control_word_ORI(controller_sequencer *cs, char *control_word);
void get_control_word_OUT(controller_sequencer *cs, char *control_word);
void get_control_word_RAL(controller_sequencer *cs, char *control_word);
void get_control_word_RAR(controller_sequencer *cs, char *control_word);
void get_control_word_RET(controller_sequencer *cs, char *control_word);
void get_control_word_STA(controller_sequencer *cs, char *control_word);
void get_control_word_SUB_B(controller_sequencer *cs, char *control_word);
void get_control_word_SUB_C(controller_sequencer *cs, char *control_word);
void get_control_word_XRA_B(controller_sequencer *cs, char *control_word);
void get_control_word_XRA_C(controller_sequencer *cs, char *control_word);
void get_control_word_XRI(controller_sequencer *cs, char *control_word);    
    
#endif
