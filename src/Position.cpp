#include "Position.h"


void makeMove(int move, Position& position, History& history) {

	if (position.board.sideToMove == WHITE) {
		makeMoveT<WHITE>(move, position, history);
	}
	else {
		makeMoveT<BLACK>(move, position, history);
	}
}

void unmakeMove(Position& position, History& history) {

	if (position.board.sideToMove == WHITE) {
		unmakeMoveT<WHITE>(position, history);
	}
	else {
		unmakeMoveT<BLACK>(position, history);
	}
}