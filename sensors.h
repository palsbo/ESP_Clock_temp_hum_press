#ifndef SENSORS_H
#define SENSORS_H
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_Si7021.h"

Adafruit_BMP280 bme; // I2C
Adafruit_Si7021 pSensor = Adafruit_Si7021();

class SENSORS {
    typedef void (*onsensortype) (char * value);
  private:
    char * t_buf;
    char * h_buf;
    char * p_buf;
    unsigned long timer;
    onsensortype ontemperature = [](char * value) {};
    onsensortype onhumidity = [](char * value) {};
    onsensortype onpressure = [](char * value) {};
    float Round(float val, int digs);
  public:
    void onTemp(onsensortype cb);
    void onHum(onsensortype cb);
    void onPress(onsensortype cb);
    void begin(char * tbuf, char * hbuf, char * pbuf);
    void loop();
};

void SENSORS::begin(char * tbuf, char * hbuf, char * pbuf) {
  t_buf = tbuf;
  h_buf = hbuf;
  p_buf = pbuf;
  if (!bme.begin(0x76)) {
    Serial.println("Did not find BMP280 sensor!");
    //while (1);
  }
  if (!pSensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    //while (true);
  }
}

void SENSORS::onTemp(onsensortype cb) {
  ontemperature = cb;
}

void SENSORS::onHum(onsensortype cb) {
  onhumidity = cb;
}

void SENSORS::onPress(onsensortype cb) {
  onpressure = cb;
}

void SENSORS::loop() {
  if (millis() < timer) return;
  static float temp, oldtemp;
  static float hum, oldhum;
  static float pres, oldpres;
  timer = millis() + 5000;
  temp = Round(bme.readTemperature(), 1); //  change - read sensor;
  hum = Round(pSensor.readHumidity(), 0);             //  change - read sensor;
  pres = Round(bme.readPressure()/100.0,0);          //  change - read sensor;
  if (temp != oldtemp) sprintf(t_buf, "%.1f", temp), ontemperature(t_buf), oldtemp = temp;
  if (hum != oldhum) sprintf(h_buf, "%d", (int)hum), onhumidity(h_buf), oldhum = hum;
  if (pres != oldpres) sprintf(p_buf, "%d", (int)pres), onpressure(p_buf), oldpres = pres;
}

float SENSORS::Round(float val, int digs) {
  return round(val * (float)pow(10, digs)) / pow(10, digs);
}

#endif
