#include <fstream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"

int main()
{

	int moveList[256]{ 0 };
	int* list = &moveList[0];

	std::string FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 9";
	State state = stateFromFEN(FEN);
	Board board = createBoard(FEN);
	Magic::computeSliderAttack();
	
	std::vector<Position> history;
	Position start(board, state);
	history.push_back(start);

	printBoard(board);

	//list = MoveGen::generateLegalMoves(board, list, state);
	//printMoveList(moveList);

	
	int depth = 2;

	std::cout << "Perft Test\n\n";
	uint64_t nodes = Test::perfTest(start, history, depth, depth);
	std::cout << "\nNodes: " << nodes << "\n";
	
}

	
