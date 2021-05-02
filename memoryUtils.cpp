#include "memoryUtils.h"

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