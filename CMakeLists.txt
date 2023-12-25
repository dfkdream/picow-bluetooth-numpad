cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

set(USE_BLE false)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(picow-bluetooth-numpad)

pico_sdk_init()

if (USE_BLE)
    add_executable(picow-bluetooth-numpad
            main.c
            key_matrix/key_matrix.c
            keyboard/ble.c
            )

    target_link_libraries(picow-bluetooth-numpad
            pico_stdlib
            pico_btstack_ble
            pico_btstack_cyw43
            pico_cyw43_arch_none
            )

    pico_btstack_make_gatt_header(picow-bluetooth-numpad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/keyboard/picow-bluetooth-numpad.gatt)
else()
    add_executable(picow-bluetooth-numpad
            main.c
            key_matrix/key_matrix.c
            keyboard/bluetooth_classic.c
            )

    target_link_libraries(picow-bluetooth-numpad
            pico_stdlib
            pico_btstack_cyw43
            pico_btstack_classic
            pico_cyw43_arch_none
            )
endif()


target_include_directories(picow-bluetooth-numpad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/keyboard)
target_include_directories(picow-bluetooth-numpad PRIVATE ${CMAKE_CURRENT_LIST_DIR}/key_matrix)

pico_enable_stdio_usb(picow-bluetooth-numpad 1)
pico_enable_stdio_uart(picow-bluetooth-numpad 0)

pico_add_extra_outputs(picow-bluetooth-numpad)