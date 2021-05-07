#include "includes.h"
constexpr uint8_t sampleSize = 10;
constexpr uint16_t threshold = 990;

Button::Button (byte pinParam, const byte pinTypeParam) : pin {pinParam}, pinType {pinTypeParam} {
  pinMode(pin, pinType);
}
bool Button::read (ButtonReadMode mode, const uint16_t repeatRate) {
  uint8_t highs = 0;
  uint8_t lows = 0;
  for (uint8_t i = 0; i < sampleSize; ++i) {
    if (digitalRead(pin))
      highs++;
    else
      lows++;
  }
  bool buttonState = (pinType == INPUT_PULLUP ? lows > highs : highs > lows);
  bool output;
  if (mode == Normal)
    output = buttonState && (millis() - lastFired >= repeatRate);
  if (mode == Push || mode == Press)
    output = (buttonState == HIGH && pressing == false);
  if (mode == Release)
    output = (buttonState == LOW && pressing == true);
  if (mode == Change)
    output = (buttonState != pressing);
  pressing = buttonState;
  if (output) {
    lastFired = millis();
  }
  return output;
}
ResistiveButton::ResistiveButton (byte pinParam) : Button (pinParam, INPUT_PULLUP) {}
bool ResistiveButton::read (ButtonReadMode mode, uint16_t repeatRate) {
  uint8_t highs = 0;
  uint8_t lows = 0;
  for (uint8_t i = 0; i < sampleSize; ++i) {
    if (analogRead(pin) >= threshold)
      highs++;
    else
      lows++;
  }
  bool buttonState = highs < lows;
  bool output;
  if (mode == Normal)
    output = buttonState && (millis() - lastFired >= repeatRate);
  if (mode == Push || mode == Press)
    output = (buttonState == HIGH && pressing == false);
  if (mode == Release)
    output = (buttonState == LOW && pressing == true);
  if (mode == Change)
    output = (buttonState != pressing);
  pressing = buttonState;
  if (output) {
    lastFired = millis();
  }
  return output;
}
uint16_t ResistiveButton::readRaw() const {
  return analogRead(pin);
}

Button leftButton(2, INPUT_PULLUP);
Button jumpButton(3, INPUT_PULLUP);
Button rightButton(4, INPUT_PULLUP);
ResistiveButton turnLeftButton(A2);
ResistiveButton turnRightButton(A3);
ResistiveButton leftMouseButton(A0);
ResistiveButton rightMouseButton(A1);
