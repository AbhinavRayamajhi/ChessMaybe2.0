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
	
	Bitboard isKingInCheck(const Board& board);

	template<bool ScoreMoves, bool captures>
	void generateLegalMoves(const Board& board, MoveList& moveList);

	extern template void generateLegalMoves<true, true>(const Board& board, MoveList& moveList);
	extern template void generateLegalMoves<true, false>(const Board& board, MoveList& moveList);
	extern template void generateLegalMoves<false, true>(const Board& board, MoveList& moveList);
	extern template void generateLegalMoves<false, false>(const Board& board, MoveList& moveList);
}
