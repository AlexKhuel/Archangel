#!/bin/bash
# Compile
clang++ -std=c++20 -O3 -ffast-math \
-mcpu=apple-m4 -flto=thin \
-Xpreprocessor -fopenmp \
-I/opt/homebrew/opt/libomp/include \
-L/opt/homebrew/opt/libomp/lib -lomp \
*.cpp -o chess-bot > /dev/null 2>&1
# Execute with arguments passed from perftree
./chess-bot "$1" "$2"