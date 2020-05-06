#include "base.h"
#include "timer.h"
int timerReady = 0;

void timerInit(void){
	//Enable timer interrupts on C3
	PUT32(0x2000B210, 1 << 3);
	//Setup the time for next interrupt.
	TONI = GET32(SYS_TIMER_CLO);
	TONI += 1000;
	PUT32(SYS_TIMER_C3, TONI);
	return;
}
