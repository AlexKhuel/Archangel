#include "board.h"
#include "piece.h"
#include <cstddef>
#include <iostream>

void Board::makeMove(Move currMove)
{
	moveHistory.push_back(currMove);

	Bitboard fromBit = bitPositions[currMove.getFrom()];
	Bitboard toBit = bitPositions[currMove.getTo()];
	uint8_t fromType = pieceArray[currMove.getFrom()] & Piece::TYPE_MASK;
	uint8_t toType = pieceArray[currMove.getTo()] & Piece::TYPE_MASK;

	passantSquare = -1;

	if ((fromType == Piece::ROOK || fromType == Piece::KING) && (whiteShortCastle || whiteLongCastle))
	{
		rookKingMove(fromType, toType, fromBit, toBit, currMove);
	}
	else if (fromType == Piece::KNIGHT || fromType == Piece::BISHOP || fromType == Piece::QUEEN)
	{
		normalMove(fromType, toType, fromBit, toBit, currMove);
	}
	else
	{
		pawnMove(fromType, toType, fromBit, toBit, currMove);
	}

	nextTurn();
}

void Board::normalMove(uint8_t fromType, uint8_t toType, Bitboard fromBit, Bitboard toBit, Move currMove)
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

void Board::rookKingMove(uint8_t fromType, uint8_t toType, Bitboard fromBit, Bitboard toBit, Move currMove)
{
	if (fromType == Piece::KING)
	{
		whiteShortCastle = whiteLongCastle = false;
	}
	else if (currMove.getFrom() == 0 && Piece::ROOK)
	{
		whiteLongCastle = false;
	}
	else if (currMove.getFrom() == 7 && Piece::ROOK)
	{
		whiteShortCastle = false;
	}

	if (currMove.isCastling())
	{
		pieceArray[4] = 0;
		bitboards[0][Piece::KING] = 0;
		bitboards[0][0] ^= bitPositions[4];
		allCombined ^= bitPositions[4];
		if (currMove.getTo() == 2)
		{
			pieceArray[2] = Piece::KING | Piece::WHITE;
			pieceArray[3] = Piece::ROOK | Piece::WHITE;
			pieceArray[0] = 0;

			bitboards[0][Piece::KING] = bitPositions[2];
			bitboards[0][0] |= bitPositions[2];
			allCombined |= bitPositions[2];

			bitboards[0][Piece::ROOK] |= bitPositions[3];
			bitboards[0][0] |= bitPositions[3];
			allCombined |= bitPositions[3];

			bitboards[0][Piece::ROOK] ^= bitPositions[0];
			bitboards[0][0] ^= bitPositions[0];
			allCombined ^= bitPositions[0];
		}
		else
		{
			pieceArray[6] = Piece::KING | Piece::WHITE;
			pieceArray[5] = Piece::ROOK | Piece::WHITE;
			pieceArray[7] = 0;

			bitboards[0][Piece::KING] = bitPositions[6];
			bitboards[0][0] |= bitPositions[6];
			allCombined |= bitPositions[6];

			bitboards[0][Piece::ROOK] |= bitPositions[5];
			bitboards[0][0] |= bitPositions[5];
			allCombined |= bitPositions[5];

			bitboards[0][Piece::ROOK] ^= bitPositions[7];
			bitboards[0][0] ^= bitPositions[7];
			allCombined ^= bitPositions[7];
		}
	}
	else
	{
		normalMove(fromType, toType, fromBit, toBit, currMove);
	}
}

void Board::pawnMove(uint8_t fromType, uint8_t toType, Bitboard fromBit, Bitboard toBit, Move currMove)
{
	if (currMove.isPassant())
	{
		int sideShift = (currMove.getTo() % 8) - (currMove.getFrom() % 8);

		// Updates friendly piece and combined Bitboards
		bitboards[0][fromType] ^= fromBit;
		bitboards[0][0] ^= fromBit;

		bitboards[0][fromType] |= toBit;
		bitboards[0][0] |= toBit;

		// Updates opponents piece and combined Bitboards if there is a capture
		bitboards[1][Piece::PAWN] ^= bitPositions[currMove.getFrom() + sideShift];
		bitboards[1][0] ^= bitPositions[currMove.getFrom() + sideShift];

		// Updates combined board
		allCombined ^= fromBit;
		allCombined |= toBit;
		allCombined ^= bitPositions[currMove.getFrom() + sideShift];

		// Updates the piece array
		pieceArray[currMove.getTo()] = pieceArray[currMove.getFrom()] & 15;
		pieceArray[currMove.getFrom()] = 0;
		pieceArray[currMove.getFrom() + sideShift] = 0;
	}
	else
	{

		normalMove(fromType, toType, fromBit, toBit, currMove);

		if ((currMove.getTo() / 8) - (currMove.getFrom() / 8) == 2)
		{
			passantSquare = currMove.getTo() - 8;
		}
	}
}

void Board::unmakeMove(const BoardState &oldBoardState)
{
	if (!moveHistory.empty())
		moveHistory.pop_back();
	// 1. Restore the Bitboard array (2x8 = 16 elements)
	// We treat the 2D array as a flat block of 16 Bitboards
	std::copy(&oldBoardState.bitboards[0][0], &oldBoardState.bitboards[0][0] + 16, &bitboards[0][0]);

	// 2. Restore the Piece Array (64 elements)
	std::copy(oldBoardState.pieceArray, oldBoardState.pieceArray + 64, pieceArray);

	// 3. Restore the individual variables
	allCombined = oldBoardState.allCombined;

	whiteShortCastle = oldBoardState.whiteShortCastle;
	whiteLongCastle = oldBoardState.whiteLongCastle;
	blackShortCastle = oldBoardState.blackShortCastle;
	blackLongCastle = oldBoardState.blackLongCastle;

	passantSquare = oldBoardState.passantSquare;
	halfmoveClock = oldBoardState.halfmoveClock;

	if (evenNumMove)
		fullmoveCounter--;
	evenNumMove = !evenNumMove;
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

			uint8_t type = piece & 0x7;	 // Extract piece type (bits 0-2)
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
	moveHistory.reserve(10);

	bool swapLater = false;

	for (uint8_t color = 0; color < 2; color++)
	{
		for (uint8_t type = 0; type < 8; type++)
		{
			bitboards[color][type] = 0;
		}
	}

	allCombined = 0;

	whiteShortCastle = whiteLongCastle = blackShortCastle = blackLongCastle = false;
	passantSquare = 0;
	halfmoveClock = 0;
	fullmoveCounter = 1;

	for (int i = 0; i < 64; i++)
		pieceArray[i] = 0;

	int rank = 7;
	int file = 0;
	size_t i = 0;

	// 1. Ensure rank/file starts correctly (Rank 7 is top of FEN, Rank 0 is bottom)

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
		uint64_t bit = 1ULL << currSquare; // THE CORRECT WAY TO GET BIT

		// Determine color and piece type
		int color = islower(currChar) ? 1 : 0; // lowercase = black (1), uppercase = white (0)

		// Map char to piece type (using a small helper or manual switch)
		int type;
		char lower = tolower(currChar);
		if (lower == 'p')
			type = Piece::PAWN;
		else if (lower == 'n')
			type = Piece::KNIGHT;
		else if (lower == 'b')
			type = Piece::BISHOP;
		else if (lower == 'r')
			type = Piece::ROOK;
		else if (lower == 'q')
			type = Piece::QUEEN;
		else if (lower == 'k')
			type = Piece::KING;

		// SET THE BITS
		bitboards[color][type] |= bit;
		pieceArray[currSquare] = (color == 0 ? Piece::WHITE : Piece::BLACK) + type;

		file++;
		i++;
	}

	// Combine everything correctly
	bitboards[0][0] = 0; // Reset index 0 to be sure
	bitboards[1][0] = 0;
	for (int t = 1; t <= 6; t++)
	{
		bitboards[0][0] |= bitboards[0][t]; // White combined
		bitboards[1][0] |= bitboards[1][t]; // Black combined
	}
	allCombined = bitboards[0][0] | bitboards[1][0];

	// Section 2: Active color
	if (i < fenString.length())
	{
		if (fenString[i] == 'b')
		{
			swapLater = true;
		}
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
	if (swapLater)
	{
		for (uint8_t color = 0; color < 2; color++)
		{
			for (uint8_t type = 0; type < 8; type++)
			{
				bitboards[color][type] = __builtin_bswap64(bitboards[color][type]);
			}
		}

		allCombined = __builtin_bswap64(allCombined);

		for (int index = 0; index < 32; index++)
		{
			std::swap(pieceArray[index], pieceArray[63 - index]);
		}

		std::swap(whiteShortCastle, blackShortCastle);
		std::swap(whiteLongCastle, blackLongCastle);
	}
}

void Board::nextTurn()
{
	// 1. Swap the White and Black bitboard sets
	for (int i = 0; i < 8; ++i)
	{
		std::swap(bitboards[0][i], bitboards[1][i]);
	}

	// 2. Vertically flip all bitboards (0-6 are pieces, 0 is combined)
	auto flipBits = [](Bitboard &b)
	{
		b = ((b >> 8) & 0x00FF00FF00FF00FFULL) | ((b << 8) & 0xFF00FF00FF00FF00ULL);
		b = ((b >> 16) & 0x0000FFFF0000FFFFULL) | ((b << 16) & 0xFFFF0000FFFF0000ULL);
		b = (b >> 32) | (b << 32);
	};

	for (int i = 0; i < 7; ++i)
	{ // Flipping 0 (combined) through 6 (King)
		flipBits(bitboards[0][i]);
		flipBits(bitboards[1][i]);
	}
	flipBits(allCombined);

	// 3. Rebuild pieceArray from the flipped bitboards
	// Clear array first
	for (int i = 0; i < 64; ++i)
		pieceArray[i] = 0;

	for (uint8_t color = 0; color < 2; ++color)
	{
		// We use bit 3 for Black (8) and 0 for White as per your Piece class
		uint8_t colorMask = (color == 0) ? Piece::WHITE : Piece::BLACK;

		for (uint8_t type = Piece::PAWN; type <= Piece::KING; ++type)
		{
			Bitboard temp = bitboards[color][type];
			while (temp)
			{
				// Get index of the least significant bit set
				int sq = __builtin_ctzll(temp);
				pieceArray[sq] = type | colorMask;
				temp &= (temp - 1); // Clear the bit
			}
		}
	}

	// 4. Handle State Variables
	std::swap(whiteShortCastle, blackShortCastle);
	std::swap(whiteLongCastle, blackLongCastle);

	if (passantSquare < 64)
	{
		passantSquare ^= 56;
	}

	if (evenNumMove)
	{
		fullmoveCounter++;
	}
	evenNumMove = !evenNumMove;
}

BoardState Board::saveState()
{
	BoardState state;
	// Copy bitboards
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			state.bitboards[i][j] = bitboards[i][j];
		}
	}
	state.allCombined = allCombined;

	// Copy piece array
	for (int i = 0; i < 64; i++)
	{
		state.pieceArray[i] = pieceArray[i];
	}

	state.whiteShortCastle = whiteShortCastle;
	state.whiteLongCastle = whiteLongCastle;
	state.blackShortCastle = blackShortCastle;
	state.blackLongCastle = blackLongCastle;

	state.passantSquare = passantSquare;
	state.halfmoveClock = halfmoveClock;

	return state;
}