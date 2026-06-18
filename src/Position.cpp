#include "Position.h"


void makeMove(int move, Board& board, History& history) {

	if (board.sideToMove == WHITE) {
		makeMoveT<WHITE>(move, board, history);
	}
	else {
		makeMoveT<BLACK>(move, board, history);
	}
}

void unmakeMove(Board& board, History& history) {

	if (board.sideToMove == WHITE) {
		unmakeMoveT<WHITE>(board, history);
	}
	else {
		unmakeMoveT<BLACK>(board, history);
	}
}