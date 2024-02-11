#include "avr\sfr_defs.h"

//Positions of items on LCD
#define LCD_SEM_0       0x02
#define LCD_SEM_1       0x05
#define LCD_H_POS       0x00
#define LCD_M_POS       0x03
#define LCD_S_POS       0x06
#define LCD_TEMP_CONST  0x40
#define LCD_RH_CONST    0x48
#define LCD_TEMP        0x43
#define LCD_HUM         0x4C

//Positions of items for printTime
// #define LCD_H           0
// #define LCD_M           1
// #define LCD_S           2

enum ptEnum{
    LCD_H,
    LCD_M,
    LCD_S
};

void LCDInit();
void LCDPutConstantSymbols();
void LCDGoTo(uint8_t pos);
void LCDPuts(const char *s);
void LCDOn();
void LCDOff();
void printTime(int c, uint8_t val);