#!/bin/bash
# High-intensity check for every C++ file
clang++ -std=c++20 -O3 -mcpu=apple-m4 -flto -Xpreprocessor -fopenmp \
-L/opt/homebrew/opt/libomp/lib \
-I/opt/homebrew/opt/libomp/include \
-lomp *.cpp -o parallel_m4_app