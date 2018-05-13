#ifndef MYAPP_H
#define MYAPP_H

//#define _SSD1306        //  default SH1106
//#define OLED_WIDTH 64   //  default 128
//#define OLED_HEIGHT 48  //  default 64
//#define OLED_TOP 18     //  default 0
//#define OLED_LEFT 32    //  default 0

#include "mqttclient.h"
#include "myntp.h"
#include "screen.h"
#include "sensors.h"

char* mqtt_client = "ESP123"; //String(ESP.getChipId());
char* mqtt_host = MQTT_SERVER;
uint16_t mqtt_port = MQTT_PORT;
char* mqtt_user = MQTT_USER;
char* mqtt_pass = MQTT_PASS;

MQTTCLIENT mqttclient;
SCREEN screen;
SENSORS sensor;

class {
  private:
  public:
    char temperature[20];
    char humidity[20];
    char pressure[20];
    void begin() {
      myntp.begin();
      mqttclient.on("connect", [](char * data) {
        Serial.println("MQTT Connected");
        mqttclient.subscribe("#");
      });
      mqttclient.on("failed", [](char * data) {
        Serial.println("MQTT Failed");
      });
      mqttclient.begin(mqtt_client, mqtt_host, mqtt_port, mqtt_user, mqtt_pass);
      screen.onTimeLeft([]() {
      });
      screen.onTimeUpdate([]() {
      });
      screen.begin(&temperature[0], &humidity[0], &pressure[0]);
      sensor.onTemp([](char * value) {
        Serial.printf("%s Temperature: %4s°C\n", myntp.fTime(),value);
        mqttclient.publish("hundested/ude/temperature/state", value, true);
      });
      sensor.onHum([](char * value) {
        Serial.printf("%s Humidity:    %4s%%\n", myntp.fTime(),value);
        mqttclient.publish("hundested/ude/humidity/state", value, true);
      });
      sensor.onPress([](char * value) {
        Serial.printf("%s Pressure:    %4smb\n", myntp.fTime(),value);
        mqttclient.publish("hundested/ude/pressure/state", value, true);
      });
      sensor.begin(&temperature[0], &humidity[0], &pressure[0]);
    }
    void loop() {
      mqttclient.loop();
      screen.loop();
      sensor.loop();
    }
} myApp;

#endif
