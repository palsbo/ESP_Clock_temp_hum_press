#ifndef SCREEN_H
#define SCREEN_H
#include <Time.h>

#ifdef ESP8266
#define SDA 4
#define SCL 5
#else
#define SDA 5
#define SCL 4
#endif

#ifdef _SSD1306
#include "SSD1306.h" // legacy include: `#include "SSD1306.h"`
SSD1306  display(0x3c, SDA, SCL);
#else
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SH1106.h" // alias for `#include "SSD1306Wire.h"`
SH1106  display(0x3c, SDA, SCL);
#endif
#include "OLEDDisplayUi.h"
#include <TimeLib.h>

#ifndef OLED_WIDTH
#define OLED_WIDTH 128
#endif
#ifndef OLED_HEIGHT
#define OLED_HEIGHT 64
#endif
#ifndef OLED_TOP
#define OLED_TOP 0
#endif
#ifndef OLED_LEFT
#define OLED_LEFT 0
#endif
#if OLED_HEIGHT > 50
#define LARGE_TXT ArialMT_Plain_24
#define SMALL_TXT ArialMT_Plain_16
#else
#define LARGE_TXT ArialMT_Plain_16
#define SMALL_TXT ArialMT_Plain_10
#endif

OLEDDisplayUi ui ( &display );

typedef void (*timeleftdef) ();

class SCREEN {
  private:
    char * t_buf;
    char * h_buf;
    char * p_buf;
  public:
    void begin(char * tbuf, char * hbuf, char * pbuf);
    void loop();
    void onTimeLeft(timeleftdef func);
    void onTimeUpdate(timeleftdef func);
};

#endif
int screenW = OLED_WIDTH;
int screenH = OLED_HEIGHT;
int clockCenterX = (screenW / 2) + OLED_LEFT;
int clockCenterY = (screenH / 2) + OLED_TOP;
int clockRadius = screenH / 2;

char * ct_buf;
char * ch_buf;
char * cp_buf;

timeleftdef _ontimeleft = []() {};
timeleftdef _ontimeupdate = []() {};

String twoDigits(int digits) {
  if (digits > 9) return String(digits);
  return '0' + String(digits);
}

char * fTime(tm * timeinfo) {
  char buf[10];
  sprintf(buf, "%02d:%02d:%02d  \n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  return &buf[0];
}
char * fDate(tm * timeinfo) {
  char buf[10];
  sprintf(buf, "%02d-%02d-%02d  \n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
  return &buf[0];
}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
  for ( int z = 0; z < 360; z = z + 30 ) {
    float angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display->drawLine( x2 + x , y2 + y , x3 + x , y3 + y);
  }
  float angle = timeinfo->tm_sec * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  angle = timeinfo->tm_min * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  angle = timeinfo->tm_hour * 30 + int( ( timeinfo->tm_min / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
}

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(LARGE_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 4, (twoDigits(timeinfo->tm_hour) + ":" + twoDigits(timeinfo->tm_min) + ":" + twoDigits(timeinfo->tm_sec)) );
  display->setFont(SMALL_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 22, (twoDigits(timeinfo->tm_mday) + "-" + twoDigits(timeinfo->tm_mon + 1) + "-" + twoDigits(timeinfo->tm_year - 100)) );
}

void temperatureFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(LARGE_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 4, String(ct_buf) + "°C" );
  display->setFont(SMALL_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 22, "Temperature" );
}

void humidityFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(LARGE_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 4, String(ch_buf) + " %" );
  display->setFont(SMALL_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 22, "Humidity" );
}

void pressureFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(LARGE_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 4, String(cp_buf) + " mb" );
  display->setFont(SMALL_TXT);
  display->drawString(clockCenterX + x , clockCenterY + y - 22, "Pressure" );
}
FrameCallback frames[6] = { analogClockFrame, digitalClockFrame, temperatureFrame, humidityFrame, pressureFrame };

int frameCount = 5;

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) { }

OverlayCallback overlays[] = { clockOverlay };

void screenbegin() {
  ui.setTargetFPS(50);
  //ui.setActiveSymbol(activeSymbol);
  //ui.setInactiveSymbol(inactiveSymbol);
  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(RIGHT);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, sizeof(overlays) / sizeof(overlays[0]));
  ui.init();
  display.flipScreenVertically();
}

void screenloop() {
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    uint32_t ms = millis();
    _ontimeleft();
    remainingTimeBudget -= (millis() - ms);
    if (remainingTimeBudget > 0) delay(remainingTimeBudget);
  }
}
void SCREEN::begin(char * tbuf, char * hbuf, char * pbuf) {
  ct_buf = tbuf;
  ch_buf = hbuf;
  cp_buf = pbuf;
  screenbegin();
}

void SCREEN::loop() {
  screenloop();
}
void SCREEN::onTimeLeft(timeleftdef func) {
  _ontimeleft = func;
}
void SCREEN::onTimeUpdate(timeleftdef func) {
  _ontimeupdate = func;
}


