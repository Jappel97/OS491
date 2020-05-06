#include "Programs.h"
#include "pi.h"
#include "frame.h"
#include "timer.h"
#include "interrupts.h"
#include "univals.h"
#include "memory.h"
#include "base.h"
#include "controller.h"


unsigned int PID;


//Function Declarations
//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra = 0;
    //unsigned int rb = 0;
    currKey = 0;
    /*
     * For the display I have to use the SPI Interface
     * Pins 16-21 have SPI functions on function Alt4
     * Pins 7-11 have SPI functions on Alt0
     * Pins 7-11 are used by the piTFT
     * Also, pin 25 is used for power
     * The current pinout seems to be as follows:
     * 7 : SPI0_CE1_N to XX
     * 8 : SPI0_CE0_N to 5 : CS (chip select signal) (hold high or low)
     * 9 : SPI0_MISO  to 7 : MISO (Serial Output Signal) (Read bits in, one per clock tick)
     * 10: SPI0_MOSI  to 6 : MOSI (Serial Input Signal) (write bits out, one per clock tick)
     * 11: SPI0_SCLK  to 3 : SCK  (Serial Interface Clock in 4-wire 8-bit serial data interface) (clock ticks)
     * 25: TFT_DC	 to 4 : DC (Command Select)
     * */

    /*
     * I think for the video I want to have a frame buffer which I will write to
     * And then I will translate into the SPI signal
     * My frame buffer should be 96x64 to copy the TI-84.
     * Each entry in my buffer then should be written (320/96) times accross
     * and each row written (240/64) times
     * The frame buffer will be indexed with the origin in the bottom left.
     * That way graphing mode is easier to implement
     * In this case I will then have to read it to SPI with inverted Y coordinates
     *
     * The FB will always be in "graphics" mode, where it takes either a 0xFF or a 0x00 per entry
     * But I will have special "write char" functions to write a 4x8 char at position CharPos
     * A program will be able to write in graphics mode using FB specific functions
     * Or write text using the char functions
     * There should be a char pos advance function
     * */
	
	if(cacheInit()){
		return 1;
	}
	//timerInit();
	//enable_irq();
	
	//Enable Pull Downs for the input Rows.
	PUT32(GPPUD, 0x01);
	for(ra=0;ra<150;ra++) dummy(ra);
	PUT32(GPPUDCLK0, (ROW0 | ROW1 | ROW2 | ROW3 |
		ROW4 | ROW5 | ROW6 | ROW7 |
		ROW8 | ROW9 | ROW10| ROW11));
	for(ra=0;ra<150;ra++) dummy(ra);
	PUT32(GPPUD, 0x00);
	PUT32(GPPUDCLK0, 0x00000000);
	
    //pin 47
    GPFSel(GPFSEL2, 5, 1);
    GPFSel(GPFSEL4, 7, 1);

    //Pull 25 high
    PUT32(GPSET0, (1 << 25));
    spi_init();
    ControllerInit();
    GFXColor(0xFF);
    GFXColor(0x00);
    ColInit();
    RowInit();
    
	FBWriteChar('0');
	memInit();
    
    while(1)
    {
		//TODO
		//On enter, we process the input via Reverse polish notation or some shit
		//Maybe a modified RPN which is more like LISP (lmao imagine using LISP unironically #schemegang)
		shell();
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
