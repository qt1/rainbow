extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
}

#include "Arduino.h"

extern "C" {
#include "esp_log.h"
}

#define APP_TX_TAG "app_main"

#include "sdkconfig.h"
#include "xtensa/core-macros.h"

#include <algorithm>
#include <stddef.h>
#include <vector>

#include <array>

using namespace std;

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

extern void setup();
extern void loop();

void loopTask(void *pvParameters) {
    static int counter = 0;

    ESP_LOGI(APP_TX_TAG, "Setup:");
    setup();
    ESP_LOGI(APP_TX_TAG, "Setup done. Entring loop");
    while (1) {
        try {
            // ESP_LOGI(RMT_TX_TAG, "loop %d", counter);
            loop();
            micros(); // update overflow // magic from components/arduino..
            counter++;
        } catch (const std::exception &ex) {
            ESP_LOGI(APP_TX_TAG, "Unhandled exception in loop %d: %s", counter, ex.what());
        }
    }
}

// TBD:
//  first check for past catastrophic failes.
//  if found try to update version in emergency mode (establish connection to master etc.)

// TBD: common log service

extern "C" void app_main(void *ignore) {
    initArduino(); // initializes NVS if needed
    // TBD: init SPIFFS if needed (takes long time - needs to stop watchdog )
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192 * 3, NULL, 10, NULL, ARDUINO_RUNNING_CORE);

    // vTaskDelete(NULL);
}
