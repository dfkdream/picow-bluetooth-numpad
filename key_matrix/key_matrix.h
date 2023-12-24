//
// Created by dfkdream on 23. 12. 24.
//

#ifndef PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H
#define PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H

void init_key_matrix(const int *keymap);
void scan_key_matrix(int *modifier, int *keycodes);

#endif //PICOW_BLUETOOTH_NUMPAD_KEY_MATRIX_H
