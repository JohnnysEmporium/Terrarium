SHELL := cmd
#Main application file name
MAIN_APP = main

LIB_DHT = ./lib/AVR-DHT/DHT
LIB_DHT_H = ./lib/DHTHandler/DHTHandler
LIB_HD = ./lib/hd44780/hd44780
LIB_LCD_H = ./lib/LCDHandler/LCDHandler
LIB_TIME_H = ./lib/TimeHandler/TimeHandler
LIB_BUTTON_H = ./lib/ButtonHandler/ButtonHandler

# The headers files needed for building the application
INCLUDE += -I.$(LIB_DHT)
INCLUDE += -I.$(LIB_DHT_H)
INCLUDE += -I.$(LIB_HD)
INCLUDE += -I.$(LIB_LCD_H)
INCLUDE += -I.$(LIB_TIME_H)
INCLUDE += -I.$(LIB_BUTTON_H)

COMPILED_LIBS = $(LIB_DHT).o $(LIB_HD).o $(LIB_DHT_H).o $(LIB_LCD_H).o $(LIB_TIME_H).o $(LIB_BUTTON_H).o

#Main hex file path in windows format MAIN_HEX_PATH =
MAIN_HEX_PATH = "D:/LocalRepo/workspace/Arduino IDE/Main/main.hex"

# Compiler and other Section 
CC = avr-gcc.exe
C++ = avr-g++.exe
OBJCOPY = avr-objcopy.exe
AVRDUDE := avrdude
#Defines
DEFINE = -DF_CPU=8000000L
DEFINE += -D__AVR_ATmega328P__

#Options for avr-gcc 
CFLAGS = -mmcu=atmega328p -g -Os -c -o 
MAINCFLAGS = -mmcu=atmega328p -g -Os -o 

#Linking options for avr-gcc 
LFLAGS = -mmcu=atmega328p -Os -o

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
DUDEFLAGS += -V
DUDEFLAGS += -U flash:w:$(MAIN_HEX_PATH):i

# Sources files needed for building the application

SRC = $(MAIN_APP).cpp

# commands Section 

#DHT Library
$(LIB_DHT).o: $(LIB_DHT).c $(LIB_DHT).h $(LIB_DHT)_settings.h
	$(CC) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_DHT).c

#LCD Library
$(LIB_HD).o: $(LIB_HD).c $(LIB_HD).h $(LIB_HD)_settings.h
	$(CC) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_HD).c

#DHT Handler
$(LIB_DHT_H).o: $(LIB_DHT_H).cpp $(LIB_DHT_H).hpp
	$(C++) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_DHT_H).cpp

#LCD Handler
$(LIB_LCD_H).o: $(LIB_LCD_H).cpp $(LIB_LCD_H).hpp
	$(C++) $(INCLUDE) $(DEFINE) $(CFLAGS) ./$@ $(LIB_LCD_H).cpp

#Time Handler - abandoned -Os because the compiler was causing problems with one of the functions
# see ButtonHandler comment for more.
$(LIB_TIME_H).o: $(LIB_TIME_H).cpp $(LIB_TIME_H).hpp $(LIB_LCD_H).o
	$(C++) $(INCLUDE) $(DEFINE) -mmcu=atmega328p -g -O2 -c -o ./$@ $(LIB_TIME_H).cpp

#Button Library - same as above
$(LIB_BUTTON_H).o: $(LIB_BUTTON_H).cpp $(LIB_BUTTON_H).hpp $(LIB_TIME_H).o $(LIB_LCD_H).o
	$(CC) $(INCLUDE) $(DEFINE) -mmcu=atmega328p -g -O2 -c -o ./$@ $(LIB_BUTTON_H).cpp

#Main
$(MAIN_APP).o: $(SRC) $(COMPILED_LIBS)
	$(C++) $^ $(INCLUDE) $(DEFINE) $(MAINCFLAGS) $@

$(MAIN_APP).elf: $(MAIN_APP).o $(COMPILED_LIBS)
	$(C++) $(SRC) $(COMPILED_LIBS) $(INCLUDE) $(LFLAGS) $@

build: $(MAIN_APP).elf
	$(OBJCOPY) $(HFLAGS) $< $(MAIN_APP).hex

burn: Build 
	$(AVRDUDE) $(DUDEFLAGS)

clean: 
	rm $(COMPILED_LIBS)


