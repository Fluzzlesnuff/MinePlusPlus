#ifndef button_h
#define button_h
#include <CapacitiveSensor.h>

class Button {
  protected:
    byte pin;
    byte pinType;
    uint16_t sampleSize;
    bool pressing;
    unsigned long lastFired;
  public:
    Button (byte pinParam, byte pinTypeParam);
    bool read (ButtonReadMode mode = Normal, uint16_t repeatRate = 0);
    void setSampleSize (uint16_t sampleSizeParam);
};
class ResistiveButton : public Button {
    uint16_t threshold;
  public:
    ResistiveButton (byte pinParam);
    bool read (ButtonReadMode mode = Normal, uint16_t repeatRate = 0);
    uint16_t readRaw() const;
    void setThreshold (uint16_t thresholdParam);
};

extern Button leftButton;
extern Button jumpButton;
extern Button rightButton;
extern ResistiveButton turnLeftButton;
extern ResistiveButton turnRightButton;
extern ResistiveButton leftMouseButton;
extern ResistiveButton rightMouseButton;

#endif
