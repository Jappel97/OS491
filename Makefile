
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

OBJECTS = vectors.o notmain.o Font.o  pi.o controller.o frame.o memory.o Programs.o timer.o interrupts.o string.o math.o

all : kernel.img

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f Logs/*.srec
	rm -f Logs/*.list

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

Font.o : Font.c
	$(ARMGNU)-gcc $(COPS) -c Font.c -o Font.o
	
pi.o : pi.c
	$(ARMGNU)-gcc $(COPS) -c pi.c -o pi.o
	
controller.o : controller.c
	$(ARMGNU)-gcc $(COPS) -c controller.c -o controller.o
	
frame.o : frame.c
	$(ARMGNU)-gcc $(COPS) -c frame.c -o frame.o
	
memory.o : memory.c
	$(ARMGNU)-gcc $(COPS) -c memory.c -o memory.o

Programs.o : Programs.c
	$(ARMGNU)-gcc $(COPS) -c Programs.c -o Programs.o
	
timer.o : timer.c
	$(ARMGNU)-gcc $(COPS) -c timer.c -o timer.o
	
interrupts.o : interrupts.c
	$(ARMGNU)-gcc $(COPS) -c interrupts.c -o interrupts.o
	
string.o : string.c
	$(ARMGNU)-gcc $(COPS) -c string.c -o string.o
	
math.o : math.c
	$(ARMGNU)-gcc $(COPS) -c math.c -o math.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain.o


notmain.elf : memmap $(OBJECTS)
	$(ARMGNU)-ld $(OBJECTS) -T memmap -o notmain.elf -L/home/josh/opt/Cross2/gcc-arm-none-eabi-9-2019-q4-major/lib/gcc/arm-none-eabi/9.2.1 -lgcc
	$(ARMGNU)-objdump -D notmain.elf > Logs/notmain.list

kernel.img : notmain.elf
	$(ARMGNU)-objcopy --srec-forceS3 notmain.elf -O srec Logs/notmain.srec
	$(ARMGNU)-objcopy notmain.elf -O binary kernel.img

