#!/bin/sh

make CONFIG=$1 CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-g++ TARGET_ARCH="-mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4" -j4 2>&1 | tee err.txt

