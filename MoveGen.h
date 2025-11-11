#pragma once

#include <algorithm>

#include "Attacks.h"
#include "Board.h"
#include "Definitions.h"
#include "Helpers.h"
#include "Move.h"
#include "Position.h"
#include "State.h"

namespace MoveGen
{
	Bitboard isKingInCheck(const Board& board, bool sideToMove);

	// bishop and rook attack arrays
	extern std::array<Bitboard, 88772> BISHOP_ATTACKS;
	extern std::array<Bitboard, 88772> ROOK_ATTACKS;
	extern std::array<std::array<Bitboard, 64>, 64> RAY_MASKS;

	int* generateLegalMoves(const Board& board, int* moveList, const State& state);
}