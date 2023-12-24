//
// Created by dfkdream on 23. 12. 24.
//

#include "key_matrix.h"

#include <stdio.h>
#include "pico/stdlib.h"

static const int *global_keymap = NULL;

static const int *outputs = NULL;
static int n_outputs = 0;
static const int *inputs = NULL;
static int n_inputs = 0;

void init_key_matrix(const int *keymap, const int *output_pins, int n_output_pins, const int *input_pins, int n_input_pins){
    int i;

    global_keymap = keymap;
    outputs = output_pins;
    n_outputs = n_output_pins;
    inputs = input_pins;
    n_inputs = n_input_pins;

    for (i=0; i<n_outputs; i++){
        gpio_init(outputs[i]);
        gpio_set_dir(outputs[i], GPIO_OUT);
    }

    for (i=0; i<n_inputs; i++){
        gpio_init(inputs[i]);
        gpio_set_dir(inputs[i], GPIO_IN);
        gpio_pull_down(inputs[i]);
    }
}

void scan_key_matrix(int *modifier, int *keycodes) {
    int i, j;
    int pressed_keys_index = 0;

    for (i = 0; i < n_outputs; i++) {
        gpio_put(outputs[i], true);
        sleep_ms(1);
        for (j = 0; j < n_inputs; j++) {
            if (gpio_get(inputs[j])) {
                if (pressed_keys_index>5) continue;
                keycodes[pressed_keys_index++] = global_keymap[j*(n_inputs-1)+i];
            }
        }
        gpio_put(outputs[i], false);
    }
}