
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );


#include <stdarg.h>
#include "Font.h"

#define GPFSEL0 0x20200000 //pins 0-9
#define GPFSEL1 0x20200004 //pins 10-19
#define GPFSEL2 0x20200008 //pins 20-29
#define GPFSEL3 0x2020000C //pins 30-29
#define GPFSEL4 0x20200010 //pins 40-49
#define GPSET0  0x2020001C //SET pins 0-31
#define GPSET1  0x20200020 //SET pins 32-53
#define GPCLR0  0x20200028 //CLEAR pins 0-31
#define GPCLR1  0x2020002C //CLEAR pins 32-53
#define GPLEV0	0x20200034 //Read state of pins 0-31
#define GPLEV1	0x20200038 //Read state of pins 32-53
#define GPEDS0	0x20200040 //Detect Event for pins 0-31
#define GPEDS1	0x20200044 //Detect Event for pins 32-53
#define GPHEN0	0x20200064 //Enable High Event for pins 0-31
#define GPHEN1	0x20200068 //Enable High Event for pins 32-53
#define GPPUD	0x20200094
#define GPPUDCLK0	0x20200098
#define GPPUDCLK1	0x202000b0

#define AUX_ENABLES     0x20215004
#define AUX_SPI0_CS     0x20204000
#define AUX_SPI0_FIFO   0x20204004
#define AUX_SPI0_CLK    0x20204008
#define AUX_SPI0_DLEN   0x2020400C
#define AUX_SPI0_LTOH   0x20204010
#define AUX_SPI0_DC     0x20204014

/* Level 1 Commands (from the display Datasheet) */
#define ILI9341_CMD_NOP					0x00
// 0x02
#define ILI9341_CMD_SOFTWARE_RESET			0x01
#define ILI9341_CMD_READ_DISP_ID			0x04
#define ILI9341_CMD_READ_DISP_STATUS			0x09
#define ILI9341_CMD_READ_DISP_POWER_MODE		0x0A
#define ILI9341_CMD_READ_DISP_MADCTRL			0x0B
#define ILI9341_CMD_READ_DISP_PIXEL_FORMAT		0x0C
#define ILI9341_CMD_READ_DISP_IMAGE_FORMAT		0x0D
#define ILI9341_CMD_READ_DISP_SIGNAL_MODE		0x0E
#define ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC		0x0F
#define ILI9341_CMD_ENTER_SLEEP_MODE			0x10
#define ILI9341_CMD_SLEEP_OUT				0x11
#define ILI9341_CMD_PARTIAL_MODE_ON			0x12
#define ILI9341_CMD_NORMAL_DISP_MODE_ON			0x13
#define ILI9341_CMD_DISP_INVERSION_OFF			0x20
#define ILI9341_CMD_DISP_INVERSION_ON			0x21
#define ILI9341_CMD_GAMMA_SET				0x26
#define ILI9341_CMD_DISPLAY_OFF				0x28
#define ILI9341_CMD_DISPLAY_ON				0x29
#define ILI9341_CMD_COLUMN_ADDRESS_SET			0x2A
#define ILI9341_CMD_PAGE_ADDRESS_SET			0x2B
#define ILI9341_CMD_MEMORY_WRITE			0x2C
#define ILI9341_CMD_COLOR_SET				0x2D
#define ILI9341_CMD_MEMORY_READ				0x2E
#define ILI9341_CMD_PARTIAL_AREA			0x30
#define ILI9341_CMD_VERT_SCROLL_DEFINITION		0x33
#define ILI9341_CMD_TEARING_EFFECT_LINE_OFF		0x34
#define ILI9341_CMD_TEARING_EFFECT_LINE_ON		0x35
#define ILI9341_CMD_MEMORY_ACCESS_CONTROL		0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDRESS		0x37
#define ILI9341_CMD_IDLE_MODE_OFF			0x38
#define ILI9341_CMD_IDLE_MODE_ON			0x39
#define ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET		0x3A
#define ILI9341_CMD_WRITE_MEMORY_CONTINUE		0x3C
#define ILI9341_CMD_READ_MEMORY_CONTINUE		0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE			0x44
#define ILI9341_CMD_GET_SCANLINE			0x45
#define ILI9341_CMD_WRITE_DISPLAY_BRIGHTNESS		0x51
#define ILI9341_CMD_READ_DISPLAY_BRIGHTNESS		0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY			0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY			0x54
#define ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS	0x55 //0b 0101 0101
#define ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHTNESS	0x56
#define ILI9341_CMD_WRITE_MIN_CAB_LEVEL			0x5E
#define ILI9341_CMD_READ_MIN_CAB_LEVEL			0x5F
#define ILI9341_CMD_READ_ID1				0xDA
#define ILI9341_CMD_READ_ID2				0xDB
#define ILI9341_CMD_READ_ID3				0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9341_CMD_RGB_SIGNAL_CONTROL			0xB0
#define ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL		0xB1
#define ILI9341_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR	0xB2
#define ILI9341_CMD_FRAME_RATE_CONTROL_PARTIAL		0xB3
#define ILI9341_CMD_DISPLAY_INVERSION_CONTROL		0xB4
#define ILI9341_CMD_BLANKING_PORCH_CONTROL		0xB5
#define ILI9341_CMD_DISPLAY_FUNCTION_CONTROL		0xB6
#define ILI9341_CMD_ENTRY_MODE_SET			0xB7
#define ILI9341_CMD_BACKLIGHT_CONTROL_1			0xB8
#define ILI9341_CMD_BACKLIGHT_CONTROL_2			0xB9
#define ILI9341_CMD_BACKLIGHT_CONTROL_3			0xBA
#define ILI9341_CMD_BACKLIGHT_CONTROL_4			0xBB
#define ILI9341_CMD_BACKLIGHT_CONTROL_5			0xBC
#define ILI9341_CMD_BACKLIGHT_CONTROL_6			0xBD
#define ILI9341_CMD_BACKLIGHT_CONTROL_7			0xBE
#define ILI9341_CMD_BACKLIGHT_CONTROL_8			0xBF
#define ILI9341_CMD_POWER_CONTROL_1			0xC0
#define ILI9341_CMD_POWER_CONTROL_2			0xC1
#define ILI9341_CMD_VCOM_CONTROL_1			0xC5
#define ILI9341_CMD_VCOM_CONTROL_2			0xC7
#define ILI9341_CMD_POWER_ON_SEQ_CONTROL		0xCB
#define ILI9341_CMD_POWER_CONTROL_A			0xCD
#define ILI9341_CMD_POWER_CONTROL_B			0xCF
#define ILI9341_CMD_NVMEM_WRITE				0xD0
#define ILI9341_CMD_NVMEM_PROTECTION_KEY		0xD1
#define ILI9341_CMD_NVMEM_STATUS_READ			0xD2
#define ILI9341_CMD_READ_ID4				0xD3
#define ILI9341_CMD_POSITIVE_GAMMA_CORRECTION		0xE0
#define ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION		0xE1
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_1		0xE2
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_2		0xE3
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_A		0xE8
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_B		0xEA
#define ILI9341_CMD_ENABLE_3_GAMMA_CONTROL		0xF2
#define ILI9341_CMD_INTERFACE_CONTROL			0xF6
#define ILI9341_CMD_PUMP_RATIO_CONTROL	0xF7

#define COL0	(1 << 2)
#define COL1	(1 << 3)
#define COL2	(1 << 4)
#define COL3	(1 << 5)

#define ROW0	(1 << 14) // 1+0, 1+12, 1+24, 1+36
#define ROW1	(1 << 15)
#define ROW2	(1 << 18)
#define ROW3	(1 << 24)

#define ROW4	(1 << 6)
#define ROW5	(1 << 12)
#define ROW6	(1 << 13)
#define ROW7	(1 << 16)

#define ROW8	(1 << 19)
#define ROW9	(1 << 20)
#define ROW10	(1 << 21)
#define ROW11	(1 << 7) //We have to sacrifice CS1 :(

//Global variables def'd
	//frame buffer: 96 wide by 64 tall
	//Accessed as [y][x]
#define maxHeight 320
#define maxWidth 240
#define blockHeight 24 //10 pixel tall
#define blockWidth 40 //8 pixel wide
unsigned char frame[maxHeight][maxWidth]; //We have 8 bits to store color info
unsigned char charX, charY;
static const unsigned int cols[4] = {COL0, COL1, COL2, COL3};
static const unsigned int rows[12] = {ROW0, ROW1, ROW1, ROW3,
	ROW4, ROW5, ROW6, ROW7,
	ROW8, ROW9, ROW10, ROW11};


//Function Declarations
void GPFSel(unsigned int reg, unsigned char pin, unsigned char function);
void spi_init ( void );
void spi_write(unsigned char value);
void spi_write16(short int value);
void FBWrite(unsigned short int x, unsigned short int y, unsigned char value);
void FBRender(void);
void FBWriteChar(char c);
void FBCharAdvance(void);
void GFXRender(void);
void GFXColor(unsigned char c);
void ControllerCommand(unsigned char command, unsigned char numParams, ...);
void ControllerInit(void);
void ControllerRender(void);
void ColInit(void);
void RowInit(void);
unsigned char readInput(unsigned char col);
void InputHandler(unsigned char input);

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra = 0;
    //unsigned int rb = 0;
    unsigned int rc = 0, rd = 0;
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
     * 25: TFT_DC	 to 4 : DC (just general power dude, lmao)
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
    GFXColor(0x00);
    ColInit();
    RowInit();
    

    while(1)
    {
		//Input
		rd = rc;
		for(ra=0; ra<4; ra++){
			//COL Enable
			PUT32(GPSET0, cols[ra]);
			//for(rb=0;rb<0x100000;rb++) dummy(rb);
			rc = readInput(ra);
			PUT32(GPCLR0, cols[ra]);
			//for(rb=0;rb<0x100000;rb++) dummy(rb);
			if((rc != 0) && (rc != rd)){
				//GFXColor((unsigned char)(rc & 0xFF));
				InputHandler(rc);
				break;
			}
		}
        FBRender();
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
//
// Copyright (c) 2015 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------


//Function Definitions:

//General Graphics Functions:
void FBWrite(unsigned short int x, unsigned short int y, unsigned char value)
{
	frame[x][y] = value;
	return;
}

void FBRender(void)
{
	ControllerRender();
	return;
}

void FBCharAdvance(void)
{
	charX++;
	if(charX > blockWidth){
		charX = 0;
		charY++;
		if(charY > blockHeight-1){
			charY = 0;
		}
	}
	return;
}

void FBWriteChar(char c)
{
	unsigned int ra, rb;
	//Itterate over the 8x10 box with CharPosition denoting the top left corner
	for(ra=0; ra < FONT_FRAME_WIDTH; ra++){
		for(rb=0; rb < FONT_FRAME_HEIGHT; rb++){
			FBWrite((FONT_FRAME_WIDTH * charX) + ra, (FONT_FRAME_HEIGHT * charY) + rb, font_data[(c - 32)%65][rb][ra]);
		}
	}
	FBCharAdvance();
	if(charX == 0 && charY == 0){
		GFXColor(0x00);
	}
	return;
}

void GFXRender(void)
{
	//TODO
	return;
}

void GFXColor(unsigned char c)
{
	for(int i = 0; i < maxHeight; i++)
	{
		for(int j = 0; j < maxWidth; j++)
		{
			FBWrite(i, j, c);
		}
	}
	FBRender();
}

//Raspberry Pi specific Functions:
//Adapted from Welch's version. I replaced the function selection with calls to our function select function.
void spi_init(void)
{
    unsigned int ra;
    ra=GET32(AUX_ENABLES);
    ra|=2; //enable spi0
    PUT32(AUX_ENABLES,ra);

    GPFSel(GPFSEL0, 9, 4);
    GPFSel(GPFSEL0, 8, 4);
    GPFSel(GPFSEL1, 0, 4);
    GPFSel(GPFSEL1, 1, 4);

    PUT32(AUX_SPI0_CS,0x0000030);
	//BCM2835_SPI_CLOCK_DIVIDER_512 = 512,     ///< 512 = 2.048us = 488.28125kHz
	//BCM2835_SPI_CLOCK_DIVIDER_256 = 256,     ///< 256 = 1.024us = 976.5625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_128 = 128,     ///< 128 = 512ns = = 1.953125MHz
	//BCM2835_SPI_CLOCK_DIVIDER_64 = 64,      ///< 64 = 256ns = 3.90625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_32 = 32,      ///< 32 = 128ns = 7.8125MHz
	//BCM2835_SPI_CLOCK_DIVIDER_16 = 16,      ///< 16 = 64ns = 15.625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_8 = 8,       ///< 8 = 32ns = 31.25MHz
	//BCM2835_SPI_CLOCK_DIVIDER_4 = 4, ///< 4 = 16ns = 62.5MHz
    PUT32(AUX_SPI0_CLK, 20); //slowest possible, could probably go faster here
}

void GPFSel(unsigned int reg, unsigned char pin, unsigned char function)
{
	unsigned int curr = GET32(reg);
	//zero out the pin
	curr = curr & ~(7 << 3*pin);
	//set pin to function
	curr = curr | (function << (3*pin));
	//write out our new value
	PUT32(reg, curr);
}

//write a single 16 bit value to the SPI buffer
//Inspired by code written by Welch. Mainly in the way we send only one byte at a time of the value
//Though since the FIFO register is 32 bits we should be able to write to it all at once???
//More investigation is required.
void spi_write(unsigned char value)
{
	//unsigned int ra;

    //for(ra=0;ra<0x10000;ra++) dummy(ra);
    //PUT32(GPCLR0, ((1<<21)));
	//First we set clear our FIFO and enable the TA Bit:
	PUT32(AUX_SPI0_CS, ((1 << 5) | (1 << 4)));
	PUT32(AUX_SPI0_CS, (((1 << 7))));

	//Then we write each byte of our 16 bits to the SPI_FIFO, starting with our High byte
	//Check bit 18 to see if we're ready to send
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<18))
		{
			break;
		}
	}

	//Send the high byte
	PUT32(AUX_SPI0_FIFO, (value >> 0) & 0xFF);
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			break;
		}
	}
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<18))
		{
			break;
		}
	}

	PUT32(AUX_SPI0_CS, 0x00000000);

	return;
}

void spi_write16(short int value)
{
	//unsigned int ra;

    //for(ra=0;ra<0x10000;ra++) dummy(ra);
    //PUT32(GPCLR0, ((1<<21)));
	//First we set clear our FIFO and enable the TA Bit:
	PUT32(AUX_SPI0_CS, ((1 << 5) | (1 << 4)));
	PUT32(AUX_SPI0_CS, (((1 << 7))));

	//Then we write each byte of our 16 bits to the SPI_FIFO, starting with our High byte
	//Check bit 18 to see if we're ready to send
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<18))
		{
			break;
		}
	}
	//Send the high byte
	PUT32(AUX_SPI0_FIFO, (value >> 8) & 0xFF);
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			break;
		}
	}
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<18))
		{
			break;
		}
	}

	//Send the low byte
	PUT32(AUX_SPI0_FIFO, (value >> 0) & 0xFF);
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			break;
		}
	}
	PUT32(AUX_SPI0_CS, 0x00000000);

	return;
}


void ColInit(void){
	//Set all COLS as Output
	GPFSel(GPFSEL0, 2, 1);
	GPFSel(GPFSEL0, 3, 1);
	GPFSel(GPFSEL0, 4, 1);
	GPFSel(GPFSEL0, 5, 1);
	return;
}

void RowInit(void){
	//Set all rows as Input
	GPFSel(GPFSEL1, 4, 0);
	GPFSel(GPFSEL1, 5, 0);
	//GPFSel(GPFSEL1, 8, 0);
	GPFSel(GPFSEL2, 4, 0);
	
	GPFSel(GPFSEL0, 6, 0);
	GPFSel(GPFSEL1, 2, 0);
	GPFSel(GPFSEL1, 3, 0);
	GPFSel(GPFSEL1, 6, 0);
	
	GPFSel(GPFSEL1, 9, 0);
	GPFSel(GPFSEL2, 0, 0);
	GPFSel(GPFSEL2, 1, 0);
	GPFSel(GPFSEL0, 7, 0);
	
	//Enable High event for all rows
	PUT32(GPHEN0, ROW0 | ROW1 | ROW3 |
		ROW4 | ROW5 | ROW6 | ROW7 |
		ROW8 | ROW9 | ROW10 | ROW11);
	return;
}

//Converts Events into button codes
//0x00 = NoIn
//Codes 1 through 48 are actual buttons
//Lower buttons take priority
unsigned char readInput(unsigned char col){
	unsigned int ra, rb, rc = 0;
	ra = GET32(GPLEV0);
	for(rb = 0; rb < 12; rb++){
		if((rows[rb] & ra) != 0){
			rc = 1;
			break;
		}
	}
	if(rc == 1){
		//Clear all events
		//PUT32(GPEDS0, 0xFFFFFFFF);
		return (1 + rb + (12 * (col)));
	}
	else{
		return 0x00;
	}
}

void InputHandler(unsigned char input){
	FBWriteChar(keyCodes[input]);
	return;
}

//ILI9340 specific functions:

/*
 * So we need some new functions to interface with the display.
 * The display consists of an ILI9340 controller and the LCD panel.
 * I have to interface with the panel via the controller, as it seems.
 * The controller will have to take in a serial input
 * and produce a serial output.
 *
 * Right now it looks like I'm using a 4-line Serial In, with Clock, MISO, and MOSI
 * being part of the 3 standard lines. And GPIO 25 being used for the command bit.
 *
 * We will need functions to initialize and send commands, as well as writing to the GRAM.
 * Maybe we have to tell it to render its frame buffer? I'm not sure.
 * The MCU Frame Buffer is 240 wide by 320 tall. [col][row]
 * Color options are available, but we want to jsut stick to black and white for now.
 *
 * The only other person who has code for bare metal piTFT interaction has initialized the following
 * He sent the commands and their associated data.
 * Power 1 0x23
 * Power 2 0x10
 * VCOM 1 0x3E 0x28
 * VCOM 2 0x86
 * COLMOD: Pixel Format Set 0x55
 * Frame rate control 0x00 0x18
 * Display function control 0x08 0x82, 0x27
 * Gamma Function Disable 0x00
 * Gamma curve select 0x01
 * Positive Gamma Correction 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00
 * Negative Gamma correction 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F
 * Sleep Out
 * Display ON
 *
 * Additionally, these were preceded by calls to these commands:
 * 0xEF 0x03, 0x80, 0x02
 * 0xCF 0x00 , 0XC1 , 0X30
 * 0xED 0x64 , 0x03 , 0X12 , 0X81
 * 0xE8 0x85 , 0x00 , 0x78
 * 0xCB 0x39 , 0x2C , 0x00 , 0x34 , 0x02
 * 0xF7 0x20
 * 0xEA  0x00 , 0x00
 *
 * These commands translate to:
 * EF: Undef'd
 * CF: Undef'd
 * ED: Undef'd
 * E8: Undef'd
 * CB: Undef'd
 * F7: Undef'd
 * EA: Undef'd
 * */

void ControllerInit(void){
	//TODO
	//Send the initialization commands
	//Anything else??
	//The guy Inits SPI
	//Then he just does commands
	

	unsigned int ra;
	
	PUT32(GPCLR0, (1 << 7));
	for(ra=0;ra<0x100000;ra++) dummy(ra);
	PUT32(GPSET0, (1 << 7));

	ControllerCommand(ILI9341_CMD_POWER_ON_SEQ_CONTROL, 5, 
		ILI9341_CMD_IDLE_MODE_ON, ILI9341_CMD_MEMORY_WRITE, 
		ILI9341_CMD_NOP, 
		ILI9341_CMD_TEARING_EFFECT_LINE_OFF, 
		0x02);

	ControllerCommand(ILI9341_CMD_POWER_CONTROL_B, 3, 
		ILI9341_CMD_NOP, 
		ILI9341_CMD_POWER_CONTROL_2, 
		ILI9341_CMD_PARTIAL_AREA);

	ControllerCommand(ILI9341_CMD_DRIVER_TIMING_CONTROL_A, 3, 
		0x85, 
		ILI9341_CMD_NOP, 
		0x78);

	ControllerCommand(ILI9341_CMD_DRIVER_TIMING_CONTROL_B, 2, 
		ILI9341_CMD_NOP, 
		ILI9341_CMD_NOP);

	ControllerCommand(0xED, 4, 
		0x64, 
		0x03, 
		ILI9341_CMD_PARTIAL_MODE_ON, 
		0X81);
	
	ControllerCommand(ILI9341_CMD_PUMP_RATIO_CONTROL, 1, 
		ILI9341_CMD_DISP_INVERSION_OFF);

	ControllerCommand(ILI9341_CMD_POWER_CONTROL_1, 1, 
		0x23);

	ControllerCommand(ILI9341_CMD_POWER_CONTROL_2, 1, 
		ILI9341_CMD_ENTER_SLEEP_MODE);

	ControllerCommand(ILI9341_CMD_VCOM_CONTROL_1, 2, 
		ILI9341_CMD_READ_MEMORY_CONTINUE, 
		ILI9341_CMD_DISPLAY_OFF);

	ControllerCommand(ILI9341_CMD_VCOM_CONTROL_2, 1, 
		0x86);

	ControllerCommand(ILI9341_CMD_MEMORY_ACCESS_CONTROL, 1, 
		0x48);

	ControllerCommand(ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET, 1, 
		ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS);

	ControllerCommand(ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL, 2, 
		ILI9341_CMD_NOP, 
		0x18);

	ControllerCommand(ILI9341_CMD_DISPLAY_FUNCTION_CONTROL, 3, 
		0x08, //0x0000 1000
		0x80, //0x1000 0010
		0x27);//0x0010 0111

	ControllerCommand(ILI9341_CMD_ENABLE_3_GAMMA_CONTROL, 1, 
		ILI9341_CMD_NOP);

	ControllerCommand(0x26, 1, 
		ILI9341_CMD_SOFTWARE_RESET);

	ControllerCommand(ILI9341_CMD_POSITIVE_GAMMA_CORRECTION, 15, 
		0x0F, 
		0x31, 
		ILI9341_CMD_PAGE_ADDRESS_SET, 
		ILI9341_CMD_READ_DISP_PIXEL_FORMAT, 
		ILI9341_CMD_READ_DISP_SIGNAL_MODE, 
		0x08, 
		0x4E, 
		0xF1, 
		ILI9341_CMD_VERT_SCROLL_START_ADDRESS, 
		0x07, 
		ILI9341_CMD_ENTER_SLEEP_MODE, 
		0x03, 
		ILI9341_CMD_READ_DISP_SIGNAL_MODE, 
		ILI9341_CMD_READ_DISP_STATUS, 
		ILI9341_CMD_NOP);

	ControllerCommand(ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION, 15, 
		ILI9341_CMD_NOP, 
		ILI9341_CMD_READ_DISP_SIGNAL_MODE, 
		0x14, 
		0x03, 
		ILI9341_CMD_SLEEP_OUT, 
		0x07, 
		0x31, 
		ILI9341_CMD_POWER_CONTROL_2, 
		0x48, 
		0x08, 
		0x0F, 
		ILI9341_CMD_READ_DISP_PIXEL_FORMAT, 
		0x31, 
		ILI9341_CMD_MEMORY_ACCESS_CONTROL, 
		ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC);

	ControllerCommand(ILI9341_CMD_SLEEP_OUT, 0);
	for(ra=0;ra<0x120000;ra++) dummy(ra);

	ControllerCommand(ILI9341_CMD_DISPLAY_ON, 0);
	return;
}


void ControllerCommand(unsigned char command, unsigned char numParams, ...){
	char buffer[50];
	va_list args;

	PUT32(GPCLR0, ((1<<25)));

	spi_write(command);

	PUT32(GPSET0, ((1<<25)));

	if(numParams){
		va_start(args, numParams);
		for(unsigned char i = 0; i < numParams; i++){
			buffer[i] = (unsigned short)va_arg(args, int);
		}
		va_end(args);
		for(unsigned char i = 0; i < numParams; i++){
			spi_write(buffer[i]);
		}
	}

	return;
}

void ControllerRender(void){
	//TODO
	//Here we start a MemWrite command
	//And then pass our frame buffer byte by byte
	//But we make sure that each byte is scaled to the right size
	//Since we are opperating at such a small size, we have to write each byte multiple times.
	//Should be simple when I calculate the scale factor (hopefully it's an integer :|)
	
	//Set viewport Height
	ControllerCommand(0x2B, 4,
		0x00,
		0x00,
		(maxHeight >> 8) & 0xFF, //High byte
		(maxHeight) & 0xFF  //Low Byte
		);
	//Set viewport Width
	ControllerCommand(0x2A, 4,
		0x00,
		0x00,
		(maxWidth >> 8) & 0xFF, //High Byte
		(maxWidth) & 0xFF  //Low Byte
		);
	
	PUT32(GPCLR0, ((1<<25)));
	spi_write(0x2C);
	PUT32(GPSET0, ((1 << 25)));
	
	for(int i = 0; i < maxHeight; i++){
		for(int j = maxWidth; j > 0; j--){
			spi_write16((frame[i][j - 1] & 0xFF) | (frame[i][j - 1] << 8));
		}
	}
	return;
}
