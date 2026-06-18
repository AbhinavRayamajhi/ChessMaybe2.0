#pragma once

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Definitions.h"
#include "Helpers.h"

struct Board {

	Board();
	Board(std::string FEN);
	void printBoard();

	inline void updateAllOccupancy() {

		occupancy[WHITE] = 0ULL;
		occupancy[BLACK] = 0ULL;

		for (Piece p = PAWN; p != PIECE_NONE; ++p) {
			occupancy[WHITE] |= pieces[WHITE][p];
			occupancy[BLACK] |= pieces[BLACK][p];
		}

		occupancy[BOTH] = occupancy[WHITE] | occupancy[BLACK];
	}

	inline void updateCombinedOccupancy() {

		occupancy[BOTH] = occupancy[WHITE] | occupancy[BLACK];
	}

	template <Color side>
	inline void removePiece(Piece piece, Square sq) {

		pieces[side][piece] ^= 1ULL << sq;
		occupancy[side] ^= 1ULL << sq;
	}

	template <Color side>
	inline void addPiece(Piece piece, Square sq) {

		pieces[side][piece] ^= 1ULL << sq;
		occupancy[side] ^= 1ULL << sq;
	}

	template <Color side>
	inline void movePiece(Piece piece, Square startSq, Square targetSq) {

		pieces[side][piece] ^= 1ULL << startSq | 1ULL << targetSq;
		occupancy[side] ^= 1ULL << startSq | 1ULL << targetSq;
	}

	template <Color side>
	inline Piece findPiece(Square sq) {

		for (Piece p = PAWN; p != PIECE_NONE; ++p) {
			if (getBit(pieces[side][p], sq)) return p;
		}
		return PIECE_NONE;
	}

	// Members
	std::array<std::array<Bitboard, PIECE_COUNT>, Colors::BOTH> pieces;
	std::array<Bitboard, OCC_COUNT> occupancy;

	Color sideToMove;
	CastlingRight castlingRights;
	Square enPassantSq;

	int halfMoveClock;
	int fullMoveClock;
};



