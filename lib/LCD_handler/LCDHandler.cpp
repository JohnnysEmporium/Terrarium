#include "LCDHandler.hpp"
#include "avr\sfr_defs.h"

extern "C" {
  #include "../hd44780/hd44780.h"
}

void LCDInit(){
    lcd_init();
    LCDPutConstantSymbols();
}

void LCDPutConstantSymbols(){
  lcd_clrscr();
  lcd_goto(LCD_SEM_0);
  lcd_puts(":");
  lcd_goto(LCD_SEM_1);
  lcd_puts(":");
  
  lcd_goto(LCD_TEMP_CONST);
  lcd_puts("T: ");
  lcd_goto(LCD_RH_CONST);
  lcd_puts("RH: ");
}

void LCDOff(){
  lcd_command(_BV(LCD_DISPLAYMODE));
}

void LCDOn(){
  lcd_command(_BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_ON));
}