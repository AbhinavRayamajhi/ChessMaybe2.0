
#include <chrono>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"
#include "Player.h"

int main()
{
	// initialization
	int moveList[256]{ 0 };
	int* list = &moveList[0];

	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Player one(FEN, Black);
	Magic::computeSliderAttack();
	auto start = std::chrono::steady_clock::now();

#if 1
	one.play(6);
#endif

#if 0
	// search
	Position current(FEN);
	int depth = 7;
	int bestMove = Engine::getBestMove(current, depth);
	std::cout << "\nBest Move: ";
	printMove(bestMove);
#endif

#if 0
	Test:testSuite();
#endif

#if 0
	int depth = 7;
	
	int m = createMove(B4, A4, Rook, None, 0, 0, 0, 0);
	makeMove(m, current.board, current.state, history);
	
	m = createMove(D6, D5, Pawn, None, 0, 0, 0, 0);
	makeMove(m, current.board, current.state, history);
	
	m = createMove(A5, B4, King, None, 0, 0, 0, 0);
	makeMove(m, current.board, current.state, history);

	m = createMove(H5, H8, Rook, None, 0, 1, 0, 0);
	makeMove(m, current.board, current.state, history);

	m = createMove(A4, A6, Rook, None, 0, 0, 0, 0);
	makeMove(m, current.board, current.state, history);

	m = createMove(C7, C5, Pawn, None, 0, 1, 0, 0);
	makeMove(m, current.board, current.state, history);
	
	std::cout << Test::perfTest(current, history, depth, depth);
#endif

	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()/ 1000.0 << " s";
}

	
