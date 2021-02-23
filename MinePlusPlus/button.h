#ifndef button_h
#define button_h

#include <CapacitiveSensor.h>

class Button {
    byte pin;
    byte pinType;
    uint16_t sampleSize;
    bool pressing;
    unsigned long lastFired;
  public:
    Button (const byte pinParam, const byte pinTypeParam);
    bool read (const ButtonReadMode mode = Normal, const uint16_t repeatRate = 0);
    void setSampleSize (const uint16_t sampleSizeParam);
};

class ResistiveButton {
    byte pin;
    uint16_t sampleSize;
    uint16_t threshold;
    bool pressing;
    unsigned long lastFired;
  public:
    ResistiveButton (const byte pinParam);
    bool read (const ButtonReadMode mode = Normal, const uint16_t repeatRate = 0);
    uint16_t readRaw();
    void setSampleSize (const uint16_t sampleSizeParam);
    void setThreshold (const uint16_t thresholdParam);
};

extern Button leftButton;
extern Button jumpButton;
extern Button rightButton;

extern ResistiveButton turnLeftButton;
extern ResistiveButton turnRightButton;
extern ResistiveButton leftMouseButton;
extern ResistiveButton rightMouseButton;











//A working but unused system to manage Capacitive Buttons.
/*class CapacitiveButton {
    CapacitiveSensor button = CapacitiveSensor(0, 0);
    byte outputPin;
    byte inputPin;
    static uint16_t sampleSize;
    bool pressing;
    unsigned long lastFired;
    uint16_t threshold;
  public:
    CapacitiveButton (uint8_t outputPinParam, uint8_t inputPinParam);
    bool read (ButtonReadMode mode = Normal, uint16_t repeatRate = 0);
    void setSampleSize (uint8_t sampleSizeParam);
    void setThreshold (uint16_t thresholdParam);
    uint16_t readRaw ();
  };*/
#endif
