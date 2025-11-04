#include "Magic.h"

namespace Magic
{
	Bitboard bishopAttackForBlocker(int sq, Bitboard blockers)
	{
		Bitboard attack = 0ULL;

		for (int nE = sq + NorthEast; nE < 64 && (nE % 8 != 0); nE += NorthEast)
		{
			attack |= 1ULL << nE;

			if (getBit(blockers, nE)) break;
		}
		for (int nW = sq + NorthWest; nW < 64 && (nW % 8 != 7); nW += NorthWest)
		{
			attack |= 1ULL << nW;
			if (getBit(blockers, nW)) break;
		}
		for (int sE = sq + SouthEast; sE >= 0 && (sE % 8 != 0); sE += SouthEast)
		{
			attack |= 1ULL << sE;
			if (getBit(blockers, sE)) break;
		}
		for (int sW = sq + SouthWest; sW >= 0 && (sW % 8 != 7); sW += SouthWest)
		{
			attack |= 1ULL << sW;
			if (getBit(blockers, sW)) break;
		}
		return attack;
	}

	Bitboard rookAttackForBlocker(int sq, Bitboard blockers)
	{
		Bitboard attack = 0ULL;

		for (int n = sq + North; n < 64; n += North)
		{
			attack |= 1ULL << n;

			if (getBit(blockers, n)) break;
		}
		for (int s = sq + South; s > 0; s += South)
		{
			attack |= 1ULL << s;
			if (getBit(blockers, s)) break;
		}
		for (int e = sq + East; e < 64 && (e % 8 != 0); e += East)
		{
			attack |= 1ULL << e;
			if (getBit(blockers, e)) break;
		}
		for (int w = sq + West; w >= 0 && (w % 8 != 7); w += West)
		{
			attack |= 1ULL << w;
			if (getBit(blockers, w)) break;
		}
		return attack;
	}

	Bitboard generateBlockers(int index, Bitboard mask)
	{
		Bitboard blockers{ 0 };

		Bitboard bitCount = popCount(mask);

		for (Bitboard i = 0; i < bitCount; ++i)
		{
			int sq = lsb(mask);
			clearLSB(mask);

			if (index & (1 << i))
				blockers |= (1ULL << sq);
		}
		return blockers;
	}

	void computeSliderAttack()
	{
		for (int sq = 0; sq < 64; ++sq)
		{
			int bishopBits = 64 - BISHOP_SHIFTS[sq];
			int rookBits = 64 - ROOK_SHIFTS[sq];

			int bishopEntries = 1 << bishopBits;
			int rookEntries = 1 << rookBits;

			for (int i = 0; i < bishopEntries; ++i)
			{
				Bitboard blockers = generateBlockers(i, BISHOP_MASKS[sq]);
				Bitboard index = (blockers * BISHOP_MAGICS[sq]) >> BISHOP_SHIFTS[sq];
				MoveGen::BISHOP_ATTACKS[sq][index] = bishopAttackForBlocker(sq, blockers);
			}

			for (int i = 0; i < rookEntries; ++i)
			{
				Bitboard blockers = generateBlockers(i, ROOK_MASKS[sq]);
				Bitboard index = (blockers * ROOK_MAGICS[sq]) >> ROOK_SHIFTS[sq];
				MoveGen::ROOK_ATTACKS[sq][index] = rookAttackForBlocker(sq, blockers);
			}
		}
	}
}