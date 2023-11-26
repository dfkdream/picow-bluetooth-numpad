cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(picow-bluetooth-numpad)

pico_sdk_init()

add_executable(picow-bluetooth-numpad main.c)

target_link_libraries(picow-bluetooth-numpad pico_stdlib)

pico_enable_stdio_usb(picow-bluetooth-numpad 1)
pico_enable_stdio_uart(picow-bluetooth-numpad 0)

pico_add_extra_outputs(picow-bluetooth-numpad)