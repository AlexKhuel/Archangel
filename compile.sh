#!/bin/bash

# Check if "debug" was passed as an argument: ./compile.sh debug
if [ "$1" == "debug" ]; then
    echo "Debug Compile"
    clang++ -std=c++20 -g -O0 \
    -Xpreprocessor \
    -I/opt/homebrew/opt/libomp/include \
    -L/opt/homebrew/opt/libomp/lib -lomp \
    *.cpp -o chess-bot
else
    echo "Fast Compile"
    clang++ -std=c++20 -O3 -ffast-math \
    -mcpu=apple-m4 -flto=thin \
    -Xpreprocessor -fopenmp \
    -I/opt/homebrew/opt/libomp/include \
    -L/opt/homebrew/opt/libomp/lib -lomp \
    *.cpp -o chess-bot
fi