#include "board.h"
#include "movelist.h"

class MoveGen
{
public:
    void MoveGen::generateMoves(const Board &board, MoveList &moves);

private:
    bool inCheck(const Board &board, bool whiteTurn);
    bool addMove(MoveList &moves, uint8_t startPos, uint8_t endPos);
    void pawnMoves(const Board &board, uint8_t startPos);
    void knightMoves(const Board &board, uint8_t startPos);
    void bishopMoves(const Board &board, uint8_t startPos);
    void rookMoves(const Board &board, uint8_t startPos);
    void queenMoves(const Board &board, uint8_t startPos);
    void kingMoves(const Board &board, uint8_t startPos);
};
