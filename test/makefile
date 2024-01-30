MAIN_APP = blink

CC = avr-gcc.exe
C++ = avr-g++.exe
OBJCOPY = avr-objcopy.exe
AVRDUDE := avrdude

MAIN_HEX_PATH = "D:/LocalRepo/workspace/Arduino IDE/Main/test/blink.hex"

DEFINE = -DF_CPU=8000000L
DEFINE += -D__AVR_ATmega328P__

CFLAGS = -g -Os -c -o
MAINCFLAGS = -g -Os -o

LFLAGS = -Os -mmcu=atmega328p -o

HFLAGS = -j .text -j .data -O ihex

DUDEFLAGS = -c
DUDEFLAGS += usbasp
DUDEFLAGS += -p
DUDEFLAGS += m328p
DUDEFLAGS += -b
DUDEFLAGS += 19200
DUDEFLAGS += -U flash:w:$(MAIN_HEX_PATH):i

$(MAIN_APP).o: $(MAIN_APP).cpp
	$(C++) $^ $(DEFINE) $(MAINCFLAGS) $@

$(MAIN_APP).elf: $(MAIN_APP).o
	$(C++) $(MAIN_APP).cpp $(LFLAGS) $@

Build: $(MAIN_APP).elf
	$(OBJCOPY) $(HFLAGS) $< $(MAIN_APP).hex

Burn: Build 
	$(AVRDUDE) $(DUDEFLAGS)