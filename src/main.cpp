
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

	Engine::initMagics();
	Engine::uciLoop();
	return 0;
}

	
