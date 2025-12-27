#!/bin/bash
# Compile
clang++ -std=c++20 -O3 -ffast-math \
-mcpu=apple-m4 -flto=thin \
-Xpreprocessor -fopenmp \
-I/opt/homebrew/opt/libomp/include \
-L/opt/homebrew/opt/libomp/lib -lomp \
*.cpp -o chess-bot > /dev/null 2>&1
# Execute with arguments passed from perftree
DEPTH=${1:-1}
FEN=${2:-"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}
MOVES=${3:-""}
./chess-bot "$DEPTH" "$FEN" "$MOVES"