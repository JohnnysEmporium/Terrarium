extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h> 
#include <string.h>

//Coursor default position for disaplying values
#define LCD_SEM_0       0x02
#define LCD_SEM_1       0x05
#define LCD_H           0x00
#define LCD_M           0x03
#define LCD_S           0x06
#define LCD_TEMP_CONST  0x40
#define LCD_RH_CONST    0x48
#define LCD_TEMP        0x43
#define LCD_HUM         0x4C


// global variables for time management
volatile uint8_t SEC_CNT = 55;
volatile uint8_t MIN_CNT = 31;
volatile uint8_t H_CNT = 14;
uint8_t SEC_OLD = 30;
uint8_t MIN_OLD = 0;
uint8_t H_OLD = 0;

// global variables for temp and hum average
int temp_hum_run = 0;
int TEMP_VALUES[10] = {0};
int HUM_VALUES[10] = {0};
int TEMP_HUM_VALUES_SIZE = 10;
int IS_STARTING_CNT = 0;


// Timer1 initialization
void init_timer1()
{
  // Timer Mode 4: Clear Timer on Compare match (CTC)
  TCCR1B |= (1 << WGM12);
  // Initialize Timer staring value
  TCNT1 = 0;
  // Set Compare value for 1s overflow
  OCR1A = 31249;
  // Enable Timer Compare A Match interrupt
  TIMSK1 |= (1 << OCIE1A);
  // Start Timer & Clock Select: Prescale I/O clock by 256
  TCCR1B |= (1 << CS12);
}

// Timer1 output compare match A interrupt rutine
/*
 * Incrementing hours and minutes in the ISR creates possibility
 * for the uC to be put to sleep when it's functions are not needed
 */
ISR(TIMER1_COMPA_vect) {
  SEC_CNT++;
  /*
    Maybe move ifs part into the main loop
    according to this: http://www.gammon.com.au/forum/?id=11488, interrupt should only remember/change(?) value
    which should later be used in main loop - see "What interrupts are NOT" section.
  */
  if (SEC_CNT == 60) {
    SEC_CNT = 0;
    MIN_CNT++;
  }
  if (MIN_CNT == 60) {
    MIN_CNT = 0;
    H_CNT++;
  }
  if (H_CNT == 24) {
    H_CNT = 0;
  }
}

//Puts default constant symbols onto LCD
void lcdPutConstantSymbols(){
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

void managePump(){
  
}

//returns average temperature from 10 samples
int calculateAverageTemp(){
  int temp_sum = 0;
  uint8_t i;
  for(i = 0; i < TEMP_HUM_VALUES_SIZE; i++){
    temp_sum += TEMP_VALUES[i];
  }

  return temp_sum / TEMP_HUM_VALUES_SIZE;
}

//returns average humidity from 10 samples
int calculateAverageHum(){
  int hum_sum = 0;
  uint8_t i;
  for(i = 0; i < TEMP_HUM_VALUES_SIZE; i++){
    hum_sum += HUM_VALUES[i];
  }
  
  return hum_sum / TEMP_HUM_VALUES_SIZE;  
}

void readDHTSensor(){

  double temperature[1];
  double humidity[1];
  int temp_i, hum_i;
  double temp_d, hum_d;
  char temp[5];
  char hum[5];
  int temp_avg;
  int hum_avg;

  DHT_Read(temperature, humidity);
  
  switch (DHT_GetStatus()){
    
    case (DHT_Ok):      
      temp_i = temperature[0] * 10;
      hum_i = humidity[0] * 10;
      TEMP_VALUES[temp_hum_run] = temp_i;
      HUM_VALUES[temp_hum_run] = hum_i;
      temp_hum_run++;
      break;
    
    case (DHT_Error_Checksum):
      lcd_goto(LCD_TEMP_CONST);
      lcd_puts("DHT ERROR CHECKSUM");
      break;
    case (DHT_Error_Timeout):
      lcd_goto(LCD_TEMP_CONST);
      lcd_puts("DHT ERROR TIMEOUT"); 
      break;
  } 

  if(temp_hum_run >= 10){
    temp_hum_run = 0;
  }

  temp_avg = calculateAverageTemp();
  hum_avg = calculateAverageHum();
  
  temp_d = (double)temp_avg / 10;
  hum_d = (double)hum_avg / 10;
  
  dtostrf(temp_d,-4,1,temp);
  dtostrf(hum_d,-4,1,hum);
  lcd_goto(LCD_TEMP);
  lcd_puts(temp);
  lcd_goto(LCD_HUM);
  lcd_puts(hum);     
}

//Setting up necessary resources
void setup() {
  init_timer1();
  lcd_init();
  lcdPutConstantSymbols();
  DHT_Setup();
  sei();
}

/*
 * Ifs are handling the display of a number < 10 by
 * adding 0 at the begening of char array
 * 
 * Could create a separate function but this would
 * increase processor load and method execution time
 * Execution time > Program memory
 */
void manageTime(){
  static uint8_t h = 0;
  static uint8_t m = 0;
  static uint8_t s = 0;

  //Stop interrupts
  cli();
  //Assign volatile int to be processed
  h = H_CNT;
  m = MIN_CNT;
  s = SEC_CNT;
  //Restore interrupts
  sei();
  
  if(H_OLD != h){
    H_OLD = h;
    printTime(0, h);
  }
  
  if(MIN_OLD != m){
    MIN_OLD = m;
    printTime(1, m);
  }
  
  if(SEC_OLD != s){
    
    SEC_OLD = s;
    printTime(2, s);
    readDHTSensor();
    //Pump should start after the array with temp/hum values fills up, maybe embed it in an if to limit usage 1time/5s?
    if(IS_STARTING_CNT < TEMP_HUM_VALUES_SIZE){
      managePump();
    } else {
      IS_STARTING_CNT++;
    }
  }
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 interrupt
int main()
{
  setup();


  while (1)
  {
    
    manageTime();
    
  }
}
