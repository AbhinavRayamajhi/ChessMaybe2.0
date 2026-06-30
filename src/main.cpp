
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

	//Engine::uciLoop();
	//Engine::testSuite();
#if 1
	// search
	int depth = 3;
	bool debug = true;
	uint64_t nodes = 0;

	Position position;
	position.setBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	int bestMove = Engine::getBestMove(position, depth, nodes, debug);
	std::cout << "Best Move: ";
	printMove(bestMove);
	std::cout << std::endl;

#endif
	return 0;
}

	
