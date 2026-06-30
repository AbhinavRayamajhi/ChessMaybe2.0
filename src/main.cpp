
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
	//Engine::testSuite();
	
#if 0
	// search
	int depth = 2;
	bool debug = true;

	uint64_t nodes = 0;
	History h;

	Position position;
	position.setBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1");

	nodes = Engine::perfTest(position, depth, depth, debug);
	std::cout << "Total nodes: " << nodes;
	std::cout << std::endl;

#endif
	return 0;
}

	
