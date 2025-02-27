#include "LCDHandler.hpp"

uint8_t OLD_VALUE = 0;

DelayHandler delayHandler;

bool *SET_FLAG_AFTER_SECONDS_RUNNING_LCD_TIMER_ADDR;
bool turn_off_lcd = false;
bool IS_LCD_ON;

void LCDInit(){
    lcd_init();
    LCDPutConstantSymbols();
    DDRC |= (1 << PC0);
    PORTC |= (1 << PC0);
    IS_LCD_ON = true;
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
  DDRD &= ~(1 << PIND4);
  DDRD &= ~(1 << PIND5);
  DDRD &= ~(1 << PIND6);
  DDRD &= ~(1 << PIND7);
  DDRD &= ~(1 << PIND1);
  DDRD &= ~(1 << PIND3);
  PORTC &= ~(1 << PC0);
  IS_LCD_ON = false;
}

void LCDOn(){
  lcd_init();
  LCDPutConstantSymbols();
  PORTC |= (1 << PC0);
  IS_LCD_ON = true;
  //printDHTValues();  
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

void LCDWakeUp(){
  if(!IS_LCD_ON){
    LCDOn();
    printTime();
  }
  delayHandler.resetSecondsCounter();
}

void LCDOffTimer(){
  if(IS_LCD_ON){
    SET_FLAG_AFTER_SECONDS_RUNNING_LCD_TIMER_ADDR = delayHandler.set_flag_after_seconds(7, turn_off_lcd);
    if(turn_off_lcd) {
      delayHandler.resetSecondsCounter();
      turn_off_lcd = false;
      LCDOff();
    }
  }
}

bool isLCDOn(){
  return IS_LCD_ON;
}

void printMainScreen(){
  lcd_clrscr();
  printTime();
  printTempAndHum();
}

void printTime(){
  printTime(LCD_H, H);
  printTime(LCD_M, M);
  printTime(LCD_S, S);
}

/*
 * Ifs are handling the display of a number < 10 by
 * adding 0 at the begening of char array
 */
void printTime(int c, uint8_t val) {
  
  if(stopMainScreenPrinting) {
    lcd_goto(LCD_DEBUG_POS+0x01);
    lcd_puts("B");
  }

  if(!stopMainScreenPrinting){
    lcd_goto(LCD_SEM_0);
    lcd_puts(":");
    lcd_goto(LCD_SEM_1);
    lcd_puts(":");

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
  } else {
    lcd_goto(LCD_DEBUG_POS+0x01);
    lcd_puts("B");
  }
}

void printTempAndHum(){
  printTempAndHum(temp_char, hum_char);
}

void printTempAndHum(char temp[], char hum[]){
  if(!stopMainScreenPrinting){
    lcd_goto(LCD_TEMP_CONST);
    lcd_puts("T:");
    lcd_goto(LCD_RH_CONST);
    lcd_puts("RH:");
    lcd_goto(LCD_TEMP);
    lcd_puts(temp);
    lcd_goto(LCD_HUM);
    lcd_puts(hum);  
  }
}

void printStats() {
  lcd_clrscr();
  lcd_goto(0x00);
  lcd_puts("Lamp_t:");
  lcd_goto(0x08);
  // lcd_puts();
}