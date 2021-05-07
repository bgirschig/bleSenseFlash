#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include <Arduino.h>
#include <stdbool.h>

namespace flashStorage {
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

  typedef struct flash_mem {
    int counter = 1;
    float val_1;
    float val_2;
    // We want to fill a whole page of memory.
    // A page is 4096 bytes, each float takes 4 bytes.
    char filler[4096 - (4+4+4) ];
  } flash_mem_t;

  // This will reserve a space in flash memory for the values we need to save.
  const flash_mem_t _values __attribute__((section("FLASH"), aligned(0x1000))) = {};
  // A regular, in-memory instance of our values for easy manipulations.
  // we'll use the load() and save() functions to move data between values and _values
  extern flash_mem_t values;

  void writeValues ();
  bool save();
  bool deletePage(void *pageStart);
  void load ();
  float getFloat(float *ptr);
  int getInt(int *ptr);

  /* Dump memory as HEX values to the serial interface starting at memStart for nbrOfBytes */
  void hexDumpMemory(uint8_t *memStart, const unsigned int nbrOfBytes);
}

#endif