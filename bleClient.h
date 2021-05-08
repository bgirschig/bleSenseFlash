#ifndef BLE_CLIENT_H
#define BLE_CLIENT_H

#include <Arduino.h>
#include <ArduinoBLE.h>

class BleClient {
  public:
    void init();
    void update();
    bool initSuccess;
    void updateAccel(float x, float y, float z);
    void updateCounter(int value);
    bool detailedReport;

  private:
    BLEService pilotService {"19B10000-E8F2-537E-4F6C-D104768A1214"};
    
    BLEByteCharacteristic switchCharacteristic {"19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite};
    
    int counter;
    BLEByteCharacteristic counterCharacteristic {"19B10002-E8F2-537E-4F6C-D104768A1214", BLERead};

    float accelerometer[3];
    BLECharacteristic accelCharacteristic { "19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 3 * sizeof(float) };
};

#endif