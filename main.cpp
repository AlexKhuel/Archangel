#include "board.h"
#include "movegen.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "board.h"
#include "movegen.h"
#include <iostream>
/*
int main()
{
    Board testBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto start = std::chrono::steady_clock::now();

    for (int depth = 1; depth < 11; depth++)

    {
        uint64_t moveCount = MoveGen::perft(testBoard, depth, true);

        auto now = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        std::cout << "Depth: " << depth << " ply   Result: " << moveCount << " Positions    Time: " << elapsed.count() << " milliseconds" << std::endl;
    }
}

*/

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

/*
int main()
{
    Board testBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    testBoard.printChessBoard();
    testBoard.makeMove(testBoard.parseMove("h2h4"));
    testBoard.makeMove(testBoard.parseMove("h7h6"));
    testBoard.makeMove(testBoard.parseMove("h4h5"));
    testBoard.makeMove(testBoard.parseMove("g7g5"));
    testBoard.makeMove(Move(39, 46, Move::PASSANT));
    testBoard.printChessBoard();
}
*/