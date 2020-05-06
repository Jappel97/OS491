#include <stdarg.h>
#include "pi.h"
#include "base.h"
#include "frame.h"
#include "controller.h"


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

