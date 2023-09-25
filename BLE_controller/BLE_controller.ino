#include <ArduinoBLE.h>
#include <Wire.h>
#include "Nunchuk.h"

// NOTE: I am just learning how BLE works and am unsure if I am properly using the UUID system for my Services and Characteristics

// UUID generated here: https://www.uuidgenerator.net/
BLEService NunchuckService("e4257d92-0c9f-4fbc-bcf2-a8361bffcfb3");

// UUID for push button = 0x04CC
BLEByteCharacteristic ButtonZ("04CC", BLENotify);  // Z button
BLEByteCharacteristic ButtonC("04CC", BLENotify);  // C button
// UUID for joystick = 0x03C3
BLEShortCharacteristic JoystickX("03C3", BLENotify);
BLEShortCharacteristic JoystickY("03C3", BLENotify);

// UUID for acceleration = 0x2713
BLEShortCharacteristic AccX("2713", BLENotify);
BLEShortCharacteristic AccY("2713", BLENotify);
BLEShortCharacteristic AccZ("2713", BLENotify);

// UUID for plane angle (degree) = 0x2763
BLEFloatCharacteristic Pitch("2763", BLENotify);
BLEFloatCharacteristic Roll("2763", BLENotify);

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

  BLE.setLocalName("Nunchuck");               // set the local name peripheral advertises
  BLE.setAdvertisedService(NunchuckService);  // set the UUID for the peripheral advertises

  // add the characteristics to the service
  NunchuckService.addCharacteristic(ButtonZ);
  NunchuckService.addCharacteristic(ButtonC);
  NunchuckService.addCharacteristic(JoystickX);
  NunchuckService.addCharacteristic(JoystickY);
  NunchuckService.addCharacteristic(AccX);
  NunchuckService.addCharacteristic(AccY);
  NunchuckService.addCharacteristic(AccZ);
  NunchuckService.addCharacteristic(Pitch);
  NunchuckService.addCharacteristic(Roll);

  BLE.addService(NunchuckService);  //Add Service

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
        if (nunchuk_pitch() != Pitch.value()) {
          Pitch.writeValue(nunchuk_pitch());
        }
        if (nunchuk_roll() != Roll.value()) {
          Roll.writeValue(nunchuk_roll());
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
