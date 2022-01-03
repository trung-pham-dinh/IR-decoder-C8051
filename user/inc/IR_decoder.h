#ifndef IR_DECODER
#define IR_DECODER

#define IR_ZERO_CODE 	0x01F6E11E
#define IR_ONE_CODE  	0x01F7D02E
#define IR_TWO_CODE  	0x01F6D12E
#define IR_THREE_CODE	0x01F750AE
#define IR_FOUR_CODE   0x01F651AE
#define IR_FIVE_CODE		0x01F7906E
#define IR_SIX_CODE		0x01F6916E
#define IR_SEVEN_CODE	0x01F710EE
#define IR_EIGHT_CODE	0x01F611EE
#define IR_NINE_CODE		0x01F7E01E
#define IR_ON_OFF_CODE 0x01F631CE

#define MAX_KEYS 11

typedef enum {
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	ON_OFF
}IR_KEY;

void IR_edge_capture();
void IR_timer_run();
void IR_fsm_receive();
unsigned char IR_get_key(unsigned char *key);

#endif