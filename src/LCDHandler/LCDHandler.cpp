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

char* intToPrintableTime(uint8_t timeInt) {
  static char timeChar[4];

  if (timeInt < 10) {
      timeChar[0] = '0';
      itoa(timeInt, &timeChar[1], 10);
  } else {
      itoa(timeInt, timeChar, 10);
  }
  return timeChar;
}

void printTime(uint8_t pos, uint8_t timeValue){
  lcd_goto(pos);
  lcd_puts(intToPrintableTime(timeValue));
}

void printMainScreenTime() {
  lcd_goto(LCD_SEM_0);
  lcd_puts(":");
  lcd_goto(LCD_SEM_1);
  lcd_puts(":");

  if(PRINT_HOURS){
    printTime(LCD_H_POS, H);
    PRINT_HOURS = false;
  }

  if(PRINT_MINUTES){
    printTime(LCD_M_POS, M);
  }

  if(PRINT_SECONDS){
    printTime(LCD_S_POS, S);
    PRINT_SECONDS = false;
  }
}

void printStats() {
  lcd_goto(0x00);
  lcd_puts("Lamp_t:");
  printTime(0x08, lampWorkingH);
  lcd_goto(0x0A);
  lcd_puts(":");
  printTime(0x0B, lampWorkingM);
  lcd_goto(0xD);
  lcd_puts(":");
  printTime(0xE, lampWorkingS);

  lcd_goto(0x40);
  lcd_puts("Humid_t:");
  printTime(0x48, humidifierWorkingH);
  lcd_goto(0x4A);
  lcd_puts(":");
  printTime(0x4B, humidifierWorkingM);
  lcd_goto(0x4D);
  lcd_puts(":");
  printTime(0x4E, humidifierWorkingS);
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
      printMainScreenTime();
    
    } else {
      printMainScreenTime();
    }
    
    printFullTIme = false;
    printTempAndHum();
  }
}