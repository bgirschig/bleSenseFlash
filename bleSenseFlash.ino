// Ispired by https://github.com/tjpetz/BLESenseFlash/blob/master/BLESenseFlash.ino

#include <Arduino.h>
#include <stdbool.h>
#include "flashStorage.h"
#include "Blinker.h"
#include "bleClient.h"
#include <Arduino_LSM9DS1.h>


Blinker statusBlinker(400, 1000, LED_BUILTIN);
BleClient bleClient {};

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Load values from flash storage
  flashStorage::load();

  // Update and save counter
  flashStorage::values.counter += 1;
  flashStorage::save();

  IMU.begin();

  bleClient.init();
  bleClient.updateCounter(flashStorage::values.counter);

  statusBlinker.set_flash_count(flashStorage::values.counter);
}

void loop() {
  statusBlinker.update();
  bleClient.update();

  if (IMU.accelerationAvailable()) {
    float x, y, z;
    IMU.readAcceleration(x, y, z);
    bleClient.updateAccel(x,y,z);
  }
}
