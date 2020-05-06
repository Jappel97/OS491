#define GPFSEL0 	0x20200000 //pins 0-9
#define GPFSEL1 	0x20200004 //pins 10-19
#define GPFSEL2 	0x20200008 //pins 20-29
#define GPFSEL3 	0x2020000C //pins 30-29
#define GPFSEL4 	0x20200010 //pins 40-49
#define GPSET0  	0x2020001C //SET pins 0-31
#define GPSET1  	0x20200020 //SET pins 32-53
#define GPCLR0  	0x20200028 //CLEAR pins 0-31
#define GPCLR1  	0x2020002C //CLEAR pins 32-53
#define GPLEV0		0x20200034 //Read state of pins 0-31
#define GPLEV1		0x20200038 //Read state of pins 32-53
#define GPEDS0		0x20200040 //Detect Event for pins 0-31
#define GPEDS1		0x20200044 //Detect Event for pins 32-53
#define GPHEN0		0x20200064 //Enable High Event for pins 0-31
#define GPHEN1		0x20200068 //Enable High Event for pins 32-53
#define GPPUD		0x20200094
#define GPPUDCLK0	0x20200098
#define GPPUDCLK1	0x202000b0

#define AUX_ENABLES     0x20215004
#define AUX_SPI0_CS     0x20204000
#define AUX_SPI0_FIFO   0x20204004
#define AUX_SPI0_CLK    0x20204008
#define AUX_SPI0_DLEN   0x2020400C
#define AUX_SPI0_LTOH   0x20204010
#define AUX_SPI0_DC     0x20204014


#define COL0	(1 << 2)
#define COL1	(1 << 3)
#define COL2	(1 << 4)
#define COL3	(1 << 5)

#define ROW0	(1 << 14) // 1+0, 1+12, 1+24, 1+36
#define ROW1	(1 << 15)
#define ROW2	(1 << 17)
#define ROW3	(1 << 24)

#define ROW4	(1 << 6)
#define ROW5	(1 << 12)
#define ROW6	(1 << 13)
#define ROW7	(1 << 16)

#define ROW8	(1 << 19)
#define ROW9	(1 << 20)
#define ROW10	(1 << 21)
#define ROW11	(1 << 7) //We have to sacrifice CS1 :(

void GPFSel(unsigned int reg, unsigned char pin, unsigned char function);
void spi_init ( void );
void spi_write(unsigned char value);
void spi_write16(short int value);

void ColInit(void);
void RowInit(void);
unsigned char readInput(unsigned char col);
unsigned char getInput(void);
