#include "movegen.h"
#include <iostream>

void MoveGen::generateMoves(const Board &board, MoveList &moves)
{
    for (uint8_t i = 0; i < 64; i++)
    {
        uint64_t bitPos = bitPositions[i];
        uint64_t colorMask = board.isWhiteTurn ? board.whiteCombined : board.blackCombined;

        if ((bitPos & colorMask) == 0)
            continue;

        int currPiece = board.pieceArray[i];

        switch (currPiece & 7)
        {
        case Piece::Pawn:
            pawnMoves(board, i, bitPos, colorMask);
            break;
        case Piece::Knight:
            knightMoves(board, i, bitPos, colorMask);
            break;
        case Piece::Bishop:
            bishopMoves(board, i, bitPos, colorMask);
            break;
        case Piece::Rook:
            rookMoves(board, i, bitPos, colorMask);
            break;
        case Piece::Queen:
            bishopMoves(board, i, bitPos, colorMask);
            rookMoves(board, i, bitPos, colorMask);
            break;
        case Piece::King:
            kingMoves(board, i, bitPos, colorMask);
            break;
        default:
            std::cout << "You have reached the default case. This is bad." << std::endl;
            break;
        }
    }
}

bool MoveGen::inCheck(const Board &board, bool isWhiteTurn)
{
    return false;
}

bool MoveGen::addMove(MoveList &moves, uint8_t startPos, uint8_t endPos)
{
    return false;
}

void MoveGen::pawnMoves(const Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    Bitboard friendlyPieces = board.isWhiteTurn ? board.whiteCombined : board.blackCombined;
    Bitboard opponentPieces = board.isWhiteTurn ? board.blackCombined : board.whiteCombined;

    uint8_t currPiece = board.pieceArray[startPos];

    if ((currPiece & Piece::White) == Piece::White)
    {
        if (((bitPos >> 7) & opponentPieces) != 0 && (currPiece & Piece::Pinned) == 0)
        {
                }
    }
    else
    {
    }
}

void MoveGen::knightMoves(const Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
}

void MoveGen::bishopMoves(const Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
}

void MoveGen::rookMoves(const Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
}

void MoveGen::kingMoves(const Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
}
