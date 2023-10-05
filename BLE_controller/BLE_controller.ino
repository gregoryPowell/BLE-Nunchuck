/*
  This program broadcasts the Nunchuk data so it can be used as a simple BLE controller.

  Board: Arduino Nano ESP32

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  Required Extensions:
  - ArduinoBLE (by Arduino)
  - WiiChuck (by Kevin Harrington) <-- Note documentation for values if off by 1

  Author: Austin
  https://github.com/gregoryPowell/BLE-Nunchuk
*/

#include <ArduinoBLE.h>
#include <Wire.h>
#include <WiiChuck.h>

/* Manufacturing Data */
#define MANUFACTURER_NAME_STRING "Arduino"
#define MODEL_NUMBER_STRING "0001"
#define SERIAL_NUMBER_STRING "0001"
#define HARDWARE_REV_STRING "0.01"
#define SOFTWARE_REV_STRING "0.01"

/* Generated UUIDs */
#define NUNCHUCK_SERVICE_UUID "e4257d92-0c9f-4fbc-bcf2-a8361bffcfb3"
#define Z_BUTTON_UUID "9ea41596-18ec-45a9-a194-49597368e655"
#define C_BUTTON_UUID "9ea41596-18ed-45a9-a194-49597368e655"
#define JOYSTICK_X_UUID "9ea41596-18ee-45a9-a194-49597368e655"
#define JOYSTICK_Y_UUID "9ea41596-18ef-45a9-a194-49597368e655"
#define ACC_X_UUID "9ea41596-18f0-45a9-a194-49597368e655"
#define ACC_Y_UUID "9ea41596-18f1-45a9-a194-49597368e655"
#define ACC_Z_UUID "9ea41596-18f2-45a9-a194-49597368e655"
#define PITCH_UUID "9ea41596-18f3-45a9-a194-49597368e655"
#define ROLL_UUID "9ea41596-18f4-45a9-a194-49597368e655"

/* Services */
BLEService DIS("180A");                            // Device Information Service
BLEService NunchukService(NUNCHUCK_SERVICE_UUID);  //generated Nunchuk primary service

/* Characterisitics */
// Device Information to be added to DIS per Spec.
BLEStringCharacteristic ManufacturerNameString("2A29", BLERead, sizeof(MANUFACTURER_NAME_STRING));
BLEStringCharacteristic ModelNumberString("2A24", BLERead, sizeof(MODEL_NUMBER_STRING));
BLEStringCharacteristic SerialNumberString("2A25", BLERead, sizeof(SERIAL_NUMBER_STRING));
BLEStringCharacteristic HardwareRevisionString("2A27", BLERead, sizeof(HARDWARE_REV_STRING));
BLEStringCharacteristic SoftwareRevisionString("2A28", BLERead, sizeof(SOFTWARE_REV_STRING));

// Nunchuk Charactersitics
BLEByteCharacteristic ButtonZ(Z_BUTTON_UUID, BLERead | BLENotify);       // Z button (uint8_t)
BLEByteCharacteristic ButtonC(C_BUTTON_UUID, BLERead | BLENotify);       // C button (uint8_t)
BLEByteCharacteristic JoystickX(JOYSTICK_X_UUID, BLERead | BLENotify);  // Joystick X (uint16_t)
BLEByteCharacteristic JoystickY(JOYSTICK_Y_UUID, BLERead | BLENotify);  // Joystick Y (uint16_t)
BLEByteCharacteristic AccX(ACC_X_UUID, BLERead | BLENotify);            // Acceleration X (uint16_t)
BLEByteCharacteristic AccY(ACC_Y_UUID, BLERead | BLENotify);            // Acceleration Y (uint16_t)
BLEByteCharacteristic AccZ(ACC_Z_UUID, BLERead | BLENotify);            // Acceleration Z (uint16_t)
BLEByteCharacteristic Pitch(PITCH_UUID, BLERead | BLENotify);             // Pitch (float)
BLEByteCharacteristic Roll(ROLL_UUID, BLERead | BLENotify);               // Roll (float)

// Initalize nunchuk object
Accessory nunchuk;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
  nunchuk.begin();  // Intialize controller

  if (nunchuk.type == Unknown) {
    nunchuk.type == NUNCHUCK;
  }

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }
  // add the characterisitics to the DIS service
  DIS.addCharacteristic(ManufacturerNameString);
  DIS.addCharacteristic(ModelNumberString);
  DIS.addCharacteristic(SerialNumberString);
  DIS.addCharacteristic(HardwareRevisionString);
  DIS.addCharacteristic(SoftwareRevisionString);

  // add the characteristics to the Nunchuk service
  NunchukService.addCharacteristic(ButtonZ);
  NunchukService.addCharacteristic(ButtonC);
  NunchukService.addCharacteristic(JoystickX);
  NunchukService.addCharacteristic(JoystickY);
  NunchukService.addCharacteristic(AccX);
  NunchukService.addCharacteristic(AccY);
  NunchukService.addCharacteristic(AccZ);
  NunchukService.addCharacteristic(Pitch);
  NunchukService.addCharacteristic(Roll);

  // write device information
  ManufacturerNameString.writeValue(MANUFACTURER_NAME_STRING);
  ModelNumberString.writeValue(MODEL_NUMBER_STRING);
  SerialNumberString.writeValue(SERIAL_NUMBER_STRING);
  HardwareRevisionString.writeValue(HARDWARE_REV_STRING);
  SoftwareRevisionString.writeValue(SOFTWARE_REV_STRING);

  BLE.setDeviceName("Nunchuk");    // set the device name
  BLE.setLocalName("Nunchuk");     // set the local name peripheral advertises
  BLE.setAdvertisedService(DIS);   // set the UUID for the peripheral advertises
  BLE.addService(NunchukService);  //Add Service

  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    // print the central's MAC address:
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // Loop until disconnected
    while (central.connected()) {
      if (nunchuk.readData()) { // Read Inputs and update maps
        if (nunchuk.values[10] != ButtonZ.value()) {
          ButtonZ.writeValue(nunchuk.values[10]);
        }
        if (nunchuk.values[11] != ButtonC.value()) {
          ButtonC.writeValue(nunchuk.values[11]);
        }
        if (nunchuk.values[0] != JoystickX.value()) {
          JoystickX.writeValue(nunchuk.values[0]);
        }
        if (nunchuk.values[1] != JoystickY.value()) {
          JoystickY.writeValue(nunchuk.values[1]);
        }
        if (nunchuk.values[4] != AccX.value()) {
          AccX.writeValue(nunchuk.values[4]);
        }
        if (nunchuk.values[5] != AccY.value()) {
          AccY.writeValue(nunchuk.values[5]);
        }
        if (nunchuk.values[6] != AccZ.value()) {
          AccZ.writeValue(nunchuk.values[6]);
        }
        if (nunchuk.values[3] != Pitch.value()) {
          Pitch.writeValue(nunchuk.values[3]);
        }
        if (nunchuk.values[2] != Roll.value()) {
          Roll.writeValue(nunchuk.values[2]);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
