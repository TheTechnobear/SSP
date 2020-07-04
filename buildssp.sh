#!/bin/sh

make CONFIG=$1 CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-g++ TARGET_ARCH=-march=armv7-a -j4

