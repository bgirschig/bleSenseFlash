// Ispired by https://github.com/tjpetz/BLESenseFlash/blob/master/BLESenseFlash.ino

#include <Arduino.h>
#include <stdbool.h>
#include "flashStorage.h"

void setup() {
  // Initialize serial
  Serial.begin(115200);
  while (!Serial.ready()) { }
  delay(500);

  // Load and show saved values
  flashStorage::load();
  if (Serial.ready()) {
    Serial.println(flashStorage::values.val_1);
    Serial.println(flashStorage::values.val_2);
    Serial.println(flashStorage::values.counter);
  }

  // Update and save values
  flashStorage::values.counter += 1;
  flashStorage::values.val_1 += 1;
  flashStorage::values.val_2 += 0.642;
  flashStorage::save();
}

// Arduino expects this
void loop() {
}
