cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(picow-bluetooth-numpad)

pico_sdk_init()

add_executable(picow-bluetooth-numpad main.c bluetooth_classic_keyboard.c ble_keyboard.c)

target_link_libraries(picow-bluetooth-numpad pico_stdlib pico_btstack_ble pico_btstack_cyw43 pico_btstack_classic pico_cyw43_arch_none)
target_include_directories(picow-bluetooth-numpad PRIVATE ${CMAKE_CURRENT_LIST_DIR})
pico_btstack_make_gatt_header(picow-bluetooth-numpad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/picow-bluetooth-numpad.gatt)

pico_enable_stdio_usb(picow-bluetooth-numpad 1)
pico_enable_stdio_uart(picow-bluetooth-numpad 0)

pico_add_extra_outputs(picow-bluetooth-numpad)