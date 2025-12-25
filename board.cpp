#include "board.h"
#include "piece.h"
#include <cstddef>
#include <iostream>

void Board::makeMove(Move currMove)
{
    Bitboard fromBit = bitPositions[currMove.getFrom()];
    Bitboard toBit = bitPositions[currMove.getTo()];
    uint8_t fromType = pieceArray[currMove.getFrom()] & Piece::TYPE_MASK;
    uint8_t toType = pieceArray[currMove.getTo()] & Piece::TYPE_MASK;

    if (fromType == Piece::KNIGHT || fromType == Piece::BISHOP || fromType == Piece::QUEEN)
    {

        // Updates friendly piece and combined Bitboards
        bitboards[0][fromType] ^= fromBit;
        bitboards[0][0] ^= fromBit;

        bitboards[0][fromType] |= toBit;
        bitboards[0][0] |= toBit;

        // Updates opponents piece and combined Bitboards if there is a capture
        if (toType != 0)
        {
            bitboards[1][toType] ^= toBit;
            bitboards[1][0] ^= toBit;
        }

        // Updates combined board
        allCombined ^= fromBit;
        allCombined |= toBit;

        // Updates the piece array
        pieceArray[currMove.getTo()] = pieceArray[currMove.getFrom()] & 15;
        pieceArray[currMove.getFrom()] = 0;
    }
    else if ((fromType == Piece::ROOK || fromType == Piece::KING) && (whiteShortCastle || whiteLongCastle))
    {
    }
    else
    {
    }
}

void Board::addMove(Move moveToAdd)
{
    this->possibleMoves.add(moveToAdd);
}

void Board::clearMoveList()
{
    this->possibleMoves.clear();
}

void Board::printChessBoard()
{
    // Unicode chess piece symbols
    const char *whitePieces[] = {" ", "♙", "♘", "♗", "♖", "♕", "♔"};
    const char *blackPieces[] = {" ", "♟", "♞", "♝", "♜", "♛", "♚"};

    std::cout << "  a b c d e f g h\n";

    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << (rank + 1) << " ";

        for (int file = 0; file < 8; file++)
        {
            int index = rank * 8 + file;
            uint8_t piece = pieceArray[index];

            uint8_t type = piece & 0x7;  // Extract piece type (bits 0-2)
            uint8_t color = piece & 0x8; // Extract color (bit 3)

            const char *symbol = " ";
            if (type > 0 && type <= 6)
            {
                if (color == Piece::WHITE)
                {
                    symbol = whitePieces[type];
                }
                else
                {
                    symbol = blackPieces[type];
                }
            }

            std::cout << symbol << " ";
        }

        std::cout << (rank + 1) << "\n";
    }

    std::cout << "  a b c d e f g h\n";
}

Board::Board(std::string fenString)
{
    for (uint8_t color = 0; color < 2; color++)
    {
        for (uint8_t type = 0; type < 8; type++)
        {
            bitboards[color][type] = 0;
        }
    }

    allCombined = 0;

    whiteShortCastle = whiteLongCastle = blackShortCastle = blackLongCastle = false;
    isWhiteTurn = true;
    passantSquare = 0;
    halfmoveClock = 0;
    fullmoveCounter = 1;
    possibleMoves.clear();

    for (int i = 0; i < 64; i++)
        pieceArray[i] = 0;

    int rank = 7;
    int file = 0;
    size_t i = 0;

    // Section 1: Parse board position
    while (i < fenString.length() && fenString[i] != ' ')
    {
        char currChar = fenString[i];

        if (currChar == '/')
        {
            rank--;
            file = 0;
            i++;
            continue;
        }

        if (isdigit(currChar))
        {
            file += currChar - '0';
            i++;
            continue;
        }

        int currSquare = rank * 8 + file;

        switch (currChar)
        {
        case 'P':
            bitboards[0][Piece::PAWN] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::PAWN;
            break;
        case 'N':
            bitboards[0][Piece::KNIGHT] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::KNIGHT;
            break;
        case 'B':
            bitboards[0][Piece::BISHOP] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::BISHOP;
            break;
        case 'R':
            bitboards[0][Piece::ROOK] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::ROOK;
            break;
        case 'Q':
            bitboards[0][Piece::QUEEN] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::QUEEN;
            break;
        case 'K':
            bitboards[0][Piece::KING] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::WHITE + Piece::KING;
            break;
        case 'p':
            bitboards[1][Piece::PAWN] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::PAWN;
            break;
        case 'n':
            bitboards[1][Piece::KNIGHT] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::KNIGHT;
            break;
        case 'b':
            bitboards[1][Piece::BISHOP] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::BISHOP;
            break;
        case 'r':
            bitboards[1][Piece::ROOK] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::ROOK;
            break;
        case 'q':
            bitboards[1][Piece::QUEEN] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::QUEEN;
            break;
        case 'k':
            bitboards[1][Piece::KING] += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::BLACK + Piece::KING;
            break;
        }

        file++;
        i++;
    }

    i++;

    // Section 2: Active color
    if (i < fenString.length())
    {
        if (fenString[i] == 'w')
            isWhiteTurn = true;
        else if (fenString[i] == 'b')
            isWhiteTurn = false;
        i += 2;
    }

    // Section 3: Castling rights
    if (i < fenString.length())
    {
        while (i < fenString.length() && fenString[i] != ' ')
        {
            switch (fenString[i])
            {
            case 'K':
                whiteShortCastle = true;
                break;
            case 'Q':
                whiteLongCastle = true;
                break;
            case 'k':
                blackShortCastle = true;
                break;
            case 'q':
                blackLongCastle = true;
                break;
            }
            i++;
        }
        i++;
    }

    // Section 4: En passant square
    if (i < fenString.length())
    {
        if (fenString[i] != '-')
        {
            file = fenString[i] - 'a';
            rank = fenString[i + 1] - '1';
            passantSquare = (uint8_t)(rank * 8 + file);
            i += 2;
        }
        else
        {
            i++;
        }

        if (i < fenString.length() && fenString[i] == ' ')
            i++;
    }

    // Section 5: Halfmove clock
    if (i < fenString.length())
    {
        std::string halfmoveStr;
        while (i < fenString.length() && isdigit(fenString[i]))
        {
            halfmoveStr += fenString[i++];
        }
        if (!halfmoveStr.empty())
            halfmoveClock = std::stoi(halfmoveStr);
        else
            halfmoveClock = 0;

        if (i < fenString.length() && fenString[i] == ' ')
            i++;
    }

    // Section 6: Fullmove counter
    if (i < fenString.length())
    {
        std::string fullmoveStr;
        while (i < fenString.length() && isdigit(fenString[i]))
        {
            fullmoveStr += fenString[i++];
        }
        if (!fullmoveStr.empty())
            fullmoveCounter = std::stoi(fullmoveStr);
        else
            fullmoveCounter = 1;
    }
}

void Board::nextTurn()
{
    for (uint8_t color = 0; color < 2; color++)
    {
        for (uint8_t type = 0; type < 8; type++)
        {
            bitboards[color][type] = __builtin_bswap64(bitboards[color][type]);
        }
    }

    allCombined = __builtin_bswap64(allCombined);

    clearMoveList();

    for (int i = 0; i < 32; i++)
    {
        std::swap(pieceArray[i], pieceArray[63 - i]);
    }

    std::swap(whiteShortCastle, blackShortCastle);
    std::swap(whiteLongCastle, blackLongCastle);

    if (!evenNumMove)
        fullmoveCounter++;
    evenNumMove = !evenNumMove;
}