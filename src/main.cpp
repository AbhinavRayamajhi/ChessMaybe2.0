
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"

int main()
{
	std::string FEN = "r3k2r/Pppp1ppp/1b3nbN/nPP5/4P3/B4N2/P2P2PP/3B1RK1 b kq - 0 3";

	Engine::computeSliderAttack();
	auto start = std::chrono::steady_clock::now();

#if 0
	one.play(6);
#endif

#if 1
	// search
	int depth = 6;
	int nodes = 0;

	while (true) {
		std::string input;
		std::cout << "\nEnter FEN(-1 to exit): ";
		std::getline(std::cin, input);

		if (input == "-1") break;

		Board board(input);
		int bestMove = Engine::getBestMove(board, depth, nodes);
		std::cout << "Best Move: ";
		printMove(bestMove);
		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif

#if 0
	Engine::testSuite();
	int nodes = 1494874546;
#endif

#if 0
	int depth = 2;
	Board board(FEN);
	History history;

	Move move1 = createMove(H7, H6, NO_PROMOTION, NORMAL);
	// Move move2 = createMove(A6, A5, NO_PROMOTION, NORMAL);

	makeMove(move1, board, history);
	// makeMove(move2, board, history);

	int nodes = Engine::perfTest(board, depth, depth);
#endif

	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;

	
	std::cout << "\nNodes: " << nodes << "\n";

	double time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
	std::cout << "Time: " << time << " s\n";

	std::cout << "Nodes per second: " << nodes / time << std::endl;
}

	
