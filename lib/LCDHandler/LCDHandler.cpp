extern "C" {
  #include "../hd44780/hd44780.h"
}
#include "LCDHandler.hpp"
#include "avr\sfr_defs.h"
#include <stdlib.h>
#include <string.h>

uint8_t OLD_VALUE = 0;

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

void LCDPuts(const char *s){
  lcd_puts(s);
}

void LCDGoTo(uint8_t pos){
  lcd_goto(pos);
}
// void LCDBlink(int position, ){
//   lcd_goto(position);
//   lcd_puts("  ");

// }

/*
 * Ifs are handling the display of a number < 10 by
 * adding 0 at the begening of char array
 */
void printTime(int c, uint8_t val) {
  switch(c){
    char buf[8];

    case 0:
      itoa(val, buf, 10);
      if (val < 10) {
        char h_disp[3];
        h_disp[0] = '0';
        h_disp[1] = '\0';
        strcat(h_disp, buf);
        lcd_goto(LCD_H_POS);
        lcd_puts(h_disp);
      } else {
        lcd_goto(LCD_H_POS);
        lcd_puts(buf);
      }    
      break;

    case 1:
      itoa(val, buf, 10);
      if (val < 10) {
        char m_disp[3];
        m_disp[0] = '0';
        m_disp[1] = '\0';
        strcat(m_disp, buf);
        lcd_goto(LCD_M_POS);
        lcd_puts(m_disp);
      } else {
        lcd_goto(LCD_M_POS);
        lcd_puts(buf);
      } 
      break; 

    case 2:
      itoa(val, buf, 10);
      if (val < 10) {  
        char s_disp[3];
        s_disp[0] = '0';
        s_disp[1] = '\0';
        strcat(s_disp, buf);
        lcd_goto(LCD_S_POS);
        lcd_puts(s_disp);
      } else {
        lcd_goto(LCD_S_POS);
        lcd_puts(buf);
      }
      break; 
    }
}