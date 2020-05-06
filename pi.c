#include "base.h"
#include "pi.h"


static const unsigned int cols[4] = {COL0, COL1, COL2, COL3};
static const unsigned int rows[12] = {ROW0, ROW1, ROW2, ROW3,
	ROW4, ROW5, ROW6, ROW7,
	ROW8, ROW9, ROW10, ROW11};

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
	GPFSel(GPFSEL1, 7, 0);
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
	PUT32(GPHEN0, ROW0 | ROW1 | ROW2 | ROW3 |
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

unsigned char getInput(void){
	unsigned int ra, rc;
	for(ra=0; ra<4; ra++){
		//COL Enable
		PUT32(GPSET0, cols[ra]);
		rc = readInput(ra);
		PUT32(GPCLR0, cols[ra]);
		if((rc != 0)){
			break;
		}
	}
	return rc;
}
