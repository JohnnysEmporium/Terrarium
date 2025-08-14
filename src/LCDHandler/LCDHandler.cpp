#include "LCDHandler.hpp"

uint8_t OLD_VALUE = 0;

bool *SET_FLAG_AFTER_SECONDS_RUNNING_LCD_TIMER_ADDR;
bool turn_off_lcd = false;
bool IS_LCD_ON;

uint8_t printTimePosition;
char printTemp[4];
char printHum[4];


bool PRINT_HOURS = true;
bool PRINT_MINUTES = true;
bool PRINT_SECONDS = true;

bool isPrintingStatsBool = false;
unsigned long wakeTime;

LiquidCrystal_PCF8574 lcd(0x27);

void LCDResetSleepTimer() {
  wakeTime = time();
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

void printTime(uint8_t col, uint8_t row, uint8_t timeValue) {
  lcd.setCursor(col, row);
  lcd.print(intToPrintableTime(timeValue));
}

void printConstantSymbols() {
  lcd.clear();
  lcd.setCursor(LCD_SEM_0_COL, LCD_SEM_0_ROW);
  lcd.print(":");
  lcd.setCursor(LCD_SEM_1_COL, LCD_SEM_1_ROW);
  lcd.print(":");

  lcd.setCursor(LCD_TEMP_CONST_COL, LCD_TEMP_CONST_ROW);
  lcd.print("T: ");
  lcd.setCursor(LCD_RH_CONST_COL, LCD_RH_CONST_ROW);
  lcd.print("RH: ");
}


void printMainScreenTime() {
  lcd.setCursor(LCD_SEM_0_COL, LCD_SEM_0_ROW);
  lcd.print(":");
  lcd.setCursor(LCD_SEM_1_COL, LCD_SEM_1_ROW);
  lcd.print(":");

  if (PRINT_HOURS) {
    printTime(LCD_H_POS_COL, LCD_H_POS_ROW, H);
    PRINT_HOURS = false;
  }

  if (PRINT_MINUTES) {
    printTime(LCD_M_POS_COL, LCD_M_POS_ROW, M);
    PRINT_MINUTES = false;
  }

  if (PRINT_SECONDS) {
    printTime(LCD_S_POS_COL, LCD_S_POS_ROW, S);
    PRINT_SECONDS = false;
  }
}

void setTimeToPrint() {
  PRINT_HOURS = true;
  PRINT_MINUTES = true;
  PRINT_SECONDS = true;
}

void setTimeToPrint(ptEnum position) {
  switch (position) {
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

// temp_char and hum_char come from DHTHandler
void printTempAndHum() {
  lcd.setCursor(LCD_TEMP_COL, LCD_TEMP_ROW);
  lcd.print(DHTgetTempChar());
  lcd.setCursor(LCD_HUM_COL, LCD_HUM_ROW);
  lcd.print(DHTgetHumChar());
}

void printMainScreenAll() {
  setTimeToPrint();
  printConstantSymbols();
  printMainScreenTime();
  printTempAndHum();
}

void LCDInit() {
  IS_LCD_ON = true;
  lcd.begin(16, 2);
  lcd.setBacklight(2);
  LCDResetSleepTimer();
  printConstantSymbols();
}

bool isLCDOn() {
  return IS_LCD_ON;
}

void LCDOff() {
  // lcd.off();
  lcd.noDisplay();
  lcd.setBacklight(0);
  IS_LCD_ON = false;

}

void LCDOn() {
  if (!IS_LCD_ON) {
    lcd.display();
    lcd.setBacklight(50);
    lcd.begin(16, 4);
    printMainScreenAll();
    LCDResetSleepTimer();
    IS_LCD_ON = true;
  } else {
    LCDResetSleepTimer();
  }
}

void LCDPuts(char *s) {
  lcd.print(s);
}

void LCDPuts(double s) {
  lcd.print(s);
}

void LCDGoTo(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void LCDWakeUp() {
  if (!IS_LCD_ON) {
    LCDOn();
    LCDResetSleepTimer();
  }

}

void LCDOffTimer() {
  if (IS_LCD_ON) {
    unsigned long now = time();
    if (now - wakeTime >= 10000) {
      LCDOff();
      IS_LCD_ON = false;
    }
  }
}

void setPrintStats() {
  isPrintingStatsBool = !isPrintingStatsBool;
  lcd.clear();
  if (!isPrintingStatsBool) {
      printMainScreenAll();
  }
}

bool isPrintingStats() {
  return isPrintingStatsBool;
}

void printStats() {

  lcd.setCursor(0x00, 0);
  lcd.print("Lamp_t:");
  printTime(0x08, 0, lampWorkingH);

  lcd.setCursor(0x0A, 0);
  lcd.print(":");
  printTime(0x0B, 0, lampWorkingM);

  lcd.setCursor(0x0D, 0);
  lcd.print(":");
  printTime(0x0E, 0, lampWorkingS);


  lcd.setCursor(0x00, 1);
  lcd.print("Humid_t:");
  printTime(0x08, 1, humidifierWorkingH);

  lcd.setCursor(0x0A, 1);
  lcd.print(":");
  printTime(0x0B, 1, humidifierWorkingM);

  lcd.setCursor(0x0D, 1);
  lcd.print(":");
  printTime(0x0E, 1, humidifierWorkingS);
}

void printScreenLoop() {
  if (!IS_LCD_ON) return;

  if (isPrintingStatsBool) {
    printStats();

  } else {
    printMainScreenTime();
    printTempAndHum();
  }
}
