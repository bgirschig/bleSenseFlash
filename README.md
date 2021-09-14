# Arduino BLE sense NVMC tests
This is a test of how to use the Arduino's NVMC to store persistent data.

- Ble communication is added to make sure it is not affected and does not interfere with this technique
- Blinker is added to monitor the state of the arduino without using Serial (which has affeced some of the tests, so can't be relied upon in this case)
