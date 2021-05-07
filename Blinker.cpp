/* LED blinker for displaying codes (a number of flashes) with an LED */

#include <Arduino.h>
#include "Blinker.h"

Blinker::Blinker(int flash_duration, int pause_duration, int pin) {
  this->flash_duration = flash_duration;
  this->pause_duration = pause_duration;
  half_flash = flash_duration / 2.0;

  set_flash_count(0);
  pinMode(pin, OUTPUT);
}

void Blinker::set_flash_count(int flash_count) {
  this->flash_count = flash_count;
  this->pattern_duration = pause_duration + flash_count * flash_duration;
}

void Blinker::update() {
  unsigned long now = millis();
  float t = fmod(now, pattern_duration) - pause_duration;

  if (t < 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else if (fmod(t, flash_duration) < half_flash) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}