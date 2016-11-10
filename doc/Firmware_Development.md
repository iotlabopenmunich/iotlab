## IoT-Lab: the firmware

Now that the [hardware is set up](Hardware_Setup.md) and you have successfully run the [Hello, world!](HelloWorld.md) sketch, it is time to bring the real firmware on the device, to make it do something useful.

## Instructions
 1. Copy the sketch folder from this repository `src/sketches/order_button` to your sketchbook location of your Arduino IDE (usually located in the `Arduino/` folder in your homedir. Check `File` > `Preferences` > `Sketchbook location` in the Arduino IDE). Your Arduiono folder should now look like the following:
```
   +--Arduino                   Arduino sketchbook location
      +--libraries              Library folder
         +--PubSubClient        The MQTT client libraray
      +--order_button           sketch folder for the order button
        +--order_button.ino     Main file
        +--config.h-dist        config.h template
        +--PushButton.h         source files ...
        +--DigitalOutput.h      
        +--...
```
 2. copy the file [`config.h-dist`](../src/sketches/order_button/config.h-dist) to a new file named `config.h`
 3. browse through the source code of the [`order_button.ino`](../src/sketches/order_button/order_button.ino) and `config.h` files and fix the marked parts (i.e. parts marked with `/*FIXME*/`)
 4. compile and upload the sketch. Carefully watch for errors.

 Congratulations, you have successfully compiled and put the firmware on the device! Now it is time for some testing.

---
Next: [Testing the device](Testing.md)
