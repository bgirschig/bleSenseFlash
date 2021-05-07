#ifndef Blinker_h
#define Blinker_h

#include <Arduino.h>

class Blinker {
  public:
    Blinker(int flash_duration, int pause_duration, int pin);

    void set_flash_count(int flash_count);
    void update();
  
  private:
    int flash_count;
    int flash_duration;
    int pause_duration;
    int pattern_duration;
    float half_flash;
};

#endif