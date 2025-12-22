#include "board.h"
#include <iostream>

int main()
{
    Board testBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << testBoard.whiteKing << std::endl;
    testBoard.printChessBoard();
}