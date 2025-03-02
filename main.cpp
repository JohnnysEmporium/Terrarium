extern "C" {
  #include "lib/hd44780/hd44780.h"
  #include "lib/AVR-DHT/DHT.h"
}
#include <avr/interrupt.h>
#include <util/delay.h>
#include "./src/LCDHandler/LCDHandler.hpp"
#include "./src/ButtonHandler/ButtonHandler.hpp"
#include "./src/DHTHandler/DHTHandler.hpp"
#include "./src/TimeHandler/TimeHandler.hpp"


//Setting up necessary resources
void setup() {
  initTimeHandler();
  //passing time_editing_engaged by reference
  initButtonHandler();
  LCDInit();
  DHTInit();
  
  DDRD &= ~(1 << PIND0);
  PORTD |= (1 << PIND0);

  DDRB |= (1 << PINB1);
  sei();
}

//For stopping lights after evening, use one of the sleep modes (idle?) by setting SM bits, wake up with timer1 and/or INT0 interrupts
int main()
{
  setup();
  while (1)
  {
    manageTime();
    printScreen();
    buttonLoop();
    // LCDOffTimer();


    // if(bit_is_clear(PIND, 0)){
    //   setTimeToPrint(0, 20);
    // } else {
    //   setTimeToPrint(0,10);
    // }

    // PORTB ^= (1 << PINB1);
    // _delay_ms(1000);
    // PORTB ^= (1 << PINB1);
    // _delay_ms(2000);

  }
}

// TODO:  fix setTimeToPrint in LCDHandler, are these many repetitions necessary? Maybe use h_disp[1] = buff; instead of strcat and \0
//        fix time increment - it goes too fast
//        declassify button handler, clear up unnecessary defined variables in hpp, ask gtp what should be in hpp and what not