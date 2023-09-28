/*
  This program broadcasts the Nunchuk data so it can be used as a simple BLE controller.

  Board: Arduino Nano ESP32

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  Author: Austin
  https://github.com/gregoryPowell/BLE-Nunchuk
*/

#include <ArduinoBLE.h>
#include <Wire.h>
#include "Nunchuk.h"

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
BLEShortCharacteristic JoystickX(JOYSTICK_X_UUID, BLERead | BLENotify);  // Joystick X (uint16_t)
BLEShortCharacteristic JoystickY(JOYSTICK_Y_UUID, BLERead | BLENotify);  // Joystick Y (uint16_t)
BLEShortCharacteristic AccX(ACC_X_UUID, BLERead | BLENotify);            // Acceleration X (uint16_t)
BLEShortCharacteristic AccY(ACC_Y_UUID, BLERead | BLENotify);            // Acceleration Y (uint16_t)
BLEShortCharacteristic AccZ(ACC_Z_UUID, BLERead | BLENotify);            // Acceleration Z (uint16_t)
BLEIntCharacteristic Pitch(PITCH_UUID, BLERead | BLENotify);             // Pitch (float)
BLEIntCharacteristic Roll(ROLL_UUID, BLERead | BLENotify);               // Roll (float)

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
  nunchuk_init();

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
      if (nunchuk_read()) {
        if (nunchuk_buttonZ() != ButtonZ.value()) {
          ButtonZ.writeValue(nunchuk_buttonZ());
        }
        if (nunchuk_buttonC() != ButtonC.value()) {
          ButtonC.writeValue(nunchuk_buttonC());
        }
        if (nunchuk_joystickX() != JoystickX.value()) {
          JoystickX.writeValue(nunchuk_joystickX());
        }
        if (nunchuk_joystickY() != JoystickY.value()) {
          JoystickY.writeValue(nunchuk_joystickY());
        }
        if (nunchuk_accelX() != AccX.value()) {
          AccX.writeValue(nunchuk_accelX());
        }
        if (nunchuk_accelY() != AccY.value()) {
          AccY.writeValue(nunchuk_accelY());
        }
        if (nunchuk_accelZ() != AccZ.value()) {
          AccZ.writeValue(nunchuk_accelZ());
        }

        int pitch_deg = (int)((nunchuk_pitch() * 180) / PI);
        int roll_deg = (int)((nunchuk_pitch() * 180) / PI);

        if (pitch_deg != Pitch.value()) {
          Pitch.writeValue(pitch_deg);

        }
        if (roll_deg != Roll.value()) {
          Roll.writeValue(roll_deg);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
