//
// Created by dfkdream on 23. 11. 27.
//

#ifndef PICOW_BLUETOOTH_NUMPAD_BLE_KEYBOARD_H
#define PICOW_BLUETOOTH_NUMPAD_BLE_KEYBOARD_H

void ble_keyboard_start();
void send_keys(int modifier, const int *keycodes);

#endif //PICOW_BLUETOOTH_NUMPAD_BLE_KEYBOARD_H
