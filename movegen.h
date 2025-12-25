#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "movelist.h"
#include "piece.h"
#include "types.h"

class Board;

class MoveGen
{

public:
    void generateMoves(Board &board, MoveList &list);
    static bool isPinned(const Board &board, uint8_t currPiece);

    // Directions: {-1,1,-8,8,-7,7,-9,9} = {S,N,W,E,SW,NE,NW,SE}
    // Square 0 = bottom-left, Square 63 = top-right
    static constexpr uint8_t disToEdge[64][8] = {
        {0, 7, 0, 7, 0, 7, 0, 0}, {0, 7, 1, 6, 0, 6, 1, 0}, {0, 7, 2, 5, 0, 5, 2, 0}, {0, 7, 3, 4, 0, 4, 3, 0}, {0, 7, 4, 3, 0, 3, 4, 0}, {0, 7, 5, 2, 0, 2, 5, 0}, {0, 7, 6, 1, 0, 1, 6, 0}, {0, 7, 7, 0, 0, 0, 7, 0}, {1, 6, 0, 7, 0, 6, 0, 1}, {1, 6, 1, 6, 1, 6, 1, 1}, {1, 6, 2, 5, 1, 5, 2, 1}, {1, 6, 3, 4, 1, 4, 3, 1}, {1, 6, 4, 3, 1, 3, 4, 1}, {1, 6, 5, 2, 1, 2, 5, 1}, {1, 6, 6, 1, 1, 1, 6, 1}, {1, 6, 7, 0, 1, 0, 7, 1}, {2, 5, 0, 7, 0, 5, 0, 2}, {2, 5, 1, 6, 1, 5, 1, 2}, {2, 5, 2, 5, 2, 5, 2, 2}, {2, 5, 3, 4, 2, 4, 3, 2}, {2, 5, 4, 3, 2, 3, 4, 2}, {2, 5, 5, 2, 2, 2, 5, 2}, {2, 5, 6, 1, 2, 1, 6, 2}, {2, 5, 7, 0, 2, 0, 7, 2}, {3, 4, 0, 7, 0, 4, 0, 3}, {3, 4, 1, 6, 1, 4, 1, 3}, {3, 4, 2, 5, 2, 4, 2, 3}, {3, 4, 3, 4, 3, 4, 3, 3}, {3, 4, 4, 3, 3, 3, 4, 3}, {3, 4, 5, 2, 3, 2, 5, 3}, {3, 4, 6, 1, 3, 1, 6, 3}, {3, 4, 7, 0, 3, 0, 7, 3}, {4, 3, 0, 7, 0, 3, 0, 4}, {4, 3, 1, 6, 1, 3, 1, 4}, {4, 3, 2, 5, 2, 3, 2, 4}, {4, 3, 3, 4, 3, 3, 3, 4}, {4, 3, 4, 3, 4, 3, 4, 4}, {4, 3, 5, 2, 4, 2, 5, 4}, {4, 3, 6, 1, 4, 1, 6, 4}, {4, 3, 7, 0, 4, 0, 7, 4}, {5, 2, 0, 7, 0, 2, 0, 5}, {5, 2, 1, 6, 1, 2, 1, 5}, {5, 2, 2, 5, 2, 2, 2, 5}, {5, 2, 3, 4, 3, 2, 3, 5}, {5, 2, 4, 3, 4, 2, 4, 5}, {5, 2, 5, 2, 5, 2, 5, 5}, {5, 2, 6, 1, 5, 1, 6, 5}, {5, 2, 7, 0, 5, 0, 7, 5}, {6, 1, 0, 7, 0, 1, 0, 6}, {6, 1, 1, 6, 1, 1, 1, 6}, {6, 1, 2, 5, 2, 1, 2, 6}, {6, 1, 3, 4, 3, 1, 3, 6}, {6, 1, 4, 3, 4, 1, 4, 6}, {6, 1, 5, 2, 5, 1, 5, 6}, {6, 1, 6, 1, 6, 1, 6, 6}, {6, 1, 7, 0, 6, 0, 7, 6}, {7, 0, 0, 7, 0, 0, 0, 7}, {7, 0, 1, 6, 0, 0, 1, 7}, {7, 0, 2, 5, 0, 0, 2, 7}, {7, 0, 3, 4, 0, 0, 3, 7}, {7, 0, 4, 3, 0, 0, 4, 7}, {7, 0, 5, 2, 0, 0, 5, 7}, {7, 0, 6, 1, 0, 0, 6, 7}, {7, 0, 7, 0, 0, 0, 7, 7}};

private:
    static void pawnMoves(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list);
    static void knightMoves(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list);
    static void bishopMoves(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list);
    static void rookMoves(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list);
    static void kingMoves(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list);
    static bool tryMove(Board &board, Move testMove);
};

#endif