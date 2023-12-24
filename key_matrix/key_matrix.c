//
// Created by dfkdream on 23. 12. 24.
//

#include "key_matrix.h"

#include <stdio.h>
#include "pico/stdlib.h"

const int output_pins[4] = {2, 3, 4, 5};
const int input_pins[5] = {28, 27, 21, 20, 19};

static const int *global_keymap = NULL;

void init_key_matrix(const int *keymap){
    int i;

    global_keymap = keymap;

    for (i=0; i<4; i++){
        gpio_init(output_pins[i]);
        gpio_set_dir(output_pins[i], GPIO_OUT);
    }

    for (i=0; i<5; i++){
        gpio_init(input_pins[i]);
        gpio_set_dir(input_pins[i], GPIO_IN);
        gpio_pull_down(input_pins[i]);
    }
}

void scan_key_matrix(int *modifier, int *keycodes) {
    int i, j;
    int pressed_keys_index = 0;

    for (i = 0; i < 4; i++) {
        gpio_put(output_pins[i], true);
        sleep_ms(1);
        for (j = 0; j < 5; j++) {
            if (gpio_get(input_pins[j])) {
                if (pressed_keys_index>5) continue;
                keycodes[pressed_keys_index++] = global_keymap[j*4+i];
            }
        }
        gpio_put(output_pins[i], false);
    }
}