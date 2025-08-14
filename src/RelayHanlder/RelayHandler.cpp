#include "RelayHandler.hpp"

void relayInit() {
  // Setting registry direction as output
  // HW-279 2 relay module - lamp and humidifier
  // IN1
  DDRB |= (1 << PB2);
  // IN2
  DDRC |= (1 << PC2);

  // Single relay - LED
  // TODO
}

void relay1on() {
  PORTB |= (1 << PB2);
}

void relay2on() {
  PORTC |= (1 << PC2);
}

void relay3on() {

}

void relay1off() {
  PORTB &= ~(1 << PB2);
}

void relay2off() {
  PORTC &= ~(1 << PC2);
}

void relay3off() {

}