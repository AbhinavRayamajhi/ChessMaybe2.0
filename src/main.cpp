
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
	Move moveList[256]{ 0 };
	Move* list = &moveList[0];

	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Engine::computeSliderAttack();
	auto start = std::chrono::steady_clock::now();

#if 0
	one.play(6);
#endif

#if 0
	// search
	Board board = createBoard(FEN);
	int depth = 7;
	int bestMove = Engine::getBestMove(current, depth);
	std::cout << "\nBest Move: ";
	printMove(bestMove);
#endif

#if 0
	Engine::testSuite();
#endif

#if 1
	int depth = 1;

	Board board = createBoard(FEN);

	std::cout << Engine::perfTest(board, depth, depth);
#endif

	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()/ 1000.0 << " s";
}

	
