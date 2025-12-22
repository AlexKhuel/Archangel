#include "movegen.h"
#include <iostream>

void MoveGen::generateMoves(Board &board, MoveList &moves)
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

bool MoveGen::isPinned(Board &board, uint8_t currPiece)
{
    // TODO: Write unmakeMove
}

void MoveGen::pawnMoves(Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    Bitboard opponentPieces = board.blackCombined;

    uint8_t currPiece = board.pieceArray[startPos];

    if (startPos % 8 != 0 && ((bitPos << 7) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos + 7)))
    {
        if (startPos / 8 == 6)
        {
            board.addMove(Move(startPos, startPos + 7, Move::PROMOTION, Move::KNIGHT));
            board.addMove(Move(startPos, startPos + 7, Move::PROMOTION, Move::BISHOP));
            board.addMove(Move(startPos, startPos + 7, Move::PROMOTION, Move::ROOK));
            board.addMove(Move(startPos, startPos + 7, Move::PROMOTION, Move::QUEEN));
        }
        else
        {
            board.addMove(Move(startPos, startPos + 7));
        }
    }
    if (startPos % 8 != 7 && ((bitPos << 9) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos + 9)))
    {
        if (startPos / 8 == 6)
        {
            board.addMove(Move(startPos, startPos + 9, Move::PROMOTION, Move::KNIGHT));
            board.addMove(Move(startPos, startPos + 9, Move::PROMOTION, Move::BISHOP));
            board.addMove(Move(startPos, startPos + 9, Move::PROMOTION, Move::ROOK));
            board.addMove(Move(startPos, startPos + 9, Move::PROMOTION, Move::QUEEN));
        }
        else
        {
            board.addMove(Move(startPos, startPos + 9));
        }
    }
    if ((bitPos << 8 & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos + 8)))
    {
        if (startPos / 8 == 6)
        {
            board.addMove(Move(startPos, startPos + 8, Move::PROMOTION, Move::KNIGHT));
            board.addMove(Move(startPos, startPos + 8, Move::PROMOTION, Move::BISHOP));
            board.addMove(Move(startPos, startPos + 8, Move::PROMOTION, Move::ROOK));
            board.addMove(Move(startPos, startPos + 8, Move::PROMOTION, Move::QUEEN));
        }
        else
        {
            board.addMove(Move(startPos, startPos + 8));
        }
    }
    if ((startPos / 8) == 1 && (bitPos << 8 & board.allCombined) == 0 && (bitPos << 16 & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos + 16)))
    {
        board.addMove(Move(startPos, startPos + 16));
    }
    if (startPos % 8 != 0 && board.passantSquare == startPos + 7 && tryMove(board, Move(startPos, startPos + 7)))
    {
        board.addMove(Move(startPos, startPos + 7, Move::PASSANT));
    }
    if (startPos % 8 != 7 && board.passantSquare == startPos + 9 && tryMove(board, Move(startPos, startPos + 9)))
    {

        board.addMove(Move(startPos, startPos + 9, Move::PASSANT));
    }
}

void MoveGen::knightMoves(Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    // TODO: Write knightMoves
}

void MoveGen::bishopMoves(Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    // TODO: Write bishopMoves
}

void MoveGen::rookMoves(Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    // TODO: Write rookMoves
}

void MoveGen::kingMoves(Board &board, uint8_t startPos, uint64_t bitPos, uint64_t colorMask)
{
    // TODO: Write kingMoves
}

bool MoveGen::tryMove(Board &board, Move testMove)
{
    board.makeMove(testMove);
    bool legal = !board.isInCheck();
    board.unmakeMove(testMove); // Revert the board state
    return legal;
}
