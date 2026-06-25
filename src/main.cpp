
#include <chrono>
#include <string>
#include <fstream>
#include <iostream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"
#include "Search.h"
#include "UCI.h"

int main() {

	freopen("/home/abhin/ChessMaybe2.0/engine_debug.log", "w", stderr);
	std::cerr << "Engine started" << std::endl;
	Engine::computeSliderAttack();
	Engine::uciLoop();
	return 0;
}

	
