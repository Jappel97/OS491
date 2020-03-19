
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#include <stdarg.h>

#define GPFSEL0 0x20200000 //pins 0-9
#define GPFSEL1 0x20200004 //pins 10-19
#define GPFSEL2 0x20200008 //pins 20-29
#define GPFSEL3 0x2020000C //pins 30-29
#define GPFSEL4 0x20200010 //pins 40-49
#define GPSET0  0x2020001C //SET pins 0-31
#define GPSET1  0x20200020 //SET pins 32-53
#define GPCLR0  0x20200028 //CLEAR pins 0-31
#define GPCLR1  0x2020002C //CLEAR pins 32-53

#define AUX_ENABLES     0x20215004
#define AUX_SPI0_CS     0x20204000
#define AUX_SPI0_FIFO   0x20204004
#define AUX_SPI0_CLK    0x20204008
#define AUX_SPI0_DLEN   0x2020400C
#define AUX_SPI0_LTOH   0x20204010
#define AUX_SPI0_DC     0x20204014

//Global variables def'd
	//frame buffer: 96 wide by 64 tall
	//Accessed as [y][x]
unsigned short int frame[64][120];
unsigned char charX, charY;

//Function Declarations
void GPFSel(unsigned int reg, unsigned char pin, unsigned char function);
void spi_init ( void );
void spi_write(unsigned char value);
void spi_write16(short int value);
void FBWrite(unsigned char x, unsigned char y, unsigned short int value);
void FBRender(void);
void FBWriteChar(char c);
void FBCharAdvance(void);
void GFXRender(void);
void GFXColor(unsigned char c);
void ControllerCommand(unsigned char command, unsigned char numParams, ...);
void ControllerInit(void);
void ControllerRender(void);
void testRender(void);

void csPullTest(void){
	unsigned int ra;
	PUT32(AUX_SPI0_CS, (((1 << 7))));
	for(ra=0;ra<0x10000;ra++) dummy(ra);
	PUT32(AUX_SPI0_CS, 0x00000000);
}
//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
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
    //pins 8, 7
    GPFSel(GPFSEL0, 7, 1); GPFSel(GPFSEL0, 8, 1);
    //pins 14, 15, 18, 12, 16,
    GPFSel(GPFSEL1, 2, 1); GPFSel(GPFSEL1, 4, 1); GPFSel(GPFSEL1, 5, 1); GPFSel(GPFSEL1, 6, 1); GPFSel(GPFSEL1, 8, 1);
    //pins 23, 24, 25, 20, 21
    GPFSel(GPFSEL2, 0, 1); GPFSel(GPFSEL2, 1, 1); GPFSel(GPFSEL2, 3, 1); GPFSel(GPFSEL2, 4, 1); GPFSel(GPFSEL2, 5, 1);
    //pin 47
    GPFSel(GPFSEL4, 7, 1);
    
    spi_init();
    
    PUT32(AUX_SPI0_CS,0x00000000); //cs1 high
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    PUT32(AUX_SPI0_CS,0x00400000); //cs1 low, reset
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    PUT32(AUX_SPI0_CS,0x00000000); //cs1 comes back up
    //Pull 25 high
    //PUT32(GPSET0, (1 << 25));
    
    //ControllerInit();
    
    
    testRender();
    
    while(1)
    {
		//SET Pin 47 ON
		//csPullTest();
        PUT32(GPSET1,1<<(47-32));
        //GFXColor(0xFF);
        //SET pins 7, 8, 12, 14, 15, 16, 18, 20, 21, 23, 24, 25
        //PUT32(GPSET0, ((1<<25)));
        //WAIT for some time
        for(ra=0;ra<0x100000;ra++) dummy(ra);
        //CLEAR Pin 47
        PUT32(GPCLR1,1<<(47-32));
        //GFXColor(0x00);
        //CLEAR pins 7, 8, 12, 14, 15, 16, 18, 20, 21, 23, 24, 25
        //PUT32(GPCLR0, ((1<<25)));
        //WAIT for some time
        for(ra=0;ra<0x100000;ra++) dummy(ra);
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

//Adapted from Welch's version. I replaced the function selection with calls to our function select function.
void spi_init(void)
{
    unsigned int ra;
    PUT32(GPSET0, ((1<<21)));
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    //PUT32(GPCLR0, ((1<<21)));

    ra=GET32(AUX_ENABLES);
    ra|=2; //enable spi0
    PUT32(AUX_ENABLES,ra);

    GPFSel(GPFSEL0, 9, 4);
    GPFSel(GPFSEL0, 8, 4);
    GPFSel(GPFSEL0, 7, 4);
    GPFSel(GPFSEL1, 0, 4);
    GPFSel(GPFSEL1, 1, 4);

    PUT32(AUX_SPI0_CS,0x0000030);
    PUT32(AUX_SPI0_CLK,0x0000); //slowest possible, could probably go faster here
    PUT32(GPCLR0, ((1<<21)));
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
	PUT32(GPSET0, ((1<<21)));
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			PUT32(GPCLR0, ((1<<21)));
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
	PUT32(GPSET0, ((1<<21)));
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			PUT32(GPCLR0, ((1<<21)));
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
	PUT32(GPSET0, ((1<<21)));
	//Check bit 16 to see if we're done sending
	while(1)
	{
		if(GET32(AUX_SPI0_CS)&(1<<16))
		{
			PUT32(GPCLR0, ((1<<21)));
			break;
		}
	}
	PUT32(AUX_SPI0_CS, 0x00000000);
	
	return;
}

void FBWrite(unsigned char x, unsigned char y, unsigned short int value)
{
	frame[y][x] = value;
	return;
}

void FBRender(void)
{
	ControllerRender();
	return;
}

void FBCharAdvance(void)
{
	if(charX + 4 > 96)
	{
		charX = 0;
		if(charY + 8 > 64)
		{
			//Clear the screen
			//TODO
			
			charY = 0;
		}
		else
		{
			charY += 8;
		}
	}
	else
	{
		charX += 4;
	}
	return;
}

void FBWriteChar(char c)
{
	//TODO
	FBCharAdvance();
	return;
}

void GFXRender(void)
{
	//TODO
	return;
}

void GFXColor(unsigned char c)
{
	for(int i = 0; i < 98; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			FBWrite(i, j, (c | (c << 8)));
			FBRender();
		}
	}
}

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
	spi_init();
	//Then he just does commands
	
	unsigned int ra;
	
	//some voodoo black magic commands. Not in the docs. No clue what they do
	ControllerCommand(0xEF, 3, 0x03, 0x80, 0x02);
	ControllerCommand(0xCF, 3, 0x00, 0xC1, 0x30);
	ControllerCommand(0xED, 4, 0x64, 0x03, 0x12, 0x81);
	ControllerCommand(0xE8, 3, 0x85, 0x00, 0x78);
	ControllerCommand(0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02);
	ControllerCommand(0xF7, 1, 0x20);
	ControllerCommand(0xEA, 2, 0x00, 0x00);
	//So yeah, I think we're fine.
	//He's got those black magic voodoo commands tho
	//No clue what those do
	
	//Power 1 0x23
	ControllerCommand(0xC0, 1, 0x23);
	//Power 2 0x10
	ControllerCommand(0xC1, 1, 0x10);
	//VCOM 1 0x3E 0x28
	ControllerCommand(0xC5, 2, 0x3E, 0x28);
	//VCOM 2 0x86
	ControllerCommand(0xC7, 1, 0x86);
	//COLMOD: Pixel Format Set 0x55
	ControllerCommand(0x3a, 1, 0x55);
	//Frame rate control 0x00 0x18
	ControllerCommand(0xB1, 2, 0x00, 0x18);
	//Display function control 0x08 0x82, 0x27
	ControllerCommand(0xB6, 3, 0x08, 0x82, 0x27);
	//Gamma Function Disable 0x00
	ControllerCommand(0xF2, 1, 0x00);
	//Gamma curve select 0x01
	ControllerCommand(0x26, 1, 0x01);
	//Positive Gamma Correction 
	ControllerCommand(0xE0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00);
	//Negative Gamma correction 
	ControllerCommand(0xE1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F);
	//Sleep Out
	ControllerCommand(0x11, 0);
	
	for(ra=0;ra<0x100000;ra++) dummy(ra);
	
	//Display ON
	ControllerCommand(0x29, 0);
	return;
}
 
void ControllerCommand(unsigned char command, unsigned char numParams, ...){
	//TODO
	short buffer[50];
	va_list args;
	//Turn on pin 25
	PUT32(GPSET0, ((1<<25)));
	//send command and params
	spi_write(command);
	//Disable CX in between in each send
	//turn pin 25 off
	PUT32(GPCLR0, ((1<<25)));
	if (numParams) {
		va_start(args, numParams);
		for (unsigned char i = 0; i < numParams; i++) {
			buffer[i] = (unsigned short)va_arg(args, int);
		}
		va_end(args);
	}
	for(int i = 0; i < numParams; i++)
	{
		spi_write16(buffer[i]);
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
	PUT32(GPCLR0, ((1<<25)));
	spi_write(0x2C);
	PUT32(GPSET0, ((1 << 25)));
	
	//itterate over height
	for(int i = 0; i < 64; i++){
		for(int h = 0; h < 5; h++){
			//and then per row
			for(int j = 0; j < 120; j++){
				for(int w = 0; w < 2; w++){
					spi_write16(frame[i][j]);
				}
			}
		}
	}
	return;
}

void testRender(void){
	/*
	 *   lcd_write_cmd(LCD_CMD_SET_COLUMN_ADDR);
  lcd_write_data_u16(1);
  lcd_write_data_u16(6);

  lcd_write_cmd(LCD_CMD_SET_PAGE_ADDR);
  lcd_write_data_u16(1);
  lcd_write_data_u16(6);
  * */
  
  /*
   * lcd_write_cmd(LCD_CMD_MEMORY_WRITE);
lcd_write_data_u16(0x0000); // Pixel 00 BLACK
lcd_write_data_u16(0x0000); // Pixel 01 BLACK
lcd_write_data_u16(0x0000); // Pixel 02 BLACK
lcd_write_data_u16(0x0000); // Pixel 03 BLACK
lcd_write_data_u16(0x0000); // Pixel 04 BLACK
lcd_write_data_u16(0xF800); // Pixel 05 RED
lcd_write_data_u16(0x0000); // Pixel 06 BLACK
lcd_write_data_u16(0x0000); // Pixel 07 BLACK
lcd_write_data_u16(0x0000); // Pixel 08 BLACK
lcd_write_data_u16(0x0000); // Pixel 09 BLACK
lcd_write_data_u16(0xF800); // Pixel 10 RED
lcd_write_data_u16(0xF800); // Pixel 11 RED
lcd_write_data_u16(0x0000); // Pixel 12 BLACK
lcd_write_data_u16(0x0000); // Pixel 13 BLACK
lcd_write_data_u16(0x0000); // Pixel 14 BLACK
lcd_write_data_u16(0xF800); // Pixel 15 RED
lcd_write_data_u16(0xF800); // Pixel 16 RED
lcd_write_data_u16(0xF800); // Pixel 17 RED
lcd_write_data_u16(0x0000); // Pixel 18 BLACK
lcd_write_data_u16(0x0000); // Pixel 19 BLACK
lcd_write_data_u16(0xF800); // Pixel 20 RED
lcd_write_data_u16(0xF800); // Pixel 21 RED
lcd_write_data_u16(0xF800); // Pixel 22 RED
lcd_write_data_u16(0xF800); // Pixel 23 RED

	*/
	
	ControllerCommand(0X2a, 2, 0X01, 0X06);
	ControllerCommand(0x2B, 2, 0x01, 0x06);
	PUT32(GPCLR0, ((1<<25)));
	//send command and params
	spi_write(0x02C);
	//Disable CX in between in each send
	//turn pin 25 off
	PUT32(GPSET0, ((1<<25)));
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0xF800);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0x0000);
	spi_write16(0x0000);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0x0000);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	spi_write16(0xF800);
	unsigned int ra;

	
	ControllerCommand(0x11, 0);
	
	for(ra=0;ra<0x100000;ra++) dummy(ra);
	
	//Display ON
	ControllerCommand(0x29, 0);
}
