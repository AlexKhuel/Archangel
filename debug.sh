#!/bin/bash
# Debug compilation with symbols and less optimization
clang++ -std=c++20 -g -O0 -mcpu=apple-m4 \
-Xpreprocessor -fopenmp \
-L/opt/homebrew/opt/libomp/lib \
-I/opt/homebrew/opt/libomp/include \
-lomp *.cpp -o chess-bot-debug