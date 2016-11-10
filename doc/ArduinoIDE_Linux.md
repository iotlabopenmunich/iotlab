
# IoT-Lab: Arduino-IDE installation instructions for Linux (Debian)

<!-- MDTOC maxdepth:6 firsth1:2 numbering:0 flatten:0 bullets:1 updateOnSave:1 -->

- [NodeMCU usb-to-serial driver](#nodemcu-usb-to-serial-driver)   
- [Arduino IDE](#arduino-ide)   

<!-- /MDTOC -->

## NodeMCU usb-to-serial driver
The NodeMCU boards we are going to use have a *CP210x* usb-to-serial bridge. Make sure you have the drivers available (should be the case by default). You can check that the driver is available with with the following command:
```
$ /sbin/modinfo -d cp210x
Silicon Labs CP210x RS232 serial adaptor driver
```

## Arduino IDE
**Important:** Install newest IDE Version from the Arduino.cc site. Do not use the Debian provided package, since it is outdated and has no board manager.
```
$ wget https://downloads.arduino.cc/arduino-1.6.12-linux64.tar.xz
$ xz -dc arduino-1.6.12-linux64.tar.xz | (cd /opt; sudo tar xvf -)
$ /opt/arduino-1.16.12/arduino &
```
Note: of course you can install the Arduino-IDE to a different directory, where no admin/sudo rights are
required for installation.

Now we are ready to install the ESP8266 support as well as the required libraries.

---
Next: [configuration of the Arduino-IDE for use with the ESP8266](ArduinoIDE_ESP8266_configuration.md)
