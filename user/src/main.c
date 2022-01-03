#include "compiler_defs.h"
#include "C8051F930_defs.h"
#include "peripherals.h"
#include "IR_decoder.h"

const unsigned char low_reload = LOW_RELOAD_0;
const unsigned char high_reload = HIGH_RELOAD_0;

void display_receive();


#pragma OT(1)
void TIMER_ISR() interrupt INTERRUPT_TIMER0 { // 0.2ms interrupt
	IR_timer_run();
	IR_edge_capture();
	
	TL0 = low_reload;
  TH0 = high_reload;
}

void main() {
	PCA0MD = 0;	// watchdog disable
	Init_Device();
	P2=P1=0x00;
	
	while(1) {
		IR_fsm_receive();
		display_receive();
		
	}
}

void display_receive() {
	unsigned char key;
	if(!IR_get_key(&key)) return;
	
	P1 = key;
}
