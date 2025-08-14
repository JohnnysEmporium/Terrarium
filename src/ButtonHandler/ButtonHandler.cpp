#include "ButtonHandler.hpp"
#include "Button.hpp"

uint16_t TIME_EDIT_TIMEFRAME = 5000;
uint16_t TIME_EDIT_BLINK_INTERVAL1 = 200;
uint16_t TIME_EDIT_BLINK_INTERVAL2 = 700;
uint16_t TIME_EDIT_FAST_FORWARD_INTERVAL = 100;

extern bool isTimeEditEngaged = false;
bool blinkFlag = false;

unsigned long timeEditStartTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long timeEditFFTime = 0;

bool wasLCDWakeUp = false;
bool time_printed_after_edit = false;

ptEnum time_editing_section = LCD_H;

void timeIncrementBasedOnSection() {
  incrementCounter(time_editing_section);
}

bool getTimeEditEngaged() {
  return isTimeEditEngaged;
}

void resetTimeEditStartTime() {
  timeEditStartTime = time();
}

void btnSetLongPress() {
  if (isTimeEditEngaged) {
    unsigned long now = time();
    if (now - timeEditFFTime >= TIME_EDIT_FAST_FORWARD_INTERVAL) {
      resetTimeEditStartTime();
      timeIncrementBasedOnSection();
      timeEditFFTime = time();
    }
  }
}

void btnSetShortPress() {
  if (isTimeEditEngaged) {
    resetTimeEditStartTime();
    timeIncrementBasedOnSection();
  }
}

void btnChangeLongPress() {
  if (!isTimeEditEngaged && !isPrintingStats()) {
    resetTimeEditStartTime();
    isTimeEditEngaged = true;
    time_editing_section = LCD_H;
  }
}

void btnChangeShortPress() {
  if (isTimeEditEngaged) {
    setTimeToPrint();
    int temp = static_cast<uint8_t>(time_editing_section);
    temp++;
    temp = temp % 3;
    time_editing_section = static_cast<ptEnum>(temp);
    timeEditStartTime = time();
  } else {
    setPrintStats();
  }
}

void timeEditBlink() {
  if (isTimeEditEngaged) {

    unsigned long now = time();
    if (!blinkFlag && now - lastBlinkTime >= TIME_EDIT_BLINK_INTERVAL2) {
      uint8_t lcd_col;
      uint8_t lcd_row;
      switch (time_editing_section) {
        case LCD_H:
          lcd_col = LCD_H_POS_COL;
          lcd_row = LCD_H_POS_ROW;
          break;
        case LCD_M:
          lcd_col = LCD_M_POS_COL;
          lcd_row = LCD_M_POS_ROW;
          break;
        case LCD_S:
          lcd_col = LCD_S_POS_COL;
          lcd_row = LCD_S_POS_ROW;
          break;
      }

      LCDGoTo(lcd_col, lcd_row);
      LCDPuts("  ");
      blinkFlag = true;
      lastBlinkTime = time();

    } else if (blinkFlag && now - lastBlinkTime >= TIME_EDIT_BLINK_INTERVAL1) {
      setTimeToPrint(time_editing_section);
      blinkFlag = false;
      lastBlinkTime = time();
    }

    if (now - timeEditStartTime >= TIME_EDIT_TIMEFRAME) {
      setTimeToPrint();
      isTimeEditEngaged = false;
      blinkFlag = false;
      lastBlinkTime = 0;
    }
  }
}

Button btnSet(
    &DDRD, &PORTD, &PIND, PIND7,
    btnSetShortPress, btnSetLongPress
);

Button btnChg(
    &DDRD, &PORTD, &PIND, PIND2,
    btnChangeShortPress, btnChangeLongPress
);

void initButtonHandler() {
    btnSet.begin();
    btnChg.begin();
}

void buttonLoop() {
  btnSet.update();
  btnChg.update();
  timeEditBlink();
}

