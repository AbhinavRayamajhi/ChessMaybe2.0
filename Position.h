#pragma once

#include "Board.h"
#include "Move.h"

void makeMove(int move, Board& board, State& state, std::vector<Position>& history);
void unmakeMove(Board& board, State& state, std::vector<Position>& history);