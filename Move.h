#pragma once

#include <iostream>
#include <array>
#include "Definitions.h"
#include "Helpers.h"

// encoding move information into 32-bit int: 0-5 current, 6-11 target, 12-14 piece, 15-17 promotion,
// 18 capture, 19 double push, 20 enPassant, 21 castling, 22-31 unused
constexpr inline int createMove(int current, int target, int piece, int promotion, bool capture,
	bool doublePush, bool enPass, bool castling)
{
	current &= 0b111111;
	target &= 0b111111;
	return current | target << 6 | piece << 12 | promotion << 15 | capture << 18 | doublePush << 19
		| enPass << 20 | castling << 21;
}

// extracting info from move integer
constexpr inline int  getCurrent(int move)			{ return move & 0b111111UL; }
constexpr inline int  getTarget(int move)			{ return move >>  6 & 0b111111UL; }
constexpr inline int  getPiece(int move)			{ return move >> 12 & 0b111UL; }
constexpr inline int  getPromotion(int move)		{ return move >> 15 & 0b111UL; }
constexpr inline bool getIsCapture(int move)		{ return move >> 18 & 0b1UL; }
constexpr inline bool getIsDoublePush(int move)		{ return move >> 19 & 0b1UL; }
constexpr inline bool getIsEnPassant(int move)		{ return move >> 20 & 0b1UL; }
constexpr inline bool getIsCastling(int move)	    { return move >> 21 & 0b1UL; }
	

inline void printMove(int move)
{
	std::cout << squareToString(getCurrent(move)) << squareToString(getTarget(move));

	if (getPromotion(move))
	{
		std::cout << intToPieceString[getPromotion(move)];
	}
}

inline void printMoveList(int moveList[])
{
	std::cout << "Possible Moves: " << '\n';
	for (int i = 0; i < 256; ++i)
	{
		if (moveList[i] == 0)
		{
			break;
		}
		printMove(moveList[i]);
		std::cout << ", ";
	}
	std::cout << "\n\n";
}