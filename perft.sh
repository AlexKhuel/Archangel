#!/bin/bash

if [ ! -f ./perft-tester ]; then
    export GLOBIGNORE="main.cpp"

    clang++ -std=c++20 -O3 -ffast-math \
    -mcpu=apple-m4 -flto=thin \
    -Xpreprocessor -fopenmp \
    -I/opt/homebrew/opt/libomp/include \
    -L/opt/homebrew/opt/libomp/lib -lomp \
    *.cpp -o perft-tester

    unset GLOBIGNORE
fi

DEPTH=${1:-1}
FEN=${2:-"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}
MOVES=${3:-""}
./perft-tester "$DEPTH" "$FEN" "$MOVES"