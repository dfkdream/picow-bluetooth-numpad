//
// Created by dfkdream on 23. 11. 27.
//

#ifndef PICOW_BLUETOOTH_NUMPAD_KEYBOARD_H
#define PICOW_BLUETOOTH_NUMPAD_KEYBOARD_H

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

void keyboard_start();
void send_keys(int modifier, const int *keycodes);

#endif //PICOW_BLUETOOTH_NUMPAD_KEYBOARD_H
