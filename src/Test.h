#pragma once

#include <iostream>
#include "Move.h"
#include "MoveGen.h"
#include "Position.h"

namespace Engine {

	inline uint64_t perfTest(Board& board, int depth, int rootDepth = -1, bool debug = false) {

		// if (depth == 0) return 1ULL;

		uint64_t nodes = 0;
		MoveList moveList;

		generateLegalMoves<false>(board, moveList);

		if (depth == 1) return moveList.end;

		for (int i = 0; i < moveList.end; ++i) {

			History history;
			// make move calculate deeper and unmake
			makeMove(moveList.list[i], board, history);
			uint64_t node = perfTest(board, depth - 1, rootDepth, debug);
			unmakeMove(board, history);

			// print out nodes after each move from the start position
			if (rootDepth == depth) {
				//board.printBoard();
				printMove(moveList.list[i]);
				std::cout << " : " << node << '\n';
			}
			if (debug && ((rootDepth - 1) == depth)) {
				std::cout << "\t";
				printMove(moveList.list[i]);
				std::cout << " : " << node << '\n';
			}
			nodes += node;
		}
		return nodes;
	}

	inline void testSuite() {
		
		Board board = Board(std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
		std::cout << "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 20) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 400) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 8902) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 197281) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 4865609) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 6 " << ((perfTest(board, 6) == 119060324) ? "Passed\n\n" : "Failed\n\n");

		board = Board(std::string("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));
		std::cout << "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 48) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 2039) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 97862) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 4085603) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 193690690) ? "Passed\n\n" : "Failed\n\n");

		board = Board(std::string("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"));
		std::cout << "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 14) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 191) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 2812) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 43238) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 674624) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 6 " << ((perfTest(board, 6) == 11030083) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 7 " << ((perfTest(board, 7) == 178633661) ? "Passed\n\n" : "Failed\n\n");

		board = Board(std::string("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"));
		std::cout << "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 6) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 264) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 9467) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 422333) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 15833292) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 6 " << ((perfTest(board, 6) == 706045033) ? "Passed\n\n" : "Failed\n\n");

		board = Board(std::string("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"));
		std::cout << "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 44) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 1486) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 62379) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 2103487) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 89941194) ? "Passed\n\n" : "Failed\n\n");
		
		board = Board(std::string("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"));
		std::cout << "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10\n\n";
		std::cout << "Depth 1 " << ((perfTest(board, 1) == 46) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 2 " << ((perfTest(board, 2) == 2079) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 3 " << ((perfTest(board, 3) == 89890) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 4 " << ((perfTest(board, 4) == 3894594) ? "Passed\n" : "Failed\n");
		std::cout << "Depth 5 " << ((perfTest(board, 5) == 164075551) ? "Passed\n\n" : "Failed\n\n");
	}
}