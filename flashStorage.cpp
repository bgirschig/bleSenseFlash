#include "flashStorage.h"

namespace flashStorage {
  flash_mem_t values = {};

  void writeValues () {
    // TODO: find a way to do this for all values in our struct automatically
    *(float *)(&_values.val_1) = values.val_1;
    *(float *)(&_values.val_2) = values.val_2;
    *(int *)(&_values.counter) = values.counter;
  }

  void load () {
    // TODO: find a way to do this for all values in our struct automatically
    values.val_1 = getFloat((float *)&_values.val_1);
    values.val_2 = getFloat((float *)&_values.val_2);
    values.counter = getInt((int *)&_values.counter);
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
    if (Serial.ready()) Serial.println(value);
    return value;
  }

  int getInt(int *ptr) {
    int value = *ptr;
    if (Serial.ready()) Serial.println(value);
    return value;
  }

  void hexDumpMemory(uint8_t *memStart, const unsigned int nbrOfBytes) {
    unsigned int bytesPerLine = 16;

    Serial.print("Memory dump starting at: ");
    Serial.println((unsigned long)(memStart), HEX);

    int lineCount = nbrOfBytes / bytesPerLine;
    uint8_t *currentPointer = memStart;
    for (unsigned int lineIdx = 0; lineIdx < lineCount; lineIdx ++) {
      Serial.print((unsigned long)currentPointer, HEX);
      Serial.print(": ");

      for (unsigned int columnIdx = 0; columnIdx < bytesPerLine; columnIdx ++) {
        char value = *(currentPointer);
        // leading 0
        if (value < 0x10) Serial.print("0");
        // HEX value
        Serial.print(value, HEX);
        // separator
        Serial.print(" ");
      
        currentPointer += 1;
      }
      Serial.print("\n");
    }
  }
}