#include "../hd44780/hd44780.h"
#include "../LCD_handler/LCDHandler.hpp"
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>

uint8_t MSEC_OLD = -1;
volatile uint8_t **MSEC_CNT_ADDR;

//Timer1 initialization
void init_timer1() {
  //Timer Mode 4: Clear Timer on Compare match (CTC)
  TCCR1B |= (1 << WGM12);
  //Initialize Timer staring value
  TCNT1 = 0;
  //Set compare value for 1s overflow
  OCR1A = 31249;
  //Enable Timer Compare A Match interrupt
  TIMSK1 |= (1 << OCIE1A);
  //Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

void pause_timer1() {
  //Disabling clock source
  TCCR1B &= ~(1 << CS12);
}

void start_timer1() {
  //Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

void init_timer0() {
  //Timer Mode 2: CTC
  TCCR0A |= (1 << WGM01);
  //Initialize Timer starting value
  TCNT0 = 0;
  //Set compare value for 20ms overflow
  OCR0A = 155;
  //Enable Timer Compare A Match Interrupt
  TIMSK0 |= (1 << OCIE0A);
  //Prescale timer by 1024 and start counting
  TCCR0B |= (1 << CS00) | (1 << CS02);
}

void pause_timer0() {
  //Set CS bits to 0 - no clock source
  TCCR0A &= ~(1 << CS00) | ~(1 << CS01) | ~(1 << CS02);
}

void start_timer0() {
  //Prescale timer by 1024 and start counting
  TCCR0B |= (1 << CS00) | (1 << CS02);
}

void initTimeHandler(volatile uint8_t *MSEC_CNT_addr){
  init_timer0();
  pause_timer0();
  init_timer1();
  MSEC_CNT_ADDR = &MSEC_CNT_addr;
}

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

void wait(uint16_t milis) {
  uint16_t loop_cnt = milis/20 + 1;
  uint8_t msec = 0;
 
  while(loop_cnt != 0) {
    pause_timer0();
    msec = **MSEC_CNT_ADDR;
    printTime(0, msec);
    start_timer0();
    if(msec == MSEC_OLD + 1) {
      loop_cnt--;
    }
    MSEC_OLD = msec;
  }
  pause_timer0();
}