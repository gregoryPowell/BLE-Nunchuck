# BLE-Nunchuk
This project turns a Wii/Wii-U Nunchuk remote into a simple BLE periphieral with the use of the Arduino Nano ESP32.

## Required Materials
- [Nintendo Nunchuk](https://www.nintendo.com/store/products/nunchuk-black-77944/)
- [Arduino Nano ESP32](https://store.arduino.cc/products/nano-esp32 "@embed")

## Setup

### Software
To build this project in the Arduino IDE there are 2 extensions that must be installed.

1. [ArduinoBLE](https://www.arduino.cc/reference/en/libraries/arduinoble/) by Arduino
1. [WiiChuck](https://github.com/madhephaestus/WiiChuck) by Kevin Harrington

### Hardware

 NOTE: Different manufactureres of the Nunchuk use different wiring colors for the 4 relevant connections.  Due to this, it is important to base wiring off of connections to the propietary male connector instead of wire color.  I note this becuase googled guides to the wiring guides can be missleading.

 Remove the male connector and note how the wires are connected to the pin out compared to the below picture.  The only wires that matter are connected to pins 1, 3, 4, 6.

 ![Nunchuk Pinout](/imgs/nunchuk-plug-schematic.png)

 * Pin 1 = SDA
 * Pin 3 = 3.3 v
 * Pin 4 = GND
 * Pin 6 = SCL

 Using the Arduino Nano ESP32 [pinout](https://docs.arduino.cc/hardware/nano-esp32) there is no explicit pins labeled for i2c SDA & SCl pins.  However, the [datasheet](https://docs.arduino.cc/resources/datasheets/ABX00083-datasheet.pdf) says they can be found as follows:

 * SDA = A4
 * SCL = A5
 
 This is all the wiring required to get the project running!

 Finally, to compile this project the ArduinoBLE Library by Arduino must also installed.  This can easily be added using the Library Manager built into Arduino IDE.  I strongly suggest reading this [documentation](https://www.arduino.cc/reference/en/libraries/arduinoble/) as an introduction into Bluetooth Low Energy (BLE).

## BLE Output

To easily test the Nunchuk you can use a generic central cenrtal app.  I recommend LightBlue ([IOS](https://apps.apple.com/us/app/lightblue/id557428110) and [Android](https://play.google.com/store/apps/details?id=com.punchthrough.lightblueexplorer&hl=en_US&gl=US)) Android users can also use [nRF Connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en_US&gl=US), however the IOS expierince on this app is lackluster.

In device discovery the device should be named 'Nunchuk'. Upon connection all below listed UUIDs should be viewable services:

| Service   |      UUID      |
|:----------:|:-------------:|
| Nunchuk | e4257d92-0c9f-4fbc-bcf2-a8361bffcfb3 |

Inside the Nunchuk service are the following relevant characteristics which have read or notify permissions:

| Value   |      UUID      |  Range |
|:----------:|:-------------:|:------:|
| Z Button | 9ea41596-18ec-45a9-a194-49597368e655 | 0 : 255 |
| C Button | 9ea41596-18ed-45a9-a194-49597368e655 | 0 : 255 |
| Joystick X | 9ea41596-18ee-45a9-a194-49597368e655 | 0 - 256 |
| Joystick Y | 9ea41596-18ef-45a9-a194-49597368e655 | 0 - 256 |
| Accelration X | 9ea41596-18f0-45a9-a194-49597368e655 | 0 - 256 |
| Accelration Y | 9ea41596-18f1-45a9-a194-49597368e655 | 0 - 256 |
| Accelration Z | 9ea41596-18f2-45a9-a194-49597368e655 | 0 - 256 |
| Pitch | 9ea41596-18f3-45a9-a194-49597368e655 | 0 - 256 |
| Roll | 9ea41596-18f4-45a9-a194-49597368e655 | 0 - 256 |


 ## Contributing
 I am just learning BLE and spent some time reading through the BLE Assigned Numbers docuemntation but still am unsure if I am properly using the predefined UUID's for characteristics.  If anyone can help my undersatnding on the topic I would greatly welcome the input.

 ## License

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

 ## Refrences
1. [Wii Brew](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck)
1. [Using a Wii Nunchuk with Arduino](https://www.xarg.org/2016/12/using-a-wii-nunchuk-with-arduino/)

 
