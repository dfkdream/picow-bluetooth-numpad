![picow-bluetooth-numpad logo](https://github.com/dfkdream/picow-bluetooth-numpad/blob/e531696fb88a60e6b5b35936c6d37e39d83e5a2e/assets/logo-wordmark.png)
Firmware for Pico W Bluetooth Numpad

# Features
* Open source hardware(3D model files available on Printables)
* Customizable keymap
* Supports both GAP and GATT profile for enhanced compatibility

# Build
## Requirements
Follow `2.2. Install the Toolchain` section of [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) to install build dependencies.

## Steps
1. `git clone https://github.com/dfkdream/picow-bluetooth-numpad.git`
2. `cd picow-bluetooth-numpad`
2. `git submodule update --init`
3. `cd lib/pico-sdk && git submodule update --init`
4. `cd ../../ && mkdir build && cd build`
5. `cmake ..`
6. `make .`

# Configurations
## Modify Keymap
Open `main.c` file and modify the `keymap[]` variable according to your preference.

The definition for keys can be found at `keyboard/scancodes.h`.

## Switch Between GAP and GATT profile
Set `USE_BLE` to `true` for GATT, `false` for GAP.

## Change device name
### GAP profile
1. Open `keyboard/bluetooth_classic.c`
2. Modify `hid_device_name[]` global variable
3. Modify parameter of `gap_set_local_name` function

### GATT profile
1. Open `keyboard/picow-bluetooth-numpad.gatt`
2. Modify line 2
3. Open `keyboard/ble.c`
4. Modify `Name` section of `adv_data[]` global variable. You need to change first hex value to `number of characters in name+1`.
