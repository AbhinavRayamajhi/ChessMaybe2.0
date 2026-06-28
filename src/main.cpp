
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>

#include "Magic.h"
#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"
#include "UCI.h"

int main() {

	Engine::initMagics();

	Engine::uciLoop();
	// Engine::testSuite();
#if 0
	// search
	int depth = 7;
	bool debug = false;
	uint64_t nodes = 0;

	Position position;
	position.board = Board("5rk1/pp3pp1/8/8/8/6K1/4r2n/4r3 b - - 3 45");
	int bestMove = Engine::getBestMove(position, depth, nodes, debug);
	std::cout << "Best Move: ";
	printMove(bestMove);
	std::cout << std::endl;

#endif
	return 0;
}

	
