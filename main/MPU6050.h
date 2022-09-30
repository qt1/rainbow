#pragma once

#include <cstdint>
#include <exception>
using namespace std;

#include "Arduino.h"
#include "Wire.h"

// https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/i2c.html
//  https://www.tutorialspoint.com/esp32_for_iot/interfacing_esp32_with_mpu6050.htm

// A simplified driver for reading acceleration over iic
class MPU6050 {
    uint8_t address = 0x68; // default I2C address of the MPU-6050
    int clock_rate;
    TwoWire &wire;
    int id;

    enum registers : uint8_t {
        GYRO_CFG_ADDR = 0x1B, // Configuration register address off MPU6050
        ACCL_CFG_ADDR = 0x1C, // Configuration register address off MPU6050
        ACCL_XH_ADDR = 0x3B,  // Register Addresses for Accelerometer Data
        ACCL_XL_ADDR = 0x3C,
        ACCL_YH_ADDR = 0x3D,
        ACCL_YL_ADDR = 0x3E,
        ACCL_ZH_ADDR = 0x3F,
        ACCL_ZL_ADDR = 0x40,
        TEMP_H_ADDR = 0x41, // Register Addresses for Thermometer Data
        TEMP_L_ADDR = 0x42,
        GYRO_XH_ADDR = 0x43, // Register Addresses for Gyroscope Data
        GYRO_XL_ADDR = 0x44,
        GYRO_YH_ADDR = 0x45,
        GYRO_YL_ADDR = 0x46,
        GYRO_ZH_ADDR = 0x47,
        GYRO_ZL_ADDR = 0x48,
        PWR_MGMT_1 = 0x6B,
    };

  public:
    int16_t ax, ay, az, temp, GyX, GyY, GyZ;
    int timestamp = 0;
    int dt = 0;
    int counter = 0;
    bool active = false;

    MPU6050(TwoWire &wire, int id) : wire(wire), id(id) {} // wire interface possibly common to several devices

    void begin(uint8_t address = 0x68) {
        try {
            Serial.printf("MPU6050 begin %d\n", address);
            this->address = address;
            // Assume already done! //Wire.begin(21, 22, 100000); // sda, scl, clock speed
            write_reg(PWR_MGMT_1, 0);             // set to zero (wakes up the MPU−6050)
            write_reg(GYRO_CFG_ADDR, (uint8_t)0); // Setup the module: Full Scale Range for Gyroscope
            write_reg(ACCL_CFG_ADDR, (uint8_t)0); // Setup the module: Full Scale Range for Acclerometer
            active = true;
        } catch (exception &e) {
            log_e("Exception MPU6050 <%02x> begin : %s", address, e.what());
        }       
    }

    void write_reg(uint8_t reg, uint8_t val) {
        wire.beginTransmission(address);
        wire.write(reg);
        wire.write(val);
        int rc = wire.endTransmission(true);
        if (rc != 0) {
            Serial.printf("MPU6050 %d write reg[%02x]=%d\n", address, reg, val);
            Serial.printf(" ==>> MPU6050 %d - write  rc=%d\n", address, rc);
            throw runtime_error("MPU6050 " + to_string(address) + " reg[" + to_string(address) + "] = " + to_string(val) + " rc=" + to_string(rc));
        }
    }

    void write(uint8_t val, bool sendStop) {
        wire.beginTransmission(address);
        wire.write(val);
        int rc = wire.endTransmission(sendStop);
        if (rc != 0) {
            Serial.printf("MPU6050 %d write %x2x sendStop=%d\n", address, val, sendStop);
            Serial.printf(" ==>> MPU6050 %d - write  %d\n", address, rc);
            throw runtime_error("MPU6050 " + to_string(address) + " write error " + to_string(val) + " rc=" + to_string(rc));
        }
    }

    // request a total of count registers
    int request(int count, bool sendStop) {
        return wire.requestFrom(address, count, sendStop); // request a total of 8 registers
    }

    uint16_t read16() {
        return (wire.read() << 8) | wire.read();
    }

    void read() { // synchroneous
        if(!active)
            return;

        write(ACCL_XH_ADDR, false);
        int rc = request(8, true); // request a total of 8 registers
        // Serial.printf("  request 6 rc =  %d", rc);

        ax = read16(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        ay = read16(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        az = read16(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        temp = read16(); // 0x41 (TEMP_OUT_H) &  0x42 (TEMP_OUT_L)
 
        // GyX = (wire.read() << 8) | wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        // Serial.printf("  gyx" );
        // GyY = (wire.read() << 8) | wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        // Serial.printf("  gyy" );
        // GyZ = (wire.read() << 8) | wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        // Serial.printf("  gyz" );
        counter++;
        int now = millis();
        dt = now - timestamp;
        timestamp = now;
    }

    void print() const {
        if(active)
            Serial.printf("\"MPU%d\": {\"ax\":%d, \"ay\":%d, \"az\":%d, \"temp\":%d}", id, ax, ay, az, temp);
        else            
            Serial.printf("\"MPU%d\": {\"active\":false}", id);
    }
};
