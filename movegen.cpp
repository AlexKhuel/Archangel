#include "movegen.h"
#include "board.h"
#include <iostream>
#include <bit>
#include <iostream>
#include <chrono>

uint64_t MoveGen::perft(Board &board, int depth, bool isRoot)
{
	// Base case: if we reached the target depth, this leaf counts as 1 position
	if (depth == 0)
	{
		return 1;
	}

	MoveList list;
	generateMoves(board, list);
	uint64_t totalNodes = 0;

	for (int i = 0; i < list.count; ++i)
	{
		Move move = list.moveList[i];

		board.makeMove(move);

		uint64_t branchNodes = perft(board, depth - 1, false);
		totalNodes += branchNodes;
		board.unmakeMove();

		if (isRoot)
		{
			char fromFile = 'a' + (move.getFrom() % 8);
			char fromRank = '1' + (move.getFrom() / 8);
			char toFile = 'a' + (move.getTo() % 8);
			char toRank = '1' + (move.getTo() / 8);

			// This should now print the large branchNodes value
			std::cout << fromFile << fromRank << toFile << toRank
					  << " " << branchNodes << std::endl;
		}
	}
	return totalNodes;
}

void MoveGen::generateMoves(Board &board, MoveList &list)
{
	Bitboard *friendlyPieces = board.isWhiteTurn ? board.bitboards[0] : board.bitboards[1];
	for (uint8_t i = 0; i < 64; i++)
	{
		Bitboard bitPos = 1ULL << i;

		if ((bitPos & friendlyPieces[0]) != 0)
		{
			int currPiece = board.pieceArray[i];

			switch (currPiece & 7)
			{
			case Piece::PAWN:
				pawnGen(board, i, bitPos, list);
				break;
			case Piece::KNIGHT:
				knightGen(board, i, list);
				break;
			case Piece::BISHOP:
				bishopGen(board, i, bitPos, list);
				break;
			case Piece::ROOK:
				rookGen(board, i, bitPos, list);
				break;
			case Piece::QUEEN:
				bishopGen(board, i, bitPos, list);
				rookGen(board, i, bitPos, list);
				break;
			case Piece::KING:
				kingGen(board, i, bitPos, list);
				break;
			default:
				std::cout << "You have reached the default case. This is bad." << std::endl;
				break;
			}
		}
	}
}

void MoveGen::pawnGen(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list)
{

	Bitboard opponentPieces = board.isWhiteTurn ? board.bitboards[1][0] : board.bitboards[0][0];

	if ((board.pieceArray[startPos] & Piece::COLOR_MASK) == Piece::WHITE)
	{
		if (startPos % 8 != 0 && ((bitPos << 7) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos + 7)))
		{
			if (startPos / 8 == 6)
			{
				list.add(Move(startPos, startPos + 7, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos + 7, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos + 7, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos + 7, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos + 7));
			}
		}
		if (startPos % 8 != 7 && ((bitPos << 9) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos + 9)))
		{
			if (startPos / 8 == 6)
			{
				list.add(Move(startPos, startPos + 9, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos + 9, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos + 9, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos + 9, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos + 9));
			}
		}
		if (((bitPos << 8) & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos + 8)))
		{
			if (startPos / 8 == 6)
			{
				list.add(Move(startPos, startPos + 8, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos + 8, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos + 8, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos + 8, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos + 8));
			}
		}
		if ((startPos / 8) == 1 && ((bitPos << 8) & board.allCombined) == 0 && (bitPos << 16 & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos + 16)))
		{
			list.add(Move(startPos, startPos + 16));
		}
		if (startPos % 8 != 0 && board.passantSquare == startPos + 7 && tryMove(board, Move(startPos, startPos + 7)))
		{
			list.add(Move(startPos, startPos + 7, Move::PASSANT));
		}
		if (startPos % 8 != 7 && board.passantSquare == startPos + 9 && tryMove(board, Move(startPos, startPos + 9)))
		{
			list.add(Move(startPos, startPos + 9, Move::PASSANT));
		}
	}
	else
	{
		if (startPos % 8 != 7 && ((bitPos >> 7) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos - 7)))
		{
			if (startPos / 8 == 1)
			{
				list.add(Move(startPos, startPos - 7, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos - 7, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos - 7, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos - 7, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos - 7));
			}
		}
		if (startPos % 8 != 0 && ((bitPos >> 9) & opponentPieces) != 0 && tryMove(board, Move(startPos, startPos - 9)))
		{
			if (startPos / 8 == 1)
			{
				list.add(Move(startPos, startPos - 9, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos - 9, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos - 9, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos - 9, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos - 9));
			}
		}
		if (((bitPos >> 8) & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos - 8)))
		{
			if (startPos / 8 == 1)
			{
				list.add(Move(startPos, startPos - 8, Move::PROMOTION, Move::KNIGHT));
				list.add(Move(startPos, startPos - 8, Move::PROMOTION, Move::BISHOP));
				list.add(Move(startPos, startPos - 8, Move::PROMOTION, Move::ROOK));
				list.add(Move(startPos, startPos - 8, Move::PROMOTION, Move::QUEEN));
			}
			else
			{
				list.add(Move(startPos, startPos - 8));
			}
		}
		if ((startPos / 8) == 6 && ((bitPos >> 8) & board.allCombined) == 0 && (bitPos >> 16 & board.allCombined) == 0 && tryMove(board, Move(startPos, startPos - 16)))
		{
			list.add(Move(startPos, startPos - 16));
		}
		if (startPos % 8 != 7 && board.passantSquare == startPos - 7 && tryMove(board, Move(startPos, startPos - 7)))
		{
			list.add(Move(startPos, startPos - 7, Move::PASSANT));
		}
		if (startPos % 8 != 0 && board.passantSquare == startPos - 9 && tryMove(board, Move(startPos, startPos - 9)))
		{
			list.add(Move(startPos, startPos - 9, Move::PASSANT));
		}
	}
}

void MoveGen::knightGen(Board &board, uint8_t startPos, MoveList &list)
{
	Bitboard friendlyPieces = board.isWhiteTurn ? board.bitboards[0][0] : board.bitboards[1][0];
	for (int i = 0; i < 8 && knightMoves[startPos][i] != 255; i++)
	{

		uint8_t endPos = knightMoves[startPos][i];
		if ((friendlyPieces & board.bitPositions[endPos]) == 0 && tryMove(board, Move(startPos, endPos)))
		{
			list.add(Move(startPos, endPos));
		}
	}
}

void MoveGen::bishopGen(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list)
{
	Bitboard friendlyPieces = board.isWhiteTurn ? board.bitboards[0][0] : board.bitboards[1][0];
	Bitboard opponentPieces = board.isWhiteTurn ? board.bitboards[1][0] : board.bitboards[0][0];
	for (uint8_t i = 4; i < 8; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[startPos][i] + 1; magnitude++)
		{
			int targetPos = startPos + directions[i] * magnitude;

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? bitPos >> std::abs(shift) : bitPos << std::abs(shift);

			if ((friendlyPieces & endBitPos) != 0)
			{
				break;
			}
			else if ((opponentPieces & endBitPos) != 0 && tryMove(board, Move(startPos, targetPos)))
			{
				list.add(Move(startPos, targetPos));
				break;
			}
			else if ((board.allCombined & endBitPos) == 0 && tryMove(board, Move(startPos, targetPos)))
			{
				list.add(Move(startPos, targetPos));
				continue;
			}
		}
	}
}

void MoveGen::rookGen(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list)
{
	Bitboard friendlyPieces = board.isWhiteTurn ? board.bitboards[0][0] : board.bitboards[1][0];
	Bitboard opponentPieces = board.isWhiteTurn ? board.bitboards[1][0] : board.bitboards[0][0];
	for (uint8_t i = 0; i < 4; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[startPos][i] + 1; magnitude++)
		{
			int targetPos = startPos + directions[i] * magnitude;

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? bitPos >> std::abs(shift) : bitPos << std::abs(shift);

			if ((friendlyPieces & endBitPos) != 0)
			{
				break;
			}
			else if ((opponentPieces & endBitPos) != 0 && tryMove(board, Move(startPos, targetPos)))
			{
				list.add(Move(startPos, targetPos));
				break;
			}
			else if ((board.allCombined & endBitPos) == 0 && tryMove(board, Move(startPos, targetPos)))
			{
				list.add(Move(startPos, targetPos));
				continue;
			}
		}
	}
}

void MoveGen::kingGen(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list)
{
	Bitboard friendlyPieces = board.isWhiteTurn ? board.bitboards[0][0] : board.bitboards[1][0];
	Bitboard opponentPieces = board.isWhiteTurn ? board.bitboards[1][0] : board.bitboards[0][0];

	for (int i = 0; i < 8 && kingMoves[startPos][i] != 255; i++)
	{
		uint8_t endPos = kingMoves[startPos][i];

		if ((friendlyPieces & board.bitPositions[endPos]) == 0 && tryMove(board, Move(startPos, endPos)))
		{
			list.add(Move(startPos, endPos));
		}
	}

	// Handles castling
	if ((board.pieceArray[startPos] & Piece::COLOR_MASK) == Piece::WHITE)
	{
		if (board.whiteShortCastle &&
			(board.allCombined & whiteShortEmpty) == 0 &&
			!isAttacked(board, 4) && !isAttacked(board, 5) && !isAttacked(board, 6) &&
			tryMove(board, Move(4, 6, Move::CASTLING)))
		{
			list.add(Move(4, 6, Move::CASTLING));
		}

		if (board.whiteLongCastle &&
			(board.allCombined & whiteLongEmpty) == 0 &&
			!isAttacked(board, 4) && !isAttacked(board, 2) && !isAttacked(board, 3) &&
			tryMove(board, Move(4, 2, Move::CASTLING)))
		{
			list.add(Move(4, 2, Move::CASTLING));
		}
	}
	else
	{
		if (board.blackShortCastle &&
			(board.allCombined & blackShortEmpty) == 0 &&
			!isAttacked(board, 60) && !isAttacked(board, 61) && !isAttacked(board, 62) &&
			tryMove(board, Move(60, 62, Move::CASTLING)))
		{
			list.add(Move(60, 62, Move::CASTLING));
		}

		if (board.blackLongCastle &&
			(board.allCombined & blackLongEmpty) == 0 &&
			!isAttacked(board, 60) && !isAttacked(board, 59) && !isAttacked(board, 58) &&
			tryMove(board, Move(60, 58, Move::CASTLING)))
		{
			list.add(Move(60, 58, Move::CASTLING));
		}
	}
}

bool MoveGen::isAttacked(Board &board, uint8_t targetSquare)
{
	Bitboard *friendlyPieces = !board.isWhiteTurn ? board.bitboards[0] : board.bitboards[1];
	Bitboard *opponentPieces = !board.isWhiteTurn ? board.bitboards[1] : board.bitboards[0];

	for (uint8_t i = 4; i < 8; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[targetSquare][i] + 1; magnitude++)
		{

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? board.bitPositions[targetSquare] >> std::abs(shift) : board.bitPositions[targetSquare] << std::abs(shift);

			if ((friendlyPieces[0] & endBitPos) != 0)
			{
				break;
			}
			else if ((opponentPieces[0] & endBitPos) != 0 && ((opponentPieces[Piece::BISHOP] & endBitPos) != 0 || (opponentPieces[Piece::QUEEN] & endBitPos) != 0))
			{
				return true;
			}
		}
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[targetSquare][i] + 1; magnitude++)
		{

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? board.bitPositions[targetSquare] >> std::abs(shift) : board.bitPositions[targetSquare] << std::abs(shift);

			if ((friendlyPieces[0] & endBitPos) != 0)
			{
				break;
			}
			else if ((friendlyPieces[0] & endBitPos) != 0 && ((opponentPieces[Piece::ROOK] & endBitPos) != 0 || (opponentPieces[Piece::QUEEN] & endBitPos) != 0))
			{
				return true;
			}
		}
	}

	for (int i = 0; i < 8 && knightMoves[targetSquare][i] != 255; i++)
	{
		uint8_t endPos = knightMoves[targetSquare][i];
		if ((opponentPieces[Piece::KNIGHT] & board.bitPositions[endPos]) != 0)
		{
			return true;
		}
	}
	return false;
}

bool MoveGen::tryMove(Board &board, Move testMove)
{
	Bitboard friendlyKing = board.isWhiteTurn ? board.bitboards[0][Piece::KING] : board.bitboards[1][Piece::KING];

	board.makeMove(testMove);
	bool legal = !isAttacked(board, std::countr_zero(friendlyKing));
	board.unmakeMove();
	return legal;
}