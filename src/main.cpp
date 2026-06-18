
#include <chrono>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"

int main()
{
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Engine::computeSliderAttack();
	auto start = std::chrono::steady_clock::now();

#if 0
	one.play(6);
#endif

#if 0
	// search
	int depth = 3;
	Board board = createBoard(FEN);
	int bestMove = Engine::getBestMove(board, depth);
	std::cout << "\nBest Move: ";
	printMove(bestMove);
	std::cout << std::endl;
#endif

#if 1
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

	
