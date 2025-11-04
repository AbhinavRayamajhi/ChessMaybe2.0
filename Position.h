#pragma once

#include "Board.h"
#include "State.h"


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
};
