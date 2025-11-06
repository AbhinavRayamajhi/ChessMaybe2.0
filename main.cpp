
#include <chrono>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"

int main()
{

	int moveList[256]{ 0 };
	int* list = &moveList[0];

	std::string FEN = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ";
	State state = stateFromFEN(FEN);
	Board board = createBoard(FEN);
	Magic::computeSliderAttack();

	std::vector<Position> history;
	Position current(board, state);
	
	std::cout << state.sideToMove;

	printBoard(board);
	
	int depth = 6;

	auto start = std::chrono::steady_clock::now();
	std::cout << "Perft Test\n\n";
	uint64_t nodes = Test::perfTest(current, history, depth, depth);
	std::cout << "\nNodes: " << nodes << "\n";
	auto end = std::chrono::steady_clock::now();

	auto duration = end - start;

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0 << " s";
}

	
