#include <stdio.h>
#include <stdlib.h>
#include "controller_sequencer.h"


//TODO: implement get_control_word_CALL
//TODO: implement get_control_word_RET
//TODO: implement get_control_word_IN
//TODO: implement get_control_word_OUT

controller_sequencer* init_controller_sequencer(reg *ins_reg, arithmetic_logic_unit *ALU)
{
    controller_sequencer *cs = (controller_sequencer*)malloc(sizeof(controller_sequencer));
    cs -> ring_counter = 0;
    cs -> instruction = ins_reg;
    cs -> zero_signal = &(ALU -> zero_flag);
    cs -> sign_signal = &(ALU -> sign_flag);
    return cs;
}
    
void del_controller_sequencer(controller_sequencer *cs)
{
    free(cs);
}

void get_control_word_fetch(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 0:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 1:
	cw[C_PC] = '1';
	break;
    case 2:
	cw[Enable_MDR] = '1';
	cw[Load_ir] = '1';
	break;
    default:
	printf("should not be here control_word_fetch\n");
    };    
}

void get_control_word_ADD_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_ADD] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ADD_B\n");
    };    
}

void get_control_word_ADD_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_ADD] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ADD_C\n");
    };    
}

void get_control_word_ANA_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_AND] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ANA_B\n");
    };    
}

void get_control_word_ANA_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_AND] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ANA_C\n");
    };    
}

void get_control_word_ANI(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_TMP] = '1';
       	break;
    case 6:
	cw[Enable_AND] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ANI\n");
    };    
}

void get_control_word_CALL(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    default:
	printf("Implement control_word_CALL\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_CMA(controller_sequencer *cs, char *cw)
{
    //perhaps have to perform CMA on the tmp so that its more general
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_CM] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_CMA\n");
    };    
}

void get_control_word_DCR_A(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_DEC] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_DCR_A\n");
    };    
}

void get_control_word_DCR_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_DEC] = '1';
	cw[Load_B] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_DCR_B\n");
    };    
}

void get_control_word_DCR_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_DEC] = '1';
	cw[Load_C] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_DCR_C\n");
    };    
}

void get_control_word_IN(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    default:
	printf("implement control_word_IN\n");
    };    
}

void get_control_word_INR_A(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_INC] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_INR_A\n");
    };    
}

void get_control_word_INR_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_INC] = '1';
	cw[Load_B] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_INR_B\n");
    };    
}

void get_control_word_INR_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_INC] = '1';
	cw[Load_C] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_INR_C\n");
    };    
}

void get_control_word_JM(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5: //assuming msb then lsb in memory
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	if (*(cs -> sign_signal) != 1) {
	    cs -> ring_counter = 0;
	    return;
	}
	break;
	//	cw[Inspect_Sign] = '1';
    case 9:
	cw[Enable_MDR] = '1';
	cw[Load_PC] = '1';
	cs -> ring_counter = 0;
	return;	    
    default:
	printf("should not be here control_word_JM\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_JMP(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5: //assuming msb then lsb in memory
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	cw[Enable_MDR] = '1';
	cw[Load_PC] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_JMP\n");
    };    
}

void get_control_word_JNZ(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5: //assuming msb then lsb in memory
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	if (*(cs -> zero_signal) != 0) {
	    cs -> ring_counter = 0;
	    return;
	}
	break;
	//	cw[Inspect_Zero] = '1';
	//	cw[Inspect_Inv] = '1';
    case 9:
	cw[Enable_MDR] = '1';
	cw[Load_PC] = '1';
	cs -> ring_counter = 0;
	return;	    
    default:
	printf("should not be here control_word_JNZ\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_JZ(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5: //assuming msb then lsb in memory
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	if (*(cs -> zero_signal) == 0) {
	    cs -> ring_counter = 0;
	    return;
	}
	break;
	//	cw[Inspect_Zero] = '1';
    case 9:
	cw[Enable_MDR] = '1';
	cw[Load_PC] = '1';
	cs -> ring_counter = 0;
	return;	    
    default:
	printf("should not be here control_word_JZ\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_LDA(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	cw[Enable_MDR] = '1';
	cw[Load_MAR] = '1';
	break;
    case 9:
	cw[Enable_MDR] = '1';
	cw[Load_A] = '1';
    default:
	printf("Implement control_word_LDA\n");
	cs -> ring_counter = 0;
	return;

    };    
}

void get_control_word_MOV_AB(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_AB\n");
    };    
}

void get_control_word_MOV_AC(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_AC\n");
    };    
}

void get_control_word_MOV_BA(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_B] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_BA\n");
    };    
}

void get_control_word_MOV_BC(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Enable_B] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_BC\n");
    };    
}

void get_control_word_MOV_CA(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_C] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_CA\n");
    };    
}

void get_control_word_MOV_CB(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_C] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MOV_CB\n");
    };    
}

void get_control_word_MVI_A(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MVI_A\n");
    };    
}

void get_control_word_MVI_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_B] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MVI_B\n");
    };    
}

void get_control_word_MVI_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_C] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_MVI_C\n");
    };    
}

void get_control_word_NOP(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_NOP\n");
    };    
}

void get_control_word_ORA_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_OR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ORA_B\n");
    };    
}

void get_control_word_ORA_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';	
	break;
    case 4:
	cw[Enable_OR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ORA_C\n");
    };    
}

void get_control_word_ORI(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_TMP] = '1';
	break;
    case 6:
	cw[Enable_OR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_ORI\n");
    };    
}

void get_control_word_OUT(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    default:
	printf("implement control_word_OUT\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_RAL(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_RL] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_RAL\n");
    };    
}

void get_control_word_RAR(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_A] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_RR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_RAR\n");
    };    
}

void get_control_word_RET(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    default:
	printf("implement control_word_RET\n");
	cs -> ring_counter = 0;
	return;
    };    
}

void get_control_word_STA(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR_U] = '1';
	break;
    case 6:
	cw[Enable_PC] = '1';
	cw[Load_MAR] = '1';
	break;
    case 7:
	cw[C_PC] = '1';
	break;
    case 8:
	cw[Enable_MDR] = '1';
	cw[Load_MAR] = '1';
	break;
    case 9:
	cw[Enable_A];
	cw[Load_MDR];
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_STA\n");
    };    
}

void get_control_word_SUB_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_SUB] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_SUB_B\n");
    };    
}

void get_control_word_SUB_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_SUB] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_SUB_C\n");
    };    
}

void get_control_word_XRA_B(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_B] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_XOR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_XRA_C\n");
    };    
}

void get_control_word_XRA_C(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_TMP] = '1';
	break;
    case 4:
	cw[Enable_XOR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_XRA_C\n");
    };    
}

void get_control_word_XRI(controller_sequencer *cs, char *cw)
{
    switch(cs -> ring_counter) {
    case 3:
	cw[Enable_C] = '1';
	cw[Load_MAR] = '1';
	break;
    case 4:
	cw[C_PC] = '1';
	break;
    case 5:
	cw[Enable_MDR] = '1';
	cw[Load_TMP] = '1';
	break;
    case 6:
	cw[Enable_XOR] = '1';
	cw[Load_A] = '1';
	cs -> ring_counter = 0;
	return;
    default:
	printf("should not be here control_word_XRI\n");
    };    
}
