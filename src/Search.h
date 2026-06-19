#pragma once

#include "Definitions.h"
#include "Evaluation.h"
#include "Move.h"
#include "MoveGen.h"

const int INF = 64000;

namespace Engine {
	
	inline int search(Board board, int depth, int alpha, int beta,int& nodes) {

		if (depth == 0) {
			return evaluate(board);
		}

		Move moveList[256]{ 0 };
		generateLegalMoves(board, moveList);

		if (moveList[0] == 0) {

			if (isKingInCheck(board)) {
				return -(INF + depth);
			}
			else {
				return 0;
			}
		}

		for (int i = 0; moveList[i] != 0; ++i) {

			History h;
			makeMove(moveList[i], board, h);
			int score = -search(board, depth - 1, -beta, -alpha, nodes);
			unmakeMove(board, h);

			nodes++;

			if (score >= beta) {
				return beta;
			}
			if (score > alpha) {
				alpha = score;
			}
		}

		return alpha;
	}

	inline int getBestMove(Board board, int depth, int& nodes) {

		Move moveList[256]{ 0 };
		generateLegalMoves(board, moveList);

		int bestMove = moveList[0];
		int bestScore = INT32_MIN + 1;
		int alpha = -INF;
		int beta = INF;

		for (int i = 0; moveList[i] != 0; ++i) {
			
			History h;
			makeMove(moveList[i], board, h);
			int currentScore = -search(board, depth - 1, alpha, beta, nodes);
			unmakeMove(board,  h);


			nodes++;
			if (currentScore > bestScore) {

				bestScore = currentScore;
				bestMove = moveList[i];
			}
		}

		return bestMove;
	}
}