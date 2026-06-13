#include "Magic.h"

namespace Engine {

	Bitboard bishopAttackForBlocker(Square sq, Bitboard blockers) {

		Bitboard attacks = 0ULL;
		Bitboard cur = 0ULL;
		Bitboard ray = 0ULL;
		setBit(cur, sq);

		ray = bitboardShift(cur, NORTH_WEST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, NORTH_WEST);
		}

		ray = bitboardShift(cur, SOUTH_WEST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, SOUTH_WEST);
		}

		ray = bitboardShift(cur, NORTH_EAST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, NORTH_EAST);
		}

		ray = bitboardShift(cur, SOUTH_EAST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, SOUTH_EAST);
		}

		return attacks;
	}

	Bitboard rookAttackForBlocker(Square sq, Bitboard blockers) {

		Bitboard attacks = 0ULL;
		Bitboard cur = 0ULL;
		Bitboard ray = 0ULL;
		setBit(cur, sq);

		ray = bitboardShift(cur, NORTH);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, NORTH);
		}

		ray = bitboardShift(cur, SOUTH);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, SOUTH);
		}

		ray = bitboardShift(cur, WEST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, WEST);
		}

		ray = bitboardShift(cur, EAST);
		while (ray != 0ULL) {

			attacks |= ray;
			if(ray & blockers) {
				break;
			}

			ray = bitboardShift(ray, EAST);
		}

    	return attacks;
	}

	Bitboard generateBlockers(int index, Bitboard mask) {

		Bitboard blockers{ 0 };

		Bitboard bitCount = popCount(mask);

		for (Bitboard i = 0; i < bitCount; ++i)
		{
			Square sq = popLSB(mask);

			if (index & (1 << i))
				blockers |= (1ULL << sq);
		}
		return blockers;
	}

	void computeSliderAttack() {

		for (Square sq = A1; sq < SQ_COUNT; ++sq) {
			
			int bishopEntries = 1 << 9;
			int rookEntries = 1 << 12;

			for (int i = 0; i < bishopEntries; ++i) {

				Bitboard blockers = generateBlockers(i, BISHOP_MASKS[sq]);
				Bitboard index = ((blockers * BISHOP_MAGICS[sq]) >> 55) + BISHOP_OFFSETS[sq];
				BISHOP_ATTACKS[index] = bishopAttackForBlocker(sq, blockers);
			}

			for (int i = 0; i < rookEntries; ++i) {

				Bitboard blockers = generateBlockers(i, ROOK_MASKS[sq]);
				Bitboard index = ((blockers * ROOK_MAGICS[sq]) >> 52) + ROOK_OFFSETS[sq];
				ROOK_ATTACKS[index] = rookAttackForBlocker(sq, blockers);
			}
		}
	}
}