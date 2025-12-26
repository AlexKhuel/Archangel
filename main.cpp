#include "board.h"
#include "movegen.h"
#include <iostream>

int main()
{
    Board testBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto start = std::chrono::steady_clock::now();

    for (int depth = 1; depth < 11; depth++)
    {
        auto now = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        uint64_t moveCount = MoveGen::perft(testBoard, depth);

        std::cout << "Depth: " << depth << " ply   Result: " << moveCount << " Positions    Time: " << elapsed.count() << " milliseconds" << std::endl;

        if (elapsed.count() >= 10000)
        {
            break;
        }
    }
}