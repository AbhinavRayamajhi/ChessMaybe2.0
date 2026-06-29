#pragma once

#include "Attacks.h"
#include "Definitions.h"
#include "Helpers.h"

namespace Engine {

	class Random64 {
	public:

		Random64(uint64_t seed) {

			state = seed;
		}

		inline uint64_t newRand() {

			state += 0x9e3779b97f4a7c15;
			uint64_t z = state;
			z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
			z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
			return z ^ (z >> 31);
		}

		inline uint64_t sparseRand() {

			return newRand() & newRand() & newRand();
		}

	private:
		uint64_t state;
	};

	struct Magic {

		Bitboard mask{};
		Bitboard magic{};
		uint32_t offset{};
		uint8_t shift{};

		inline uint64_t getIndex(Bitboard subset) {
			//printBitboard(subset);
			return offset + ((subset * magic) >> (64 - shift));
		}
	};

	inline std::array<Magic, 64> BISHOP_MAGICS{};
	inline std::array<Magic, 64> ROOK_MAGICS{};

	void initMagics();

	// retrieve bishop moves using magic 
	inline Bitboard getBishopAttacks(Bitboard occ, int sq) {

		occ &= BISHOP_MAGICS[sq].mask;
		uint64_t index = BISHOP_MAGICS[sq].getIndex(occ);
		return BISHOP_ATTACKS[index];
	}

	// retrieve rook moves using magic 
	inline Bitboard getRookAttacks(Bitboard occ, int sq) {

		occ &= ROOK_MAGICS[sq].mask;
		uint64_t index = ROOK_MAGICS[sq].getIndex(occ);
		return ROOK_ATTACKS[index];
	}

	// retrieve queen moves from rook and bishop 
	inline Bitboard getQueenAttacks(Bitboard occ, int sq) {
		return getRookAttacks(occ, sq) | getBishopAttacks(occ, sq);
	}
}