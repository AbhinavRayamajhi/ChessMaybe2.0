#pragma once

#include "Helpers.h"
#include "MoveGen.h"
#include "Position.h"

namespace Engine
{
	enum PieceValue {
		PawnValue = 100,
		KnightValue = 350,
		BishopValue = 350,
		RookValue = 525,
		QueenValue = 1000,
		KingValue = 2147483647
	};

	inline int evaluate(Board& board) {
		
		int numWhitePawns   { popCount(board.pieces[WHITE][PAWN]) },
			numWhiteKnights	{ popCount(board.pieces[WHITE][KNIGHT]) }, 
			numWhiteBishops	{ popCount(board.pieces[WHITE][BISHOP]) }, 
			numWhiteRooks	{ popCount(board.pieces[WHITE][ROOK]) }, 
			numWhiteQueens	{ popCount(board.pieces[WHITE][QUEEN]) },
			numBlackPawns	{ popCount(board.pieces[BLACK][PAWN]) }, 
			numBlackKnights	{ popCount(board.pieces[BLACK][KNIGHT]) }, 
			numBlackBishops	{ popCount(board.pieces[BLACK][BISHOP]) }, 
			numBlackRooks	{ popCount(board.pieces[BLACK][ROOK]) }, 
			numBlackQueens	{ popCount(board.pieces[BLACK][QUEEN]) };


		int whiteScore = numWhitePawns * PawnValue + numWhiteKnights * KnightValue + numWhiteBishops * BishopValue
			+ numWhiteRooks * RookValue + numWhiteQueens * QueenValue;
		int blackScore = numBlackPawns * PawnValue + numBlackKnights * KnightValue + numBlackBishops * BishopValue
			+ numBlackRooks * RookValue + numBlackQueens * QueenValue;

		if (board.pieces[WHITE][PAWN] & 0x0000001818000000)
			whiteScore += 20;
		if (getBit(board.pieces[WHITE][PAWN], A2))
			whiteScore += 5;
		if (getBit(board.pieces[WHITE][PAWN], B2))
			whiteScore += 5;
		if (getBit(board.pieces[WHITE][PAWN], C2))
			whiteScore += 5;
		if (getBit(board.pieces[WHITE][PAWN], F2))
			whiteScore += 5;
		if (getBit(board.pieces[WHITE][PAWN], G2))
			whiteScore += 5;
		if (getBit(board.pieces[WHITE][PAWN], H2))
			whiteScore += 5;

		if (board.pieces[BLACK][PAWN] & 0x0000001818000000)
			blackScore += 20;
		if (getBit(board.pieces[BLACK][PAWN], A7))
			blackScore += 5;
		if (getBit(board.pieces[BLACK][PAWN], B7))
			blackScore += 5;
		if (getBit(board.pieces[BLACK][PAWN], C7))
			blackScore += 5;
		if (getBit(board.pieces[BLACK][PAWN], F7))
			blackScore += 5;
		if (getBit(board.pieces[BLACK][PAWN], G7))
			blackScore += 5;
		if (getBit(board.pieces[BLACK][PAWN], H7))
			blackScore += 5;

		if (whiteScore > 2500)
		{
			if (getBit(board.pieces[WHITE][QUEEN], D1)) whiteScore += 50;
		}
		if (blackScore > 2500)
		{
			if (getBit(board.pieces[BLACK][QUEEN], D8)) blackScore += 50;
		}

		return whiteScore - blackScore;
	}
}