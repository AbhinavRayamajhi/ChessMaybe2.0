#pragma once

#include "Definitions.h"
#include "Evaluation.h"
#include "Move.h"
#include "MoveGen.h"

const int INF = 64000;

namespace Engine {
	
	inline int search(Board& board, int depth, int alpha, int beta, uint64_t& nodes) {

		if (depth == 0) {
			return evaluate(board);
		}

		MoveList moveList;
		generateLegalMoves(board, moveList);

		if (moveList.end == 0) {

			if (isKingInCheck(board)) {
				return -(INF + depth);
			}
			else {
				return 0;
			}
		}

		for (int i = 0; i < moveList.end; ++i) {

			// find best move
			int bestInd = i;
			for (int j = i + 1; j < moveList.end; ++j) {

				if (moveList.score[j] > moveList.score[bestInd]) {
					bestInd = j;
				}
			}

			moveList.swapMoveScore(i, bestInd);

			History h;
			makeMove(moveList.list[i], board, h);
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

	inline int getBestMove(Board& board, int depth, uint64_t& nodes, bool debug) {

		MoveList moveList;
		generateLegalMoves(board, moveList);

		int bestMove = moveList.list[0];
		int bestScore = INT32_MIN + 1;
		int alpha = -INF;
		int beta = INF;

		for (int i = 0; i < moveList.end; ++i) {

			// find best move
			int bestInd = i;
			for (int j = i + 1; j < moveList.end; ++j) {

				if (moveList.score[j] > moveList.score[bestInd]) {
					bestInd = j;
				}
			}

			moveList.swapMoveScore(i, bestInd);

			History h;
			makeMove(moveList.list[i], board, h);
			int currentScore = -search(board, depth - 1, alpha, beta, nodes);
			unmakeMove(board,  h);


			nodes++;
			if (currentScore > bestScore) {

				bestScore = currentScore;
				bestMove = moveList.list[i];
			}

			if (debug) {
				printMove(moveList.list[i]);
				std::cout << ": " << currentScore << std::endl;
			}
		}

		return bestMove;
	}
}