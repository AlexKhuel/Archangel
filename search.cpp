#include "search.h"

Move Search::findBestMove(Board &board, uint8_t depth)
{
        MoveList list;

        MoveGen::generateMoves(board, list);

        uint8_t index = rand() % list.count;

        return list.moveList[index];
}