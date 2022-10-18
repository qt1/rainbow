#pragma once

#include <cstdint>
#include <string>
#include "Arduino.h"
using std::string;

// https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf
class HX711_driver {
    uint8_t ck = 100, dat = 100;

  public:
    uint32_t data = 0;
    int timestamp = 0;
    int dt = 0;
    int counter = 0;
    string name;

    HX711_driver(string name) : name(name) {}

    void begin(uint8_t ck, uint8_t dat) {
        this->ck = ck;
        this->dat = dat;
        pinMode(ck, OUTPUT);
        pinMode(dat, INPUT_PULLUP);
    }

    // read data synchronous 1us per clock
    void read_data(int extra_clocks = 1) {
        uint32_t d = 0;
        delayMicroseconds(1);
        for (int i = 0; i < 24; i++) {
            digitalWrite(ck, 1);
            delayMicroseconds(1);
            d = (d << 1) | digitalRead(dat);
            digitalWrite(ck, 0);
            delayMicroseconds(1);
        }
        for (int i = 0; i < extra_clocks; i++) {
            digitalWrite(ck, 1);
            delayMicroseconds(1);
            digitalWrite(ck, 0);
            delayMicroseconds(1);
        }
        data = d;
        counter++;
        int now = millis();
        dt = now - timestamp;
        timestamp = now;
    }

    bool tick() {
        // loop tick: samples for data ready and reads if available
        // return true if data was read
        if (digitalRead(dat) == 0) {
            read_data();
            return true;
        } else
            return false;
    }

    void print() const {
       Serial.printf("\"HX711_%s\": {\"data\":%d}", name.c_str(), data);
    }
};
