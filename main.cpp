#include "board.h"
#include "movegen.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // 1. HANDLE DIRECT COMMAND LINE ARGUMENTS (for ./check.sh 3 "FEN")
    if (argc >= 3)
    {
        int depth = std::stoi(argv[1]);
        std::string fen = argv[2];
        Board perftBoard(fen);
        MoveGen::perft(perftBoard, depth, true);
        return 0;
    }

    // 2. HANDLE INTERACTIVE UCI/PERFTREE STDIN
    std::string line;
    Board currentBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    while (std::getline(std::cin, line))
    {
        if (line == "uci")
        {
            std::cout << "id name Archangel" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (line == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        // Handle "position fen ..."
        else if (line.substr(0, 12) == "position fen")
        {
            std::string fen = line.substr(13);
            currentBoard = Board(fen);
        }
        // Handle "go perft X" or just "perft X"
        else if (line.find("perft") != std::string::npos)
        {
            size_t pos = line.find_last_of(' ');
            int depth = std::stoi(line.substr(pos + 1));
            MoveGen::perft(currentBoard, depth, true);
        }
        else if (line == "quit")
        {
            break;
        }
    }
    return 0;
}