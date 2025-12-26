#include "movegen.h"
#include "board.h"
#include <iostream>
#include <bit>
#include <iostream>
#include <chrono>

uint64_t MoveGen::perft(Board &board, int depth)
{ // PASS BY VALUE (Copy the board)
	if (depth == 0)
		return 1;

	MoveList list;
	generateMoves(board, list);
	uint64_t nodes = 0;

	for (int i = 0; i < list.count; i++)
	{
		Board nextBoard = board; // Create a fresh copy of the current state
		nextBoard.makeMove(list.moveList[i]);
		// nextBoard.makeMove already calls nextTurn(), so perspective is flipped

		nodes += perft(nextBoard, depth - 1);
	}
	return nodes;
}

void MoveGen::generateMoves(Board &board, MoveList &list)
{
	for (uint8_t i = 0; i < 64; i++)
	{
		Bitboard bitPos = 1ULL << i;

		if ((bitPos & board.bitboards[0][0]) != 0)
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

	Bitboard opponentPieces = board.bitboards[1][0];

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

void MoveGen::knightGen(Board &board, uint8_t startPos, MoveList &list)
{
	for (int i = 0; i < 8 && knightMoves[startPos][i] != 255; i++)
	{
		uint8_t endPos = knightMoves[startPos][i];
		if ((board.bitboards[0][0] & board.bitPositions[endPos]) == 0 && tryMove(board, Move(startPos, endPos)))
		{
			list.add(Move(startPos, endPos));
		}
	}
}

void MoveGen::bishopGen(Board &board, uint8_t startPos, Bitboard bitPos, MoveList &list)
{
	for (uint8_t i = 4; i < 8; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[startPos][i] + 1; magnitude++)
		{
			int targetPos = startPos + directions[i] * magnitude;

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? bitPos >> std::abs(shift) : bitPos << std::abs(shift);

			if ((board.bitboards[0][0] & endBitPos) != 0)
			{
				break;
			}
			else if ((board.bitboards[1][0] & endBitPos) != 0 && tryMove(board, Move(startPos, targetPos)))
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
	for (uint8_t i = 0; i < 4; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[startPos][i] + 1; magnitude++)
		{
			int targetPos = startPos + directions[i] * magnitude;

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? bitPos >> std::abs(shift) : bitPos << std::abs(shift);

			if ((board.bitboards[0][0] & endBitPos) != 0)
			{
				break;
			}
			else if ((board.bitboards[1][0] & endBitPos) != 0 && tryMove(board, Move(startPos, targetPos)))
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
	// Handles 1 square movement
	for (uint8_t i = 0; i < 8; i++)
	{
		if (disToEdge[startPos][i] == 0)
		{
			continue;
		}

		Bitboard endBitPos = i % 2 == 0 ? bitPos >> std::abs(directions[i]) : bitPos << std::abs(directions[i]);

		if ((board.bitboards[0][0] & endBitPos) == 0 && tryMove(board, Move(startPos, startPos + directions[i])))
		{
			list.add(Move(startPos, startPos + directions[i]));
		}
	}

	// Handles short castling
	if (board.whiteShortCastle &&
		(board.allCombined & board.bitPositions[5]) == 0 &&
		(board.allCombined & board.bitPositions[6]) == 0 &&
		!isAttacked(board, 4) && !isAttacked(board, 5) && !isAttacked(board, 6) &&
		tryMove(board, Move(4, 6, Move::CASTLING)))
	{
		list.add(Move(4, 6, Move::CASTLING));
	}

	// Handles long castling
	if (board.whiteLongCastle &&
		(board.allCombined & board.bitPositions[1]) == 0 &&
		(board.allCombined & board.bitPositions[2]) == 0 &&
		(board.allCombined & board.bitPositions[3]) == 0 &&
		!isAttacked(board, 4) && !isAttacked(board, 2) && !isAttacked(board, 3) &&
		tryMove(board, Move(4, 2, Move::CASTLING)))
	{
		list.add(Move(4, 2, Move::CASTLING));
	}
}

bool MoveGen::isAttacked(Board &board, uint8_t targetSquare)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		for (int magnitude = 1; magnitude < disToEdge[targetSquare][i] + 1; magnitude++)
		{

			int shift = directions[i] * magnitude;
			Bitboard endBitPos = i % 2 == 0 ? board.bitPositions[targetSquare] >> std::abs(shift) : board.bitPositions[targetSquare] << std::abs(shift);

			if ((board.bitboards[0][0] & endBitPos) != 0)
			{
				break;
			}
			else if ((board.bitboards[1][0] & endBitPos) != 0 || (board.allCombined & endBitPos) == 0)
			{
				return true;
			}
		}
	}

	for (int i = 0; i < 8 && knightMoves[targetSquare][i] != 255; i++)
	{
		uint8_t endPos = knightMoves[targetSquare][i];
		if ((board.bitboards[0][0] & board.bitPositions[endPos]) == 0)
		{
			return true;
		}
	}
	return false;
}

bool MoveGen::tryMove(Board &board, Move testMove)
{
	return true;
	/*
	 BoardState oldState = board.saveState();
	 board.makeMove(testMove);
	 bool legal = !isAttacked(board, std::countr_zero(board.bitboards[0][Piece::KING]));
	 board.unmakeMove(oldState);
	 return legal;
	 */
}