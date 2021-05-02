// Ispired by https://github.com/tjpetz/BLESenseFlash/blob/master/BLESenseFlash.ino

#include <Arduino.h>
#include <stdbool.h>
#include "memoryUtils.h"

typedef struct flash_mem {
  float val_1;
  float val_2;
  // We want to fill a whole page of memory.
  // A page is 4096 bytes, each float takes 4 bytes.
  char filler[4096 - (4+4) ];
} flash_mem_t;

// This will reserve a space in flash memory for the values we need to save.
const flash_mem_t _values __attribute__((section("FLASH"), aligned(0x1000))) = {};
// A regular, in-memory instance of our values for easy manipulations.
// we'll use the load() and save() functions to move data between values and _values
flash_mem_t values;

void setup() {
  // Initialize serial
  Serial.begin(115200);
  while (!Serial.ready()) { }
  delay(500);

  // Load and show saved values
  load();
  Serial.println(values.val_1);
  Serial.println(values.val_2);

  // Update and save values
  values.val_1 += 1;
  values.val_2 += 0.642;
  save();
}

// Arduino expects this
void loop() {}

void load () {
  // TODO: find a way to do this for all values in our struct automatically
  values.val_1 = getFloat((float *)&_values.val_1);
  values.val_2 = getFloat((float *)&_values.val_2);
}

void writeValues () {
  // TODO: find a way to do this for all values in our struct automatically
  *(float *)(&_values.val_1) = values.val_1;
  *(float *)(&_values.val_2) = values.val_2;
}

bool save() {
  // NVMC can only write on "deleted" bytes, so we delete the page we reserved for 
  deletePage((void *)&_values);

  // make sure NVMC is ready
  if (*(uint32_t *)NVMC_READY == false) return false;

  // write values to flash
  *(uint32_t *)NVMC_CONFIG = MVMC_WRITE_MODE;
  writeValues();
  while(*(uint32_t *)NVMC_READY == false) delayMicroseconds(50);
  *(uint32_t *)NVMC_CONFIG = MVMC_READ_MODE;

  return true;
}

bool deletePage(void *pageStart) {
  if (*(uint32_t *)NVMC_READY == false) return false;
  
  *(uint32_t *)NVMC_CONFIG = MVMC_ERASE_MODE;
  *(uint32_t *)NVMC_ERASEPAGE = (uint32_t)pageStart;
  while (*(uint32_t *)NVMC_READY == false) delay(85);
  *(uint32_t *)NVMC_CONFIG = MVMC_READ_MODE;

  return true;
}

float getFloat(float *ptr) {
  float value = *ptr;
  Serial.println(value);
  return value;
}
