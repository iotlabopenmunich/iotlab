/*
 * IoT-Lab: firmware for our order button device
 * 
 * Functional description:
 *   - on start device connects to WiFi and to the configured MQTT broker
 *   - when button is pressed, LED is flashed for 0.25 seconds and order 
 *     message is published
 *   - when status message from backend is received, LED is flashed 
 *     for 2 seconds to give the user feedback that the order is processed.
 * -------------------------------------------------------------------------
 * This is DEMO source code, demonstrating some basics and principles
 * used in IoT devices (e.g. MQTT, interfacing with hardware input and 
 * outputs). In a real world application we would likely use a REST 
 * interface to the backend service and also need some kind of personal-
 * isation procedure to initialize the device. Also note that for reasons
 * of simplicity that this demo does not implement any security measures 
 * like encryption, authentication or authorization.
 * -------------------------------------------------------------------------
 */
#include <ESP8266WiFi.h>
#include <pins_arduino.h>
#include <PubSubClient.h>

#include "PushButton.h"
#include "DigitalOutput.h"
// don't forget to copy and adapt config.h-dist
#include "config.h"

// The feedback LED is connected to pin D6
DigitalOutput g_ledFeedback(D6);

// the WiFi status led is connected to the builtin LED. note that the builtin 
// LED is inverted.
DigitalOutput g_ledWifi(BUILTIN_LED, DigitalOutput::INVERTED);

// this function is called when the push button is pressed
extern void onButtonChanged(bool);
// connect the button to pin D5
PushButton g_button(D5, onButtonChanged, PushButton::PULLUP);

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient g_wifiClient;

// MQTT client instance
PubSubClient g_mqtt(g_wifiClient);

/*
 * let an LED blink for a certain number of time. One iteration turns the LED 
 * on and off. This simple implementation is blocking and simply uses 
 * delay() to let pause between flashes. 
 * In a more sophisticated version we would use use a timer which would be 
 * triggered from the event loop, because the blocking approach may lead i
 * to problems. But for demonstration purposes, this method is fair enough.
 */
 void blinkLed(DigitalOutput *pLed, int numberOfTimes, int delayMs) {
  bool state = true;
  numberOfTimes *= 2;
  while(numberOfTimes-- > 0) {
    pLed->set(state);
    // dont delay on last iteration
    if (numberOfTimes == 0) {
      return;
    }
    delay(delayMs);
    state = !state;
  }
 }
 
/*
 * this callback is called when the state of the button changes. 
 * We then give a short feedback using the LED and send oout the 
 * ordering message.
 */
void onButtonChanged(bool state) {
  // button is pressed
  if (state) {
    Serial.println("button pressed. publishing message to " MQTT_TOPIC "/" DEVICE_ID "/order");
    // let the feedback led flash for 250ms publish the order message
    blinkLed(&g_ledFeedback, 1, 250);
    // publish the order message
    g_mqtt.publish(MQTT_TOPIC "/" DEVICE_ID "/order", "1");
  }
}

/*
 * this callback is invoked when a mqtt message is received on our 
 * topic. This happens when the backend sends us an status update.
 *  - status == "ok": turn the feedback led on for 2 seconds.
 *  - status == "nok": turn the feedback led on for 0.5 seconds.
 */
void onMqttMessageReceived(const char* topic, const byte* payload, unsigned int length) {
  Serial.print("mqtt: message arrived on topic " + String(topic) + String(": ["));
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("]");

  if (strncmp((const char*)payload, "ok", length) == 0) {
    blinkLed(&g_ledFeedback, 1, 2000);
  } else 
  if (strncmp((const char*)payload, "nok", length) == 0) {
    // we received an "not ok" status - blink led quickly to notify user of problems
    blinkLed(&g_ledFeedback, 5, 100);
  }
}

/*
 * establish connection to MQTT broker and subscribe topic to receive
 * feedback from the backend
 */
void mqttReconnect() {
  // Loop until we're reconnected
  while (!g_mqtt.connected()) {
    Serial.print("mqtt: attempting connection to " MQTT_HOST);
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (g_mqtt.connect(clientId.c_str())) {
      // subscribe to topic to receive status messages from the backend
      Serial.println(" ok, subscribing to " MQTT_TOPIC "/" DEVICE_ID "/status");
      g_mqtt.subscribe(MQTT_TOPIC "/" DEVICE_ID "/status");
    } else {
      Serial.println(" failed, rc=" + String(g_mqtt.state()) + ", retry in 5s");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
 * one-time setup after the device started.
 * initialize input and output pins and wifi.
 */
void setup() {

  Serial.begin(9600);

  // turn LEDs off
  g_ledWifi.off();
  g_ledFeedback.off();
    
  // try to connect to WiFi
  Serial.print("wifi connecting to " WIFI_SSID); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int wifiCounter = WIFI_MAX_TRIES;
  bool ledState = false;
  
  while (WiFi.status() != WL_CONNECTED && wifiCounter > 0) {
    // let LED_WIFI_ON led blink during connection setup
    g_ledWifi.set(ledState);
    ledState = !ledState;
    delay(WIFI_DELAY_TRY);    
    wifiCounter--;
    Serial.print(".");
  }

  g_ledWifi.off();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected with ip " + WiFi.localIP().toString());
  } else {
    Serial.println("wifi could not be started.");
    // force WDT to restart
    while(1);
  }

  // prepare mqtt connection
  g_mqtt.setServer(MQTT_HOST, MQTT_PORT);
  g_mqtt.setCallback(onMqttMessageReceived);
}

/*
 * main loop - handles mqtt and push button events
 */
void loop() {
  // make sure mqtt connection is up and process messages
  if (!g_mqtt.connected()) {
    mqttReconnect();
  }
  g_mqtt.loop();

  // process button presses
  g_button.loop();
}

