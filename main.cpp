#include <fstream>

#include "MoveGen.h"
#include "Test.h"
#include "Magic.h"

int main()
{

	int moveList[256]{ 0 };
	int* list = &moveList[0];
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KB1R w KQkq - 0 1";
	State state = stateFromFEN(FEN);
	Board board = createBoard(FEN);
	Magic::computeSliderAttack();

	printBoard(board);

	list = MoveGen::generateLegalMoves(board, list, state.sideToMove, state.enPassantSquare, state.castlingRights);
	std::cout << printMoveList(moveList);
}

	
