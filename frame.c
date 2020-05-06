#include "Font.h"
#include "frame.h"
#include "controller.h"

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
	if(charX > blockWidth - 1){
		charX = 1;
		charY++;
		if(charY > blockHeight-1){
			charY = 0;
		}
	}
	return;
}

void FBCharNewline(void){
	charX = 1;
	charY++;
	if(charY > blockHeight-1){
			charY = 0;
			GFXColor(0x00);
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

void FBCharReset(void){
	charX = 0;
	charY = 0;
	return;
}

void FBWriteString(char* str, int len){
	for(int i = 0; i < len; i++){
		if(str[i] != '\0'){
			FBWriteChar(str[i]);
		}
		else{
			return;
		}
	}
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

void FBWriteNum(int N, int base){
	//GFXColor(0x00);
	unsigned int ra;
	if(N == 0){
		FBWriteChar('0');
		FBCharNewline();
		return;
	}
	if(N < 0){
		FBWriteChar('-');
		N *= -1;
	}
	int rb;
	ra = countDigits(N, base);
	char digits[ra];
	char Nums[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	for(rb = 0; rb < ra; rb++){
			digits[rb] = Nums[N%base];
			N /= base;
	}
	for(rb = ra - 1; rb >= 0; rb--){
		FBWriteChar(digits[rb]);
	}
	FBCharNewline();
	return;
}

void FBWriteDec(double dec){
	unsigned int rb;
	if(dec == 0){
		FBWriteChar('0');
		return;
	}
	if(dec < 0){
		FBWriteChar('-');
		dec *= -1;
	}
	char length = countDecDigits(dec);
	char length2 = countDigits((int)dec, 10);
	char digits[length+1];
	double runningTotal = dec;
	char Nums[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	for(rb = 0; rb < length2; rb++){
		digits[rb + (length - length2) + 1] = Nums[(int)runningTotal%10];
		runningTotal /= 10;
	}
	if((dec - (int) dec) > 0.0000000001){
		digits[rb + (length - length2) - 1] = -1;
		runningTotal = dec - (int)dec;
		for(rb = 0; rb < (length - length2); rb++){
			runningTotal *= 10;
			digits[(length - length2) - 1 - rb] = Nums[((int)runningTotal%10)];
		}
	}
	for(rb = length+1; rb > 0; rb--){
		if(digits[rb-1] < 48 || digits[rb-1] > 57){
			FBWriteChar('.');
		}
		else{
			FBWriteChar(digits[rb-1]);
		}
	}
}

char countDigits(int N, int base){
	char count = 0;
	while(N > 0){
			count++;
			N/=base;
	}
	return count;
}

char countDecDigits(double N){
	char count = 0;
	int intPart = (int)N;
	double decPart = N - intPart;
	while(intPart > 0){
		count++;
		intPart/=10;
	}
	while(decPart > 0.0000000001 && count < 10){
		count++;
		decPart *= 10;
		decPart -= (int)decPart;
	}
	return count;
}

