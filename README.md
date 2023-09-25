# BLE-Nunchuk
This project turns a Wii/Wii-U Nunchuk remote into a simple BLE periphieral with the use of the Arduino Nano ESP32.

## Required Materials
- Nintendo Nunchuk (https://www.nintendo.com/store/products/nunchuk-black-77944/ "@embed")
- Arduino Nano ESP32 (https://store.arduino.cc/products/nano-esp32 "@embed")

## Setup
 This project uses the Arduino header file [Nunchuk.h] (https://github.com/infusion/Fritzing/blob/master/Nunchuk/Nunchuk.h) by Fritzing.  For a better undersantding on what Fritzing is doing I encourage you to look into the documentation compiled by Wii Brew [here] (http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck).  Fritzing, also, compiled a wonderful usage guide for his header file [here] (https://www.xarg.org/2016/12/using-a-wii-nunchuk-with-arduino/) which I will do my best to summarize here.

 NOTE: Different manufactureres of the Nunchuk use different wiring colors for the 4 relevant connections.  Due to this, it is important to base wiring off of connections to the propietary male connector instead of wire color.  I note this becuase googled guides to the wiring guides can be missleading.

 1. Remove the male connector and note how the wires are connected to the pin out comparing to the below picture <attach-image-here>
 1. The only wires that matter are connected to pins 1, 3, 4, 6.
    1. Pin 1 = SDA
    1. Pin 3 = 3.3 v
    1. Pin 4 = GND
    1. Pin 6 = SCL
 1. Using the pinout of the Arduino Nano ESP32 [here] (https://docs.arduino.cc/hardware/nano-esp32) there is no explicit pins labeled for i2c SDA & SCl pins.  However, the [datasheet] (https://docs.arduino.cc/resources/datasheets/ABX00083-datasheet.pdf) says they can be found as follows:
    1. SDA = A4
    1. SCL = A5
 
 This is all the wiring required to get the project running!

 Finally, to compile this project the ArduinoBLE Library by Arduino must also installed.  This can easily be added using the Library Manager built into Arduino IDE.  I strongly suggest reading this [documentation] (https://www.arduino.cc/reference/en/libraries/arduinoble/) as an introduction into Bluetooth Low Energy (BLE).

 ## Contributing
 I am just learning BLE and spent some time reading through the BLE Assigned Numbers docuemntation but still am unsure if I am properly using the predefined UUID's for characteristics.  If anyone can help my undersatnding on the topic I would greatly welcome the input.

 ## License

 
