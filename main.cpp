#include "board.h"
#include "movegen.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char *argv[])
{
    // 1. Defaults
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    int depth = 1;
    std::string moveList = "";

    // 2. Safely parse Depth (Arg 1)
    if (argc > 1)
    {

        depth = std::stoi(argv[1]);
    }

    // 3. Parse FEN (Arg 2)
    if (argc > 2)
    {
        fen = argv[2];
    }

    // 4. Parse Move List (Arg 3)
    if (argc > 3)
    {
        moveList = argv[3];
    }

    Board testBoard(fen);

    // 5. Apply moves
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

    // 6. Execute Perft
    // testBoard.printChessBoard();
    // std::cout << depth << std::endl;

    uint64_t totalNodes = MoveGen::perft(testBoard, depth, true);

    // Final output for perftree
    std::cout << "\n"
              << totalNodes << std::endl;
}