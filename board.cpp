#include "board.h"
#include "piece.h"
#include <cstddef>
#include <iostream>

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
                if (color == Piece::White)
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
    whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0;
    blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0;
    whiteShortCastle = whiteLongCastle = blackShortCastle = blackLongCastle = false;
    whiteTurn = true;
    passantSquare = 0;
    halfmoveClock = 0;
    fullmoveCounter = 1;

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
            whitePawns += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::Pawn;
            break;
        case 'N':
            whiteKnights += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::Knight;
            break;
        case 'B':
            whiteBishops += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::Bishop;
            break;
        case 'R':
            whiteRooks += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::Rook;
            break;
        case 'Q':
            whiteQueens += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::Queen;
            break;
        case 'K':
            whiteKing += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::White + Piece::King;
            break;
        case 'p':
            blackPawns += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::Pawn;
            break;
        case 'n':
            blackKnights += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::Knight;
            break;
        case 'b':
            blackBishops += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::Bishop;
            break;
        case 'r':
            blackRooks += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::Rook;
            break;
        case 'q':
            blackQueens += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::Queen;
            break;
        case 'k':
            blackKing += (int)pow(2, currSquare);
            pieceArray[currSquare] = Piece::Black + Piece::King;
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
            whiteTurn = true;
        else if (fenString[i] == 'b')
            whiteTurn = false;
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
            int file = fenString[i] - 'a';
            int rank = fenString[i + 1] - '1';
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