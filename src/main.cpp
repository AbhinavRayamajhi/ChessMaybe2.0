
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"

int main() {
	
	std::string FEN = "5k2/4b1p1/2N1Q2p/p1p5/8/3P1P2/1P1P2PP/2B1R1K1 w - - 2 24";

	Engine::computeSliderAttack();
	auto start = std::chrono::steady_clock::now();

#if 0
	// search
	int depth = 7;
	uint64_t nodes = 0;

	while (true) {
		std::string input;
		std::cout << "\nEnter FEN(-1 to exit): ";
		std::getline(std::cin, input);

		if (input == "-1") break;

		Board board(input);
		int bestMove = Engine::getBestMove(board, depth, nodes, false);
		std::cout << "Best Move: ";
		printMove(bestMove);
		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif

#if 0
	// search
	int depth = 7;
	bool debug = false;
	uint64_t nodes = 0;

	Board board(FEN);
	int bestMove = Engine::getBestMove(board, depth, nodes, debug);
	std::cout << "Best Move: ";
	printMove(bestMove);
	std::cout << std::endl;
	
#endif

#if 1
	Engine::testSuite();
	uint64_t nodes = 1494874546;
#endif

#if 0
	int depth = 6;
	Board board(FEN);
	History history;

	//Move move1 = createMove(H7, H6, NO_PROMOTION, NORMAL);
	// Move move2 = createMove(A6, A5, NO_PROMOTION, NORMAL);

	//makeMove(move1, board, history);
	// makeMove(move2, board, history);

	board.printBoard();

	uint64_t nodes = Engine::perfTest(board, depth, depth);
	std::cout << "\n";

#endif

	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;

	
	std::cout << "Nodes: " << nodes << "\n";

	double time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
	std::cout << "Time: " << time << " s\n";

	std::cout << "Nodes per second: " << nodes / time << std::endl;
}

	
