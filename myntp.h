#ifndef MYNTP_H
#define MYNTP_H

#include <Time.h>
#ifdef ESP8266
//#include <simpleDSTadjust.h>
#endif

class MYNTP {
  private:
    time_t this_second = 0;
    time_t last_second = 0;
    int dst = 3600;
  public:
    tm * timeInfo() {
      time_t now;
      struct tm * timeinfo;
      time(&now);
      return localtime(&now);
    }
    char * fTime() { return fTime(timeInfo()); }
    char * fTime(tm * timeinfo) {
      char buf[10];
      sprintf(buf, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      return &buf[0];
    }
    char * fDate() { return fDate(timeInfo()); }
    char * fDate(tm * timeinfo) {
      char buf[10];
      sprintf(buf, "%02d-%02d-%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
      return &buf[0];
    }
    void begin() {
      configTime(7200, 3600, "time.nist.gov", "time.windows.com", "de.pool.ntp.org");
      while (!this_second) {
        time(&this_second);
        Serial.print("-");
        delay(100);
      }
      Serial.println();
      Serial.printf("Now it is %s\n", fTime());
      Serial.printf("Today is  %s\n", fDate());
    }
} myntp;

#endif
