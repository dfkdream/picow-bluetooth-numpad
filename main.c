#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/flash.h"

#include "key_matrix.h"

#include "keyboard.h"
#include "scancodes.h"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

const int keymap[] = {
        KEY_NUM_LOCK, KEY_KP_SLASH, KEY_KP_ASTERISK, KEY_KP_MINUS,
        KEY_KP_7,     KEY_KP_8,     KEY_KP_9,        KEY_NULL,
        KEY_KP_4,     KEY_KP_5,     KEY_KP_6,        KEY_KP_PLUS,
        KEY_KP_1,     KEY_KP_2,     KEY_KP_3,        KEY_NULL,
        KEY_KP_0,     KEY_KP_0,     KEY_KP_DOT,      KEY_KP_ENTER
};

const int output_pins[4] = {2, 3, 4, 5};
const int input_pins[5] = {28, 27, 21, 20, 19};

bool is_keycodes_changed(const int* prev, const int* curr){
    for (int i=0; i<6; i++){
        if (prev[i]!=curr[i]) return true;
    }
    return false;
}

void core1_main(void){
    if (!flash_safe_execute_core_init()){
        printf("flash_safe_execute_core_init failed");
        return;
    }

    int keycodes_prev[6] = {0, };

    while(1){
        int keycodes[6] = {0, };
        int modifier = 0;
        scan_key_matrix(&modifier, keycodes);
        if (is_keycodes_changed(keycodes_prev, keycodes))
            send_keys(modifier, keycodes);
        memcpy(keycodes_prev, keycodes, sizeof(int)*6);
    }
}

int main(void){
    stdio_init_all();

    init_key_matrix(keymap, output_pins, ARRAY_SIZE(output_pins), input_pins, ARRAY_SIZE(input_pins));

    multicore_launch_core1(core1_main);

    keyboard_start();
}