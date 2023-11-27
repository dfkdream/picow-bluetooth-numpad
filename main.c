#include <stdio.h>
#include "pico/stdlib.h"

#include "ble_keyboard.h"


int main(void){
    stdio_init_all();

    ble_keyboard_start();
}