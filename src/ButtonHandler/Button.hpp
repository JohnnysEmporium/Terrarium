#ifndef BUTTON
#define BUTTON

#pragma once

#include <stdint.h>
#include <avr/io.h>
#include <TimeHandler/TimeHandler.hpp>
#include <LCDHandler/LCDHandler.hpp>

#endif

enum class ButtonState {
    IDLE,
    DEBOUNCING,
    PRESSED
};

class Button {
 public:
    Button(uint8_t* ddr, uint8_t* port, uint8_t* pinReg, uint8_t pin,
                  void (*shortPressHandler)(),
                  void (*longPressHandler)(),
                  uint8_t debounceDelay = 10,
                  uint16_t shortPressTime = 1,
                  uint16_t longPressTime = 800);

    void begin();
    void update();

 private:
    uint8_t* ddr;
    uint8_t* port;
    uint8_t* pinReg;
    uint8_t pin;
    uint16_t debounceDelay;
    uint16_t shortPressTime;
    uint16_t longPressTime;
    void (*longPressHandler)();
    void (*shortPressHandler)();
    ButtonState state;
    bool debounced;
    bool isShortPress;
    bool isLongPress;
    uint32_t lastDebounceTime;
    uint32_t pressStartTime;
    bool longPressHandled;
    bool longPress;
    bool needsDepressing;
};
