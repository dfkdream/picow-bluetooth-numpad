//
// Created by dfkdream on 23. 12. 24.
//

#ifndef PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H
#define PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H

void init_key_matrix(const int *keymap, const int *output_pins, int n_output_pins, const int *input_pins, int n_input_pins);
void scan_key_matrix(int *modifier, int *keycodes);

#endif //PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H
