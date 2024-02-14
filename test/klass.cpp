#include <avr/io.h>
#include <util/delay.h>
#include "klass.hpp"

klass::klass() {
    DDRC |= (1 << PC0);
}

void klass::start() {
    while (1) {
        PORTC |= (1 << PC0);
        _delay_ms(2000);
        PORTC &= ~(1 << PC0);
        _delay_ms(2000);
    }
}