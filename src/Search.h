#pragma once

#include <chrono>

#include "Definitions.h"
#include "Evaluation.h"
#include "Move.h"
#include "MoveGen.h"

const int INF = 64000;
const int QUIESCENCE_DEPTH = 4;

namespace Engine {

	inline int quiescenceSearch(Position& position, int alpha, int beta, int depth) {

		Board b = position.getBoard();
		if (depth == 0) return evaluate(b);

		MoveList moveList;

		if (isKingInCheck(position)) {

			generateLegalMoves<SCORE_GEN_TRUE, CAPTURE_FALSE>(position, moveList);

			if (moveList.end == 0) {

				return -(INF + QUIESCENCE_DEPTH -depth);
			}
		}
		else {

			int standPat = evaluate(b);
			if (standPat >= beta) return beta;
			if (standPat > alpha) alpha = standPat;
			generateLegalMoves<SCORE_GEN_TRUE, CAPTURE_TRUE>(position, moveList);
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
			makeMove(moveList.list[i], position, h);
			int score = -quiescenceSearch(position, -beta, -alpha, depth - 1);
			unmakeMove(position, h);

			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		}

		return alpha;
	}

	inline int search(Position& position, int depth, int alpha, int beta, uint64_t& nodes, std::chrono::steady_clock::time_point stop, bool& stopped) {

		if (depth == 0) {
			return quiescenceSearch(position, alpha, beta, QUIESCENCE_DEPTH);
		}

		// Mate distance pruning
		alpha = std::max(alpha, -(INF + depth));   
		beta = std::min(beta, INF + depth);        

		if (alpha >= beta) return alpha;            

		MoveList moveList;
		generateLegalMoves <SCORE_GEN_TRUE, CAPTURE_FALSE>(position, moveList);

		if (moveList.end == 0) {

			if (isKingInCheck(position)) {
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
			makeMove(moveList.list[i], position, h);
			int score = -search(position, depth - 1, -beta, -alpha, nodes, stop, stopped);
			if (stopped) return 0;
			unmakeMove(position, h);

			nodes++;
			if ((nodes & 2047) == 0) {
				if (std::chrono::steady_clock::now() >= stop) {
					stopped = true;
					return 0;
				}
			}

			if (score >= beta) {
				return beta;
			}
			if (score > alpha) {
				alpha = score;
			}
		}

		return alpha;
	}

	inline int getBestMove(Position& position, const int MAX_DEPTH, uint64_t& nodes, int time, bool debug) {

		auto stop = std::chrono::steady_clock::now() + std::chrono::milliseconds(time);
		bool stopped = false;

		MoveList moveList;
		generateLegalMoves<SCORE_GEN_TRUE, CAPTURE_FALSE>(position, moveList);


		if (moveList.end == 0) {
			// no legal moves - checkmate or stalemate
			std::cout << "bestmove 0000\n" << std::flush;
			return 0;
		}

		int bestMove = moveList.list[0];
		int beta = INT32_MAX;

		for (int depth = 1; depth < MAX_DEPTH; depth++) {

			int bestScore = INT32_MIN + 1;

			for (int i = 0; i < moveList.end; ++i) {

				// find best move is selection sort so should only start from index 1 as index 0 is left for best move
				if (i != 0) {

					int bestInd = i;
					for (int j = i + 1; j < moveList.end; ++j) {

						if (moveList.score[j] > moveList.score[bestInd]) {
							bestInd = j;
						}
					}
					moveList.swapMoveScore(i, bestInd);
				}

				History h;
				makeMove(moveList.list[i], position, h);
				int currentScore = -search(position, depth - 1, -beta, -bestScore, nodes, stop, stopped);
				if (stopped) return bestMove;
				unmakeMove(position, h);


				nodes++;
				if (currentScore > bestScore) {

					bestScore = currentScore;
					bestMove = moveList.list[i];
				}


				// if (debug) {
				// 	printMove(moveList.list[i]);
				// 	std::cout << ": " << currentScore << std::endl;
				// }
			}

			if (bestMove != moveList.list[0]) {

				// swap everything with first index till we reach the target index with best move and swap one last time with first index
				// preserves order for the rest of the moves and brings bestMove to the front to be analyzed first for deeper searches
				int index = 1;
				while (moveList.list[index] != bestMove) {
					moveList.swapMoveScore(index, 0);
					++index;
				}
				moveList.swapMoveScore(index, 0);
			}
		}

		return bestMove;
	}
}