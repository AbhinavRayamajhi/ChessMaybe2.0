#pragma once

#include <algorithm>
#include <cassert>

#include "Attacks.h"
#include "Board.h"
#include "Definitions.h"
#include "Helpers.h"
#include "Magic.h"
#include "Move.h"
#include "Position.h"

namespace Engine {

	// checks if king is being attacked by enemy pieces
	Bitboard isKingInCheck(Position& position);

	template<bool ScoreMoves, bool captures>
	void generateLegalMoves(Position& position, MoveList& moveList);

	extern template void generateLegalMoves<true, true>(Position& position, MoveList& moveList);
	extern template void generateLegalMoves<true, false>(Position& position, MoveList& moveList);
	extern template void generateLegalMoves<false, true>(Position& position, MoveList& moveList);
	extern template void generateLegalMoves<false, false>(Position& position, MoveList& moveList);
}
