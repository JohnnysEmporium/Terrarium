#Main application file name

MAIN_APP = main

LIB_DHT = ./lib/AVR-DHT/DHT
LIB_HD = ./lib/hd44780/hd44780

#Main hex file path in windows format MAIN_HEX_PATH =
MAIN_HEX_PATH = D\:\\LocalRepo\\workspace\\Arduino IDE\\Main\out\\${MAIN_APP}.hex

# Compiler and other Section 
CC = avr-gcc.exe
C++ = avr-g++.exe
OBJCOPY = avr-objcopy.exe
AVRDUDE := avrdude -C "D\:\\Program Files (x86)\\Arduino\\hardware\\tools\\avr\\etc\\avrdude.conf"

#Defines
DEFINE = -DF_CPU=8000000L
DEFINE += -D__AVR_ATmega328P__

#Options for avr-gcc 
CFLAGS = -g -Os -c -o
MAINCFLAGS = -g -Os -o

#Linking options for avr-gcc 
LFLAGS = -Os -mmcu=atmega328p -o

#Options for HEX file generation 
HFLAGS = -j .text -j .data -O ihex

#Options for avrdude to burn the hex file

#MMCU model here according to avrdude options

DUDEFLAGS = -c
DUDEFLAGS += usbasp
DUDEFLAGS += -p
DUDEFLAGS += m328p
DUDEFLAGS += -P
DUDEFLAGS += COM3
DUDEFLAGS += -b
DUDEFLAGS += 19200
DUDEFLAGS += -U flash\:w\:$(MAIN_HEX_PATH)\:i

# Sources files needed for building the application

SRC = $(MAIN_APP).cpp

#DEPS = hd44780.h

# The headers files needed for building the application

#INCLUDE = -I.
INCLUDE += -I./lib/AVR-DHT
INCLUDE += -I./lib/hd44780_111

#INCLUDE += -L.
#INCLUDE += -L./lib/AVR-DHT-master
#INCLUDE += -L./lib/hd44780_111

#INCLUDE += -lDHT
#INCLUDE += -lhd44780

#INCLUDE += -v


# commands Section 


$(LIB_DHT).o: $(LIB_DHT).c $(LIB_DHT).h $(LIB_DHT)_settings.h
	$(CC) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_DHT).c

$(LIB_HD).o: $(LIB_HD).c $(LIB_HD).h $(LIB_HD)_settings.h
	$(CC) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_HD).c

$(MAIN_APP).o: $(SRC) $(LIB_HD).o $(LIB_DHT).o
	$(C++) $^ $(INCLUDE) $(DEFINE) $(MAINCFLAGS) $@

$(MAIN_APP).elf: $(MAIN_APP).o $(LIB_HD).o $(LIB_DHT).o
	$(CC) $(SRC) $(LIB_DHT).o $(LIB_HD).o $(INCLUDE) $(LFLAGS) $@

Build: $(MAIN_APP).elf
	$(OBJCOPY) $(HFLAGS) $< $(MAIN_APP).hex

Burn: Build 
	$(AVRDUDE) $(DUDEFLAGS)



