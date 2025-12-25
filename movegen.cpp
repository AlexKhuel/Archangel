#include "movegen.h"
#include "board.h"
#include <iostream>

void MoveGen::generateMoves(Board &board)
{
    for (uint8_t i = 0; i < 64; i++)
    {
        Bitboard bitPos = Board::bitPositions[i];

        if ((bitPos & board.bitboards[0][0]) == 0)
            continue;

        int currPiece = board.pieceArray[i];

        switch (currPiece & 7)
        {
        case Piece::PAWN:
            pawnMoves(board, i, bitPos);
            break;
        case Piece::KNIGHT:
            knightMoves(board, i, bitPos);
            break;
        case Piece::BISHOP:
            bishopMoves(board, i, bitPos);
            break;
        case Piece::ROOK:
            rookMoves(board, i, bitPos);
            break;
        case Piece::QUEEN:
            bishopMoves(board, i, bitPos);
            rookMoves(board, i, bitPos);
            break;
        case Piece::KING:
            kingMoves(board, i, bitPos);
            break;
        default:
            std::cout << "You have reached the default case. This is bad." << std::endl;
            break;
        }
    }
}

void MoveGen::pawnMoves(Board &board, uint8_t startPos, Bitboard bitPos)
{
    Bitboard opponentPieces = board.bitboards[1][0];

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

bool MoveGen::tryMove(Board &board, Move testMove)
{
    board.makeMove(testMove);
    bool legal = !board.isInCheck();
    board.unmakeMove(testMove);
    return legal;
}
