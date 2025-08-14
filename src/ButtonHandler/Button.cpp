#include "Button.hpp"
#include "../LCDHandler/LCDHandler.hpp"

Button::Button(uint8_t* ddr, uint8_t* port, uint8_t* pinReg, uint8_t pin,
              void (*shortPressHandler)(),
              void (*longPressHandler)(),
              uint8_t debounceDelay,
              uint16_t shortPressTime,
              uint16_t longPressTime)
    : ddr(ddr),
      port(port),
      pinReg(pinReg),
      pin(pin),
      debounceDelay(debounceDelay),
      shortPressTime(shortPressTime),
      longPressTime(longPressTime),
      shortPressHandler(shortPressHandler),
      longPressHandler(longPressHandler),
      state(ButtonState::IDLE),
      debounced(false),
      isShortPress(false),
      isLongPress(false),
      lastDebounceTime(0),
      pressStartTime(0),
      longPressHandled(false),
      longPress(false),
      needsDepressing(false)
{}

void Button::begin() {
    *ddr &= ~(1 << pin);  // Set pin as input
    *port |= (1 << pin);  // Enable pull-up resistor
}

void Button::update() {
  // false - btn pressed, true - btn depressed
  bool rawState = *pinReg & (1 << pin);
  unsigned long now;

  switch (state) {
    case ButtonState::IDLE:
      if (rawState == false && !needsDepressing) {
        LCDResetSleepTimer();
        state = ButtonState::DEBOUNCING;
        lastDebounceTime = time();

      // Holds any button functions when the screen was just turned on, until button is depressed
      } else if (rawState == true && needsDepressing) {
        needsDepressing = false;

      } else if (rawState == false) {
        LCDResetSleepTimer();
      }
      break;

    case ButtonState::DEBOUNCING:
      now = time();
      if (now - lastDebounceTime >= debounceDelay) {
        if (rawState == false) {
          state = ButtonState::PRESSED;
          pressStartTime = time();
          longPressHandled = false;
        } else {
          state = ButtonState::IDLE;
        }
      }
      break;

    case ButtonState::PRESSED:
      now = time();

      if (!isLCDOn()) {
        LCDOn();
        state = ButtonState::IDLE;
        needsDepressing = true;

      } else {
        if (rawState == true && !longPress) {
          if (!longPressHandled && now - pressStartTime >= shortPressTime) {
            (*shortPressHandler)();
          }
          state = ButtonState::IDLE;

        } else if ((!longPressHandled && now - pressStartTime >= longPressTime) || longPress) {
          longPress = true;
          if (rawState == true) {
            longPressHandled = true;
            longPress = false;
          }
          (*longPressHandler)();
        }
      }
      break;
  }
}
