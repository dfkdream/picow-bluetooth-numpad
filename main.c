#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/flash.h"

#include "bluetooth_classic_keyboard.h"
//#include "ble_keyboard.h"

#include "scancodes.h"

void core1_main(void){
    if (!flash_safe_execute_core_init()){
        printf("flash_safe_execute_core_init failed");
        return;
    }

    while(1){
        int keycodes[6] = {KEY_KP_1, };
        send_keys(0, keycodes);
        sleep_ms(1);
        keycodes[0] = 0;
        send_keys(0, keycodes);
        sleep_ms(1000);
    }
}

int main(void){
    stdio_init_all();

    multicore_launch_core1(core1_main);

    bluetooth_classic_keyboard_start();
    //ble_keyboard_start();
}