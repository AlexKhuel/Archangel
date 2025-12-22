#ifndef BOARD_H
#define BOARD_H

#include "movelist.h"
#include <cstdint>
#include <string>

class Board
{
    using Bitboard = uint64_t;

public:
    Board(std::string fenSring);
    Bitboard whitePawns;
    Bitboard whiteKnights;
    Bitboard whiteBishops;
    Bitboard whiteRooks;
    Bitboard whiteQueens;
    Bitboard whiteKing;
    Bitboard whiteCombined;
    Bitboard whitePinned;

    Bitboard blackPawns;
    Bitboard blackKnights;
    Bitboard blackBishops;
    Bitboard blackRooks;
    Bitboard blackQueens;
    Bitboard blackKing;
    Bitboard blackCombined;
    Bitboard blackPinned;

    Bitboard allCombined;

    uint8_t pieceArray[64];

    MoveList possibleMoves;

    bool isWhiteTurn;
    bool whiteShortCastle;
    bool whiteLongCastle;
    bool blackShortCastle;
    bool blackLongCastle;

    uint8_t passantSquare;
    uint8_t halfmoveClock;

    bool evenNumMove;
    uint16_t fullmoveCounter;

public:
    void makeMove(Move currMove);
    void unmakeMove(Move currMove);
    bool isInCheck();
    void addMove(Move moveToAdd);
    void clearMoveList();
    void printChessBoard();
    void nextTurn();
};

#endif