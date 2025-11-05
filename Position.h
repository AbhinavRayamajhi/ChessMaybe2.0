#pragma once

#include "Board.h"
#include "State.h"
#include "Move.h"


struct CheckInfo
{
	Bitboard pinned{ 0 };
	Bitboard checkers{ 0 };
	Bitboard checkMask{ 0 };
};

struct Position
{
	Board board;
	State state;

	Position(Board b, State s)
		:board(b), state(s) {};
};

void makeMove(int move, Board& board, State& state, std::vector<Position>& history);
void unmakeMove(Board& board, State& state, std::vector<Position>& history);