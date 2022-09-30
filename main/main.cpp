/* The true ESP32 chip ID is essentially its MAC address.
This sketch provides an alternate chip ID that matches
the output of the ESP.getChipId() function on ESP8266
(i.e. a 32-bit integer matching the last 3 bytes of
the MAC address. This is less unique than the
MAC address chip ID, but is helpful when you need
an identifier that can be no more than a 32-bit integer
(like for switch...case).

created 2020-06-07 by cweinhofer
with help from Cicicok */

#include <cstdint>

#include "Arduino.h"
#include "Wire.h"

#include "HX711.h"
#include "MPU6050.h"

uint32_t chipId = 0;
#define LED 2

HX711_driver wheigt0("0");
HX711_driver wheigt1("1");
MPU6050 mpu0(Wire, 0);
MPU6050 mpu1(Wire1, 1);

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

    wheigt0.begin(14, 27);

    wheigt1.begin(18, 19);

    bool wire_begin_ok = Wire.begin(21, 22, 100000);
    if (!wire_begin_ok)
        Serial.print("!!!! Wire begin error!"); // TBD: disable

    bool wire1_begin_ok = Wire1.begin(26, 25, 100000);
    if (!wire1_begin_ok)
        Serial.print("!!!! Wire1 begin error!"); // TBD: disable


    // Wire.setTimeout(3000 /* us */);//, true /* reset_on_timeout */); stream-> _timeout, Wire ->  _timeOutMillis
    mpu0.begin();
    mpu1.begin();
    //delayMicroseconds(1000);
    // for (int i = 0; i < 17; i = i + 8) {
    //     chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    // }

    // Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
    // Serial.printf("This chip has %d cores\n", ESP.getChipCores());
    // Serial.print("Chip ID: ");
    // Serial.println(chipId);

    //   delay(3000);
}

void loop() {
   try {
        //  if (wheigt0.tick()) {
        //     Serial.printf("wheigt0: %d, dt=%d, t=%d, counter=%d\n", wheigt0.data, wheigt0.dt, wheigt0.timestamp, wheigt0.counter);
        //     Serial.print(", ");
        // }

        // if (wheigt1.tick()) {
        //     Serial.printf("wheigt1: %d, dt=%d, t=%d, counter=%d\n", wheigt1.data, wheigt1.dt, wheigt1.timestamp, wheigt1.counter);
        //     Serial.print(", ");
        // }
        wheigt0.tick(); // read if available
        wheigt1.tick();

        wheigt0.print(); // possibly repeat if not read in this loop
		Serial.print(", ");

        wheigt1.print();
		Serial.print(", ");

        mpu0.read();
        mpu0.print();

		Serial.print(", ");
        mpu1.read();
        mpu1.print();

		Serial.println();
    } catch (exception &e) {
        log_e("loop exception: %s", e.what());
    }
    delay(45);
}
