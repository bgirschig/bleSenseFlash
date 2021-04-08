// https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.2.pdf
// https://store.arduino.cc/arduino-nano-33-ble-sense
// https://github.com/tjpetz/BLESenseFlash/blob/master/BLESenseFlash.ino

#include <Arduino.h>
#include <stdbool.h>

// nFR52 NVMC registers
#define NVMC_BASE (0x4001E000U)
#define NVMC_READY (NVMC_BASE + 0x400U)
#define NVMC_READYNEXT (NVMC_BASE + 0x408U)
#define NVMC_CONFIG (NVMC_BASE + 0x504U)
#define NVMC_ERASEPAGE (NVMC_BASE + 0x508U)
#define NVMC_ERASEALL (NVMC_BASE + 0x50CU)
#define NVMC_ERASEUICR (NVMC_BASE + 0x514U)
#define NVMC_ERASEPAGEPARTIAL (NVMC_BASE + 0X518U)
#define NVMC_ERASEPAGEPARTIALCFG (NVMC_BASE + 0X51CU)
#define NVMC_ICACHECNF (NVMC_BASE + 0x540U)
#define NVMC_IHIT (NVMC_BASE + 0x548U)
#define NVMC_IMISS (NMVC_BASE + 0x54cU)

// nFR52 MVMC values
#define MVMC_READ_MODE 0x00
#define MVMC_WRITE_MODE 0x01
#define MVMC_ERASE_MODE 0x02

typedef struct flash_mem
{
  float val_1;
  float val_2;
  char filler[4096 - (4+4) ];
} flash_mem_t;

const flash_mem_t myFlash __attribute__((section("FLASH"), aligned(0x1000))) = {
  0,
  0
};

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial.ready()) { }
  delay(500);

  uint8_t * ptr = (uint8_t *)(&myFlash);

  Serial.println("--------- A");
  showByte(ptr);
  //
  Serial.println("--------- B");
  Serial.println(getFloat(ptr));
  //
  Serial.println("--------- C");
  char value = *ptr;
  Serial.println(value, HEX);

  hexDumpMemory((uint8_t *)(&myFlash), 128);
  
  deleteFlash();
  hexDumpMemory((uint8_t *)(&myFlash), 128);
  
  bool success = flashMem((uint8_t *)&myFlash);
  if (!success) Serial.println("... Flash Not Ready! ...");
  hexDumpMemory((uint8_t *)(&myFlash), 256);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(750);
  digitalWrite(LED_BUILTIN, LOW);
  delay(750);
}

bool flashMem(uint8_t *location) {
  if (*(uint32_t *)NVMC_READY == true) {
    *(uint32_t *)NVMC_CONFIG = MVMC_WRITE_MODE;
    *(float *)(&myFlash.val_1) = 15.535;
    *(float *)(&myFlash.val_2) = 2863.6578;

    while(*(uint32_t *)NVMC_READY == false)
      delayMicroseconds(50);
    *(uint32_t *)NVMC_CONFIG = MVMC_READ_MODE;

    return true;
  } else {
    return false;
  }
}

bool deleteFlash() {
  if (*(uint32_t *)NVMC_READY == true)
  {
    *(uint32_t *)NVMC_CONFIG = MVMC_ERASE_MODE;
    *(uint32_t *)NVMC_ERASEPAGE = (uint32_t)(&myFlash);
    while (*(uint32_t *)NVMC_READY == false)
      delay(85);
    *(uint32_t *)NVMC_CONFIG = MVMC_READ_MODE;
    return true;
  }
  else
  {
    return false;
  }
}

float getFloat(uint8_t *ptr)
{
  float value = *ptr;
  char value2 = *ptr;
  Serial.println(value);
  return value;
}

void showByte(uint8_t *ptr)
{
  char value = *ptr;
  Serial.print((unsigned long)ptr, HEX);
  Serial.print(" ");
  Serial.println(value, HEX);
}

void hexDumpMemory(uint8_t *memStart, const unsigned int nbrOfBytes)
{
  /* hex dump memory to the serial interface starting at memStart for nbrOfBytes */

  uint8_t *ptr;
  int bytesPerLine = 16;
  int bytesOnLine = 0;

  ptr = memStart;

  Serial.print("Memory dump of: ");
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