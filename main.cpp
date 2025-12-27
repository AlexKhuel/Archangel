#include "board.h"
#include "movegen.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    // 1. Defaults
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    int depth = 1;
    std::string moveList = "";

    if (argc > 1)
    {

        depth = std::stoi(argv[1]);
    }

    if (argc > 2)
    {
        fen = argv[2];
    }

    if (argc > 3)
    {
        moveList = argv[3];
    }

    Board testBoard(fen);

    if (!moveList.empty())
    {
        std::stringstream ss(moveList);
        std::string mStr;
        while (ss >> mStr)
        {
            Move m = testBoard.parseMove(mStr);
            testBoard.makeMove(m);
        }
    }

    uint64_t totalNodes = MoveGen::perft(testBoard, depth, true);

    std::cout << "\n"
              << totalNodes << std::endl;
}