#pragma once

#include "Definitions.h"
#include "Helpers.h"

constexpr Bitboard knightAttacks(Square sq) {

	Bitboard res = 0ULL;
	Bitboard cur = boardFromSq(sq);

	res |= bitboardShift<NORTH_NORTH_EAST>(cur);
	res |= bitboardShift<NORTH_NORTH_WEST>(cur);
	res |= bitboardShift<NORTH_EAST_EAST>(cur);
	res |= bitboardShift<NORTH_WEST_WEST>(cur);
	res |= bitboardShift<SOUTH_SOUTH_EAST>(cur);
	res |= bitboardShift<SOUTH_SOUTH_WEST>(cur);
	res |= bitboardShift<SOUTH_EAST_EAST>(cur);
	res |= bitboardShift<SOUTH_WEST_WEST>(cur);

	return res;
}

constexpr Bitboard kingAttacks(Square sq) {

	Bitboard res = 0ULL;
	Bitboard cur = boardFromSq(sq);

	res |= bitboardShift<NORTH_WEST>(cur);
	res |= bitboardShift<NORTH>(cur);
	res |= bitboardShift<NORTH_EAST>(cur);
	res |= bitboardShift<WEST>(cur);
	res |= bitboardShift<EAST>(cur);
	res |= bitboardShift<SOUTH_WEST>(cur);
	res |= bitboardShift<SOUTH>(cur);
	res |= bitboardShift<SOUTH_EAST>(cur);

	return res;
}

template<Piece p>
constexpr Bitboard slidingAttacks(Square sq, Bitboard occ) {

	Bitboard res = 0ULL;
	Bitboard cur = boardFromSq(sq);
	
	if (p == ROOK) {

		while (cur = bitboardShift<NORTH>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
		cur = boardFromSq(sq);

		while (cur = bitboardShift<SOUTH>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
		cur = boardFromSq(sq);

		while (cur = bitboardShift<EAST>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}

		cur = boardFromSq(sq);
		while (cur = bitboardShift<WEST>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
	}
	else if (p == BISHOP) {

		while (cur = bitboardShift<NORTH_EAST>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
		cur = boardFromSq(sq);

		while (cur = bitboardShift<NORTH_WEST>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
		cur = boardFromSq(sq);

		while (cur = bitboardShift<SOUTH_EAST>(cur)) {

			res |= cur;
			if (cur & occ) break;
		}
		cur = boardFromSq(sq);

		while (cur = bitboardShift<SOUTH_WEST>(cur)) {
			
			res |= cur;
			if (cur & occ) break;
		}
	}

	return res;
}

inline constexpr auto attackMasks = [] () constexpr{

	std::array<std::array<Bitboard, SQ_COUNT>, 4> res{};

	for (Square sq = A1; sq != SQ_NONE; ++sq) {

		res[0][sq] = knightAttacks(sq);
		res[1][sq] = kingAttacks(sq);
		res[2][sq] = slidingAttacks<BISHOP>(sq, 0);
		res[3][sq] = slidingAttacks<ROOK>(sq, 0);
	}
	return res;
} ();

template<Piece p>
inline Bitboard getAttackMask(Square sq) {

	if (p == KNIGHT) return attackMasks[0][sq];
	if (p == KING) return attackMasks[1][sq];
	if (p == BISHOP) return attackMasks[2][sq];
	if (p == ROOK) return attackMasks[3][sq];

	return 0ULL;
}

template<Direction dir>
constexpr Bitboard computeRay(Square sq) {

	Bitboard res = 0ULL;
	Bitboard cur = boardFromSq(sq);

	while (cur = bitboardShift<dir>(cur)) {
		res |= cur;
	}

	return res;
}

constexpr Bitboard getRays(Square sq1, Square sq2) {

	int rankDiff = getRankFromSquare(sq2) - getRankFromSquare(sq1);
	int fileDiff = getFileFromSquare(sq2) - getFileFromSquare(sq1);

	if (fileDiff == 0) {
		if (rankDiff > 0) return computeRay<NORTH>(sq1) & computeRay<SOUTH>(sq2);
		if (rankDiff < 0) return computeRay<SOUTH>(sq1) & computeRay<NORTH>(sq2);
	}
	if (rankDiff == 0) {
		if (fileDiff > 0) return computeRay<EAST>(sq1) & computeRay<WEST>(sq2);
		if (fileDiff < 0) return computeRay<WEST>(sq1) &  computeRay<EAST>(sq2);
	}
	if (rankDiff == fileDiff) {
		if (rankDiff > 0) return computeRay<NORTH_EAST>(sq1) & computeRay<SOUTH_WEST>(sq2);
		if (rankDiff < 0) return computeRay<SOUTH_WEST>(sq1) & computeRay<NORTH_EAST>(sq2);
	}
	if (rankDiff == -fileDiff) {
		if (rankDiff > 0) return computeRay<NORTH_WEST>(sq1) & computeRay<SOUTH_EAST>(sq2);
		if (rankDiff < 0) return  computeRay<SOUTH_EAST>(sq1) & computeRay<NORTH_WEST>(sq2);
	}

	return 0ULL;
}

// Precomputed table for rays between all squares
inline constexpr auto RAYS = [] () constexpr{

	std::array<std::array<Bitboard, SQ_COUNT>, SQ_COUNT> res{};

	for (Square sq1 = A1; sq1 != SQ_NONE; ++sq1) {
		for (Square sq2 = A1; sq2 != SQ_NONE; ++sq2) {
			res[sq1][sq2] = getRays(sq1, sq2);
		}
	}

	return res;
} ();

inline std::array<Bitboard, 0x1480> BISHOP_ATTACKS{};
inline std::array<Bitboard, 0x19000> ROOK_ATTACKS{};

