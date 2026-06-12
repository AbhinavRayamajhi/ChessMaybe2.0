#pragma once

#include "Attacks.h"
#include "Definitions.h"
#include "Helpers.h"
#include "MoveGen.h"

namespace Magic
{
	Bitboard bishopAttackForBlocker(Square sq, Bitboard blockers);
	Bitboard rookAttackForBlocker(Square sq, Bitboard blockers);

	void computeSliderAttack();
}