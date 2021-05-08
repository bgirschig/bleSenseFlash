#include "bleClient.h"

void BleClient::init() {
  initSuccess = BLE.begin();
  if (!initSuccess) return;

  BLE.setLocalName("arduino pilot");

  pilotService.addCharacteristic(switchCharacteristic);
  pilotService.addCharacteristic(counterCharacteristic);
  pilotService.addCharacteristic(accelCharacteristic);

  BLE.addService(pilotService);
  
  switchCharacteristic.writeValue(0);
  counterCharacteristic.writeValue(0);
  BLE.advertise();
}

void BleClient::updateAccel(float x, float y, float z) {
  if ((abs(x - accelerometer[0]) > 0.1) ||
      (abs(y - accelerometer[1]) > 0.1) ||
      (abs(z - accelerometer[2]) > 0.1)) {
    accelerometer[0] = x;
    accelerometer[1] = y;
    accelerometer[2] = z;

    // only write the new values if detailed report is enabled
    if (!detailedReport) return;

    const short int valueCount = 3;
    const uint bufferLength = valueCount * sizeof(float);
    uint8_t buffer[bufferLength];
    for (size_t i = 0; i < bufferLength; i++)
    {
      float val = accelerometer[i/sizeof(float)];
      buffer[i] = ((uint8_t*)&val)[i%sizeof(float)];
    }

    accelCharacteristic.writeValue(buffer, valueCount * sizeof(float));
  }
}

void BleClient::updateCounter(int val) {
  if (counter != val) {
    counter = val;
    counterCharacteristic.writeValue(val);
  }
}

void BleClient::update() {
  // listen for BLE peripherals to connect
  BLEDevice central = BLE.central();
  if (!central) return;
  if (!central.connected()) return;
  
  // If the remote device wrote to the characteristic
  if (switchCharacteristic.written()) {
    detailedReport = (bool)switchCharacteristic.value();
  }
}