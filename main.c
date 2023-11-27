#include <stdio.h>
#include "pico/stdlib.h"

#include "bluetooth_classic_keyboard.h"
//#include "ble_keyboard.h"

int main(void){
    stdio_init_all();

    bluetooth_classic_keyboard_start();
    //ble_keyboard_start();
}