#!/usr/bin/zsh

screen /dev/ttyACM0 1200 &&
sleep 3 &&
cp cmake-build-debug/picow-bluetooth-numpad.uf2 /media/$(whoami)/RPI-RP2 &&
sleep 3 &&
screen /dev/ttyACM0 115200