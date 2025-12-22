#ifndef BOARD_H
#define BOARD_H

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

    bool isWhiteTurn;
    bool whiteShortCastle;
    bool whiteLongCastle;
    bool blackShortCastle;
    bool blackLongCastle;

    uint8_t passantSquare;
    uint8_t halfmoveClock;
    uint16_t fullmoveCounter;

public:
    void makeMove();
    void unmakeMove();
    void clearMoveList();
    void printChessBoard();
};

#endif