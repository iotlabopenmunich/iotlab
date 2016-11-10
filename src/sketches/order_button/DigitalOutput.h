/*
 * IoT-Lab: simple abstraction of a digital output
 * sample usage:
 *   DigitalOutput led(D1);
 *   led.on();
 *   led.off();
 *   led.set(true);
 */
#ifndef __DIGITAL_OUTPUT_H__
#define __DIGITAL_OUTPUT_H__

class DigitalOutput {
private:
    uint8_t pin;
    bool inverted;

public:
    static const bool INVERTED = true;
    
    /**
     * constructor
     * _pin - pin to which the digital output is connected
     * _inverted - if true then all values written/read will be inverted
     */
    DigitalOutput(uint8_t _pin, bool _inverted=false) 
        : pin(_pin), inverted(_inverted) { 
      pinMode(pin, OUTPUT);
    }

    /*
     * set output to given value
     */
    void set(bool value) {
        value = inverted ^ value;
        digitalWrite(pin, value ? HIGH : LOW);
    }

    void on() {
        set(true);
    }

    void off() {
        set(false);
    }
};

#endif

