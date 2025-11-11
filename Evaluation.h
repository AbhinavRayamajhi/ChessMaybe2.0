#pragma once

#include "Helpers.h"
#include "MoveGen.h"
#include "Position.h"

namespace Engine
{
	enum PieceValue
	{
		PawnValue = 100,
		KnightValue = 300,
		BishopValue = 350,
		RookValue = 500,
		QueenValue = 800,
		KingValue = 2147483647
	};

	inline int evaluate(Position p)
	{
		int checkScore = 0;

		if (MoveGen::isKingInCheck(p.board, p.state.sideToMove))
			checkScore -= 100;

		int numWhitePawns{ 0 }, numWhiteKnights{ 0 }, numWhiteBishops{ 0 }, numWhiteRooks{ 0 }, numWhiteQueens{ 0 };
		int numBlackPawns{ 0 }, numBlackKnights{ 0 }, numBlackBishops{ 0 }, numBlackRooks{ 0 }, numBlackQueens{ 0 };

		while (p.board.pieces[WhitePawn])
		{
			clearLSB(p.board.pieces[WhitePawn]);
			numWhitePawns++;
		} 
		while (p.board.pieces[WhiteKnight])
		{
			clearLSB(p.board.pieces[WhiteKnight]);
			numWhiteKnights++;
		}
		while (p.board.pieces[WhiteBishop])
		{
			clearLSB(p.board.pieces[WhiteBishop]);
			numWhiteBishops++;
		}
		while (p.board.pieces[WhiteRook])
		{
			clearLSB(p.board.pieces[WhiteRook]);
			numWhiteRooks++;
		}
		while (p.board.pieces[WhiteQueen])
		{
			clearLSB(p.board.pieces[WhiteQueen]);
			numWhiteQueens++;
		}

		while (p.board.pieces[BlackPawn])
		{
			clearLSB(p.board.pieces[BlackPawn]);
			numBlackPawns++;
		}
		while (p.board.pieces[BlackKnight])
		{
			clearLSB(p.board.pieces[BlackKnight]);
			numBlackKnights++;
		}
		while (p.board.pieces[BlackBishop])
		{
			clearLSB(p.board.pieces[BlackBishop]);
			numBlackBishops++;
		}
		while (p.board.pieces[BlackRook])
		{
			clearLSB(p.board.pieces[BlackRook]);
			numBlackRooks++;
		}
		while (p.board.pieces[BlackQueen])
		{
			clearLSB(p.board.pieces[BlackQueen]);
			numBlackQueens++;
		}


		int whiteScore = numWhitePawns * PawnValue + numWhiteKnights * KnightValue + numWhiteBishops * BishopValue
			+ numWhiteRooks * RookValue + numWhiteQueens * QueenValue;
		int blackScore = numBlackPawns * PawnValue + numBlackKnights * KnightValue + numBlackBishops * BishopValue
			+ numBlackRooks * RookValue + numBlackQueens * QueenValue;

		return whiteScore - blackScore + checkScore;
	}
}