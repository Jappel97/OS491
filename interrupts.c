#include "timer.h"
#include "pi.h"
#include "univals.h"
#include "base.h"
#include "frame.h"

unsigned int counter = 0;

void c_irq_handler ( void )
{
    unsigned int ra;
	ra = GET32(0x2000B204);
    //The only interrupts we have enabled are clock interrupts
    //Which should occur every 10000 cycles
	//TODO: Make an interrupt handler
	if(ra == (1 << 3)){
		TONI += 10000;
		PUT32(SYS_TIMER_C3, TONI);
		PUT32(SYS_TIMER_CTL, (1 << 3));
		//FBRender();
		return;
	}

}
