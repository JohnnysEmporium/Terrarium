#include "LCDHandler.hpp"

uint8_t OLD_VALUE = 0;

DelayHandler LCDWakeUp_DH;
DelayHandler statsScreen_DH;

bool *SET_FLAG_AFTER_SECONDS_RUNNING_LCD_TIMER_ADDR;
bool turn_off_lcd = false;
bool IS_LCD_ON;

uint8_t printTimePosition;
char printTemp[4];
char printHum[4];


bool PRINT_HOURS = true;
bool PRINT_MINUTES = true;
bool PRINT_SECONDS = true;

bool printStatsBool_old = false;
bool printFullTIme = true;

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
  }
  LCDWakeUp_DH.resetSecondsCounter();
}

void LCDOffTimer(){
  if(IS_LCD_ON){
    SET_FLAG_AFTER_SECONDS_RUNNING_LCD_TIMER_ADDR = LCDWakeUp_DH.set_flag_after_seconds(7, turn_off_lcd);
    if(turn_off_lcd) {
      LCDWakeUp_DH.resetSecondsCounter();
      turn_off_lcd = false;
      LCDOff();
    }
  }
}

bool isLCDOn(){
  return IS_LCD_ON;
}

void setTimeToPrint(){
  PRINT_HOURS = true;
  PRINT_MINUTES = true;
  PRINT_SECONDS = true;
}

void setTimeToPrint(ptEnum position){
  switch(position) {
    case(LCD_H):
      PRINT_HOURS = true;
      break;
    case(LCD_M):
      PRINT_MINUTES = true;
      break;
    case(LCD_S):
      PRINT_SECONDS = true;
      break;
  }
}

/*
 * Ifs are handling the display of a number < 10 by
 * adding 0 at the begening of char array
 */

//temp_char and hum_char come from DHTHandler
void printTempAndHum(){
  lcd_goto(LCD_TEMP_CONST);
  lcd_puts("T:");
  lcd_goto(LCD_RH_CONST);
  lcd_puts("RH:");
  lcd_goto(LCD_TEMP);
  lcd_puts(temp_char);
  lcd_goto(LCD_HUM);
  lcd_puts(hum_char);  
}

void printTime() {
  lcd_goto(LCD_SEM_0);
  lcd_puts(":");
  lcd_goto(LCD_SEM_1);
  lcd_puts(":");

  
    char buf[8];

  if(PRINT_HOURS){
    itoa(H, buf, 10);
    if (H < 10) {
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
    PRINT_HOURS = false;
  }

  if(PRINT_MINUTES){
    itoa(M, buf, 10);
    if (M < 10) {
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
    PRINT_MINUTES = false;
  }

  if(PRINT_SECONDS){
    itoa(S, buf, 10);
    if (S < 10) {  
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
    PRINT_SECONDS = false;
  }
}

void printStats() {
  lcd_goto(0x00);
  lcd_puts("Lamp_t:");
  lcd_goto(0x08);
  // lcd_puts();
}

void printScreen(){
  if(!IS_LCD_ON) return;

  if(printStatsBool != printStatsBool_old) {
    lcd_clrscr();
    printFullTIme = true;
  }

  printStatsBool_old = printStatsBool;

  if(printStatsBool){
    statsScreen_DH.set_flag_after_seconds(5, printStatsBool);
    printStats();
  
  } else {
    if(printFullTIme){
      setTimeToPrint();
      printTime();
    
    } else {
      printTime();
    }
    
    printFullTIme = false;
    printTempAndHum();
  }
}