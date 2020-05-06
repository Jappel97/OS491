//Global variables def'd
	//frame buffer: 96 wide by 64 tall
	//Accessed as [y][x]
#define maxHeight 320
#define maxWidth 240
#define blockHeight 24 //10 pixel tall
#define blockWidth 40 //8 pixel wide
//The frame buffer can be read by anyone.
unsigned char frame[maxHeight][maxWidth];
unsigned char charX, charY;

void FBWrite(unsigned short int x, unsigned short int y, unsigned char value);
void FBRender(void);
void FBWriteChar(char c);
void FBWriteString(char* str, int len);
void FBCharNewline(void);
void FBCharReset(void);
void FBCharAdvance(void);
void GFXRender(void);
void GFXColor(unsigned char c);

void FBWriteNum(int N, int base);
char countDigits(int N, int base);
char getLastDigit(int N, int base);
char countDecDigits(double N);
void FBWriteDec(double dec);
