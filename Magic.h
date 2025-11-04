#pragma once

#include "Attacks.h"
#include "Definitions.h"
#include "Helpers.h"
#include "MoveGen.h"

namespace Magic
{
	Bitboard bishopAttackForBlocker(int sq, Bitboard blockers);
	Bitboard rookAttackForBlocker(int sq, Bitboard blockers);

	void computeSliderAttack();
}