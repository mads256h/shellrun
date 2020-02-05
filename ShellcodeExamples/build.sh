#!/bin/sh
find . -type f -name *.asm -exec sh -c 'nasm -o "${0%.asm}.bin" -f bin "$0" ' {} \;
