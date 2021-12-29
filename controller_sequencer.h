#ifndef CONTROLLER_SEQUENCER_H
#define CONTROLLER_SEQUENCER_H

#include "registers.h"
#define CONTROL_WORD_SIZE 33

enum control_word_bits {
    //Enable puts value on WBus (lower 8 bits)
    //Load takes value from WBus (lower 8 bits unless loading to PC or MAR, then all 16 bits)
    C_PC, //increment pc
    Load_PC,
    Enable_PC,
    Load_MAR,
    Load_MDR,
    Enable_MDR,
    Enable_MDR_U, //loading MRD to WBus Upper 8 bits
    Enable_I1,
    Enable_I2,
    Load_ir,
    Load_A,
    Enable_A,
    Load_TMP,
    Enable_TMP,
    Load_B,
    Enable_B,
    Load_C,
    Enable_C,
    Load_O3,
    Load_O4,
    Enable_ADD,
    Enable_SUB,
    Enable_INC,
    Enable_DEC,
    Enbale_AND,
    Enable_OR,
    Enable_XOR,
    Enable_RL, //rotate left
    Enable_RR, //rotate right
    Enable_CMA, //complement accumulator
    Inspect_Sign,
    Inspect_Zero,
    Inspect_Inv //inspect is naturally looking for a high sign or zero flag, reversed when this is high
}

typedef struct controller_sequencer
{
    int ring_counter;
    reg *instruction;
} controller_sequencer;

controller_sequencer* init_controller_sequencer(reg *ins_reg);
void del_controller_sequencer(controller_sequencer *cs);

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
