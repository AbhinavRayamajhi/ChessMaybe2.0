#pragma once

#include "Definitions.h"
#include "Evaluation.h"
#include "Move.h"
#include "MoveGen.h"

const int INF = 64000;

namespace Engine
{
	inline int search(Position p, int depth, int alpha, int beta)
	{
		if (depth == 0) return evaluate(p);

		int bestScore = p.state.sideToMove? -INF : INF;
		std::vector<Position> history;

		int moveList[256]{ 0 };
		MoveGen::generateLegalMoves(p.board, moveList, p.state);

		if (MoveGen::isKingInCheck(p.board, p.state.sideToMove) && moveList[0] == 0)
			return p.state.sideToMove ? -INF: INF;

		for (int i = 0; moveList[i] != 0; ++i)
		{
			makeMove(moveList[i], p.board, p.state, history);
			int currentScore = search(p, depth - 1, alpha, beta);
			unmakeMove(p.board, p.state, history);

			if (p.state.sideToMove == White)
			{
				if (currentScore > bestScore) bestScore = currentScore;
				if (currentScore > alpha) alpha = currentScore;
			}
			else if (p.state.sideToMove == Black)
			{
				if (currentScore < bestScore) bestScore = currentScore;
				if (currentScore < beta) beta = currentScore;
			}

			if (alpha >= beta) break;
		}

		return bestScore;
	}

	inline int getBestMove(Position p, int depth)
	{
		std::vector<Position> history;

		int moveList[256]{ 0 };
		MoveGen::generateLegalMoves(p.board, moveList, p.state);

		int bestMove = moveList[0];
		int bestScore = 0;
		int alpha = -INF;
		int beta = INF;

		for (int i = 0; moveList[i] != 0; ++i)
		{
			makeMove(moveList[i], p.board, p.state, history);
			int currentScore = search(p, depth - 1, alpha, beta);
			unmakeMove(p.board, p.state, history);

			if (p.state.sideToMove == White)
			{
				if (currentScore > bestScore)
				{
					bestScore = currentScore;
					bestMove = moveList[i];
				}
				if (currentScore > alpha) alpha = currentScore;
			}
			else if (p.state.sideToMove == Black)
			{
				if (currentScore < bestScore) 
				{
					bestScore = currentScore;
					bestMove = moveList[i];
				}
				if (currentScore < beta) beta = currentScore;
			}

			if (alpha >= beta) break;
		}

		return bestMove;
	}
}