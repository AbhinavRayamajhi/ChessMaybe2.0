#pragma once

#include "Definitions.h"
#include "Evaluation.h"
#include "Move.h"
#include "MoveGen.h"

const int INF = 64000;

namespace Engine {
	
	inline int search(Board board, int depth, int alpha, int beta) {

		if (depth == 0) {
			return evaluate(board);
		}

		int bestScore = board.sideToMove == WHITE ? -65000 : 65000;

		Move moveList[256]{ 0 };
		generateLegalMoves(board, moveList);

		if (moveList[0] == 0) {

			if (isKingInCheck(board)) {
				return board.sideToMove == WHITE ? -(INF + depth) : INF + depth;
			}
			else {
				return 0;
			}
		}

		for (int i = 0; moveList[i] != 0; ++i) {

			History h;
			makeMove(moveList[i], board, h);
			int currentScore = search(board, depth - 1, alpha, beta);
			unmakeMove(board, h);

			if (board.sideToMove == WHITE) {
				if (currentScore > bestScore) bestScore = currentScore;
				if (currentScore > alpha) alpha = currentScore;
			}
			else if (board.sideToMove == BLACK) {
				if (currentScore < bestScore) bestScore = currentScore;
				if (currentScore < beta) beta = currentScore;
			}
			if (alpha >= beta) break;
		}

		return bestScore;
	}

	inline int getBestMove(Board board, int depth) {

		Move moveList[256]{ 0 };
		generateLegalMoves(board, moveList);

		int bestMove = moveList[0];
		int bestScore = board.sideToMove ? -65000 : 65000;
		int alpha = -INF;
		int beta = INF;

		for (int i = 0; moveList[i] != 0; ++i)
		{
			History h;
			makeMove(moveList[i], board, h);
			int currentScore = search(board, depth - 1, alpha, beta);
			unmakeMove(board,  h);

			if (board.sideToMove == WHITE) {

				if (currentScore > bestScore) {

					bestScore = currentScore;
					bestMove = moveList[i];
				}
				if (currentScore > alpha) alpha = currentScore;
			}
			else if (board.sideToMove == BLACK)
			{
				if (currentScore < bestScore) 
				{
					bestScore = currentScore;
					bestMove = moveList[i];
				}
				if (currentScore < beta) beta = currentScore;
			}
		}

		return bestMove;
	}
}