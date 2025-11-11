
#include <chrono>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"

int main()
{
	// initialization
	int moveList[256]{ 0 };
	int* list = &moveList[0];

	std::string FEN = "rnbqkbnr/1ppppppp/p7/8/3P4/2N5/PPP1PPPP/R1BQKBNR b KQkq - 1 2";

	Position current(FEN);
	std::vector<Position> history;
	Magic::computeSliderAttack();

	printBoard(current.board);
	
	// search
	int depth = 7;

	auto start = std::chrono::steady_clock::now();

	int bestMove = Engine::getBestMove(current, depth);
	std::cout << "\nBest Move: ";
	printMove(bestMove);

	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;
	std::cout << '\n' << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms";
}

	
