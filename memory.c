#define MAXADDRESS 	0x1FFFFFFF //End of physical user address space
#define USERSTART 	0x00FF89C0 //Start of physical user address space
#define DOCSTART 	0x1FF5CFFF //Start of "Docspace" where programs can store data to be used by other programs. In lieu of an actual filesystem.
#define MMUTABLEBASE 0x00080000
#include "base.h"
#include "frame.h"
#include "memory.h"

//The place where we can denote which bytes of memory are taken. 1 bit = 4 bytes.
unsigned char memArray[16220608];

void memInit(void){
	for(int i = 0; i < 16220608; i++){
		memArray[i] = 0x00;
	}
}


unsigned int mmu_section ( unsigned int add, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=add>>20;
    rb=MMUTABLEBASE|(ra<<2);
    rc=(ra<<20)|flags|2;
    PUT32(rb,rc);
    return(0);
}

unsigned int cacheInit(void){
	
    //ram used by the stack and the program
    //Every megabyte in our lower 512 should be cachable now.
    //Can change as needed.
    for(int i = 0x00000000; i < 0x20000000; i += 0x100000)
		if(mmu_section(i + 0x00000000,0x0000|8|4)) return(1);
    //Memory mapped I/O used by the uart, etc, not cached
    if(mmu_section(0x20000000,0x0000)) return(1);
    if(mmu_section(0x20100000,0x0000)) return(1);
    if(mmu_section(0x20200000,0x0000)) return(1);
    
    start_l1cache();
    return 0;
}

/*
 * Memory Manager
 * */
 
 
char checkAtPos(int memIndex, char bitPos){
	char mask = 0x1 << bitPos;
	return (mask == (memArray[memIndex] & mask)) ? 1 : 0;
}

void setHigh(int memIndex, char bitpos){
	char mask = 0x1 << bitpos;
	memArray[memIndex] = memArray[memIndex] | mask;
	return;
}

void setLow(int memIndex, char bitpos){
	char mask = 0x1 << bitpos;
	memArray[memIndex] = memArray[memIndex] & (~mask);
	return;
}

void setMem(int memIndex, char bitPos, int numBytes){
	for(char i = numBytes; i > 0; i--){
		setHigh(memIndex, bitPos);
		if(bitPos == 7){
			bitPos = 0;
			memIndex++;
		}
		else{
			bitPos++;
		}
	}
	return;
}

void clearMem(int memIndex, char bitPos, int numBytes){
	for(int i = numBytes; i > 0; i--){
		setLow(memIndex, bitPos);
		if(bitPos == 7){
			bitPos = 0;
			memIndex++;
		}
		else{
			bitPos++;
		}
	}
}

//this should return the real address that our chosen bit refers to
//SO bit one of long one should just be the base address
//And bit two of long one should be the base address plus one
int encodeAddress(int memIndex, char bitPos){
	return USERSTART + (4 * memIndex * 8) + bitPos*4;
}


void* malloc(int numBytes)
{
	int bucketsToFind = 1 + (numBytes/4);
	//space for the mem requested
	int bucketsAvailable = 0;
	char bitPos = 0;
	//16220608 is how many buckets we need. Since every 4 bytes of mem is 
	//one bit, and each bucket is 8 bits. (512*1024*1024) bytes over 4 over 8.
	for(int i = 0; i < 16220608; i++){
		//check if the current bucket is full. If so, move on.
		if(memArray[i] == 255){
			bucketsAvailable = 0;
			continue;
		}
		//If it isn't, we can check if there's room in it.
		else{
			if(bucketsAvailable < bucketsToFind){
				for(char j = 0; j < 8; j++){
					if(checkAtPos(i, j) == 1){
						bucketsAvailable = 0;
					}
					else{
						if(bucketsAvailable == 0){
							bitPos = j;
						}
						bucketsAvailable++;
					}
				}
			}
			else if(bucketsAvailable >= bucketsToFind){
				setMem(i, bitPos, bucketsToFind);
				return (unsigned char*)encodeAddress(i, bitPos);
			}
		}
	}
	//If we get here, then memory is full. Sucks to suck, lol
	return (void*)0x0;
}

void free(void* ptr, int numBytes){
	unsigned int ra = (numBytes/4);
	unsigned int rb = (numBytes % 4) == 0 ? ra : ra + 1;
	clearMem(rb / 8, rb % 8, numBytes);
	//Somehow we need to decode this into the correct memIndex and bitPos...
	//bitPos can be only as large as 7. So that means the most bitPos * 4 can be is 28
	//If we itterate through the various bitPos's, then rb - (bitPos*4) should only reveal our proper memIndex when we get the right bitPos.
	return;
}
