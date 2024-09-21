#!/usr/bin/bash

gcc -g ./include/* ./src/* -o diangit -lssl -lcrypto -lz

if [ $? -eq 0 ]; then
    echo "Compilation successful"
else
    echo "Compilation failed"
fi