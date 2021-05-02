// https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.2.pdf
// https://store.arduino.cc/arduino-nano-33-ble-sense

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

/* Dump memory as HEX values to the serial interface starting at memStart for nbrOfBytes */
void hexDumpMemory(uint8_t *memStart, const unsigned int nbrOfBytes);