#idf_component_register(SRCS "./app_main.cpp"                    INCLUDE_DIRS ".")

file(GLOB main_files  "*.cpp" "*.c")

set(requires spi_flash mbedtls mdns esp_adc_cal wifi_provisioning nghttp)
set(priv_requires fatfs nvs_flash app_update spiffs bootloader_support openssl bt esp_ipc esp_hid)

idf_component_register(
    SRCS  ${main_files}
#    REQUIRES  arduino ArduinoJson ibn_esp32_gen_lib ${requires}
#    PRIV_REQUIRES ${priv_requires}
)
#    INCLUDE_DIRS "." "../components/arduino"
#    "../components/ibn_esp32_gen_lib/include/"
#    REQUIRES  arduino ArduinoJson ibn_esp32_gen_lib mbedtls)

#works but triggers compilation errors 
#target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++17)
