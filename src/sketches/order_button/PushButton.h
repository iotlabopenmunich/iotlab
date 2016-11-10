/*
 * IoT-Lab: simple button library with debouncing and optional
 * pull-up functionality.
 * sample usage:
 *   void onButtonStateChanged(bool state) {
 *      if (state) {
 *         // button is pressed.
 *      }
 *   }
 *   PushButton button(D5, onButtonStateChanged);
 *   ....
 *   void loop() {
 *      button.loop();
 *   }
 */
#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

typedef void (*PushButtonCallback)(bool);

class PushButton {
private:
    static const int DEBOUNCE_TIME = 10;
    PushButtonCallback callback;
    uint8_t     pin;
    uint32_t    lastDebounceTime;
    bool        state;
    bool        lastState;
    bool        pullup;

public:
    static const bool PULLUP = true;

  /*
   * constructor
   * _pin - gpio pin button is connected to
   * _callback - optional callback to call on state changes
   * _pullup - if true, then pin will be automatically pulled up
   */
    PushButton( uint8_t _pin, 
                PushButtonCallback _callback = nullptr, 
                bool _pullup = false) : pin(_pin), callback(_callback), 
                    pullup(_pullup), state(false) {
        lastDebounceTime = millis();
        if (pullup) {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    /*
     * read a debounced button value
     */
    bool loop(void) {

        bool newState = (digitalRead(pin) != 0);
        if (pullup) {
            newState = !newState;
        }

        if (newState != lastState) {
            lastDebounceTime = millis();
        }
       
        // for DEBOUNCE_TIME ms there was no new change -> take curr reading
        if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
            if (state != newState) {
                state = newState;
                if (callback) {
                    callback(state);
                }
            }
        }
        lastState = newState;
        return state;
    }

    // return status of button
    bool isPressed(void) const {
        return state;
    }

};

#endif

