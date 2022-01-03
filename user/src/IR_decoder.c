#include "IR_decoder.h"


static volatile unsigned char is_edge;
static volatile unsigned char timer_counter;
static unsigned long frame;
static unsigned char frame_vld;

static unsigned long key_code[MAX_KEYS] = {IR_ZERO_CODE, IR_ONE_CODE, IR_TWO_CODE, IR_THREE_CODE, IR_FOUR_CODE, IR_FIVE_CODE, IR_SIX_CODE, IR_SEVEN_CODE, IR_EIGHT_CODE, IR_NINE_CODE, IR_ON_OFF_CODE};

typedef enum {
	IDLE,
	START_VLD,
	EXTRACT
}State;

void IR_edge_capture() {
	static unsigned char pre_state = 1;
	if(pre_state == 1 && IR_pin == 0) {
		pre_state = 0;
		is_edge = 1;
	}
	else if(pre_state == 0 && IR_pin == 1){
		pre_state = 1;
	}
}

void IR_timer_run() {
	timer_counter++;
}

#pragma OT(1)
void IR_fsm_receive() {
	static unsigned char pre_count;
	static unsigned char index;
	static State state = IDLE;
	
	
	switch(state) {
		case IDLE:
			if(is_edge) {
				is_edge = 0;
				
				pre_count = timer_counter;
				state = START_VLD;
			}
			break;
		case START_VLD:
			if(timer_counter - pre_count > 75) { // uncheck "Enable ANSI integer promotion rule", if not, subtraction result will be 2 bytes instead of 1 byte
				state = IDLE;
			}
			else if(is_edge) {
				is_edge = 0;
				if((timer_counter - pre_count) >= 65) { 
					state = EXTRACT;
					
					index = 0;
					frame = 0;
					frame_vld = 0;
					pre_count = timer_counter;
				}
				else {
					state = IDLE;
				}
			}
			break;
		case EXTRACT:
			if(timer_counter - pre_count > 12) {
				state = IDLE;
				frame_vld = 0;
			}
			else if(is_edge) {
				is_edge = 0;
				
				frame |= (timer_counter - pre_count > 9);
				frame <<= 1;
				pre_count = timer_counter;
				index++;
				
				if(index >= 32){
					frame_vld = 1;
					state = IDLE;
				}
			}
			break;
		default:
			break;
	}
}

unsigned char IR_get_key(unsigned char *key) {
	unsigned char i;
	if(!frame_vld) return 0;
	
	for(i=0; i < MAX_KEYS; i++) {
		if(frame == key_code[i]) {
			*key = i;
		}
	}
	return 1;
}