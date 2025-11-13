#pragma once

#include "Helpers.h"
#include "MoveGen.h"
#include "Position.h"

namespace Engine
{
	enum PieceValue
	{
		PawnValue = 100,
		KnightValue = 350,
		BishopValue = 350,
		RookValue = 525,
		QueenValue = 1000,
		KingValue = 2147483647
	};

	inline int evaluate(Position p)
	{
		int numWhitePawns	{ popCount(p.board.pieces[WhitePawn]) },
			numWhiteKnights	{ popCount(p.board.pieces[WhiteKnight]) }, 
			numWhiteBishops	{ popCount(p.board.pieces[WhiteBishop]) }, 
			numWhiteRooks	{ popCount(p.board.pieces[WhiteRook]) }, 
			numWhiteQueens	{ popCount(p.board.pieces[WhiteQueen]) },
			numBlackPawns	{ popCount(p.board.pieces[BlackPawn]) }, 
			numBlackKnights	{ popCount(p.board.pieces[BlackKnight]) }, 
			numBlackBishops	{ popCount(p.board.pieces[BlackBishop]) }, 
			numBlackRooks	{ popCount(p.board.pieces[BlackRook]) }, 
			numBlackQueens	{ popCount(p.board.pieces[BlackQueen]) };


		int whiteScore = numWhitePawns * PawnValue + numWhiteKnights * KnightValue + numWhiteBishops * BishopValue
			+ numWhiteRooks * RookValue + numWhiteQueens * QueenValue;
		int blackScore = numBlackPawns * PawnValue + numBlackKnights * KnightValue + numBlackBishops * BishopValue
			+ numBlackRooks * RookValue + numBlackQueens * QueenValue;

		if (p.board.pieces[WhitePawn] & 0x0000001818000000)
			whiteScore += 20;
		if (getBit(p.board.pieces[WhitePawn], A2))
			whiteScore += 5;
		if (getBit(p.board.pieces[WhitePawn], B2))
			whiteScore += 5;
		if (getBit(p.board.pieces[WhitePawn], C2))
			whiteScore += 5;
		if (getBit(p.board.pieces[WhitePawn], F2))
			whiteScore += 5;
		if (getBit(p.board.pieces[WhitePawn], G2))
			whiteScore += 5;
		if (getBit(p.board.pieces[WhitePawn], H2))
			whiteScore += 5;

		if (p.board.pieces[BlackPawn] & 0x0000001818000000)
			blackScore += 20;
		if (getBit(p.board.pieces[BlackPawn], A7))
			blackScore += 5;
		if (getBit(p.board.pieces[BlackPawn], B7))
			blackScore += 5;
		if (getBit(p.board.pieces[BlackPawn], C7))
			blackScore += 5;
		if (getBit(p.board.pieces[BlackPawn], F7))
			blackScore += 5;
		if (getBit(p.board.pieces[BlackPawn], G7))
			blackScore += 5;
		if (getBit(p.board.pieces[BlackPawn], H7))
			blackScore += 5;

		if (whiteScore > 2500)
		{
			if (getBit(p.board.pieces[WhiteQueen], D1)) whiteScore += 50;
		}
		if (blackScore > 2500)
		{
			if (getBit(p.board.pieces[BlackQueen], D8)) blackScore += 50;
		}

		return whiteScore - blackScore;
	}
}