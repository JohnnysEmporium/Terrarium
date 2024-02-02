#include "../hd44780/hd44780.h"
#include "../LCD_handler/LCDHandler.hpp"
#include <string.h>
#include <stdlib.h>

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
        lcd_goto(LCD_H);
        lcd_puts(h_disp);
      } else {
        lcd_goto(LCD_H);
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
        lcd_goto(LCD_M);
        lcd_puts(m_disp);
      } else {
        lcd_goto(LCD_M);
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
        lcd_goto(LCD_S);
        lcd_puts(s_disp);
      } else {
        lcd_goto(LCD_S);
        lcd_puts(buf);
      }
      break; 
    }
}