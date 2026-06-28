#include "Magic.h"

#include <cmath>

namespace Engine {

    void bishopMagicFinder(int sq, Random64& rand) {

        Bitboard edges = ((RANK_1 | RANK_8) & ~RANKS[getRankFromSquare(sq)]) | ((FILE_A | FILE_H) & ~FILES[getFileFromSquare(sq)]);
        BISHOP_MAGICS[sq].mask = getAttackMask<BISHOP>(sq) & ~edges;
        BISHOP_MAGICS[sq].magic = 0;
        BISHOP_MAGICS[sq].offset = sq == 0 ? 0 : BISHOP_MAGICS[sq - 1].offset + std::pow(2, BISHOP_MAGICS[sq - 1].shift);
        BISHOP_MAGICS[sq].shift = static_cast<uint8_t>(popCount(BISHOP_MAGICS[sq].mask));
        
        bool found = false;

        while (!found) {

            // temp array to check hash collisions
            Bitboard magicChecker[512]{};
            Bitboard subset = 0;
            BISHOP_MAGICS[sq].magic = rand.sparseRand();

            do {
                
                Bitboard attacks = slidingAttacks<BISHOP>(sq, subset);
                uint64_t index = (subset * BISHOP_MAGICS[sq].magic) >> (64 - BISHOP_MAGICS[sq].shift);
                uint64_t attackIndex = BISHOP_MAGICS[sq].offset + ((subset * BISHOP_MAGICS[sq].magic)
                    >> (64 - BISHOP_MAGICS[sq].shift));

                // hash collision check
                if (magicChecker[index] && (magicChecker[index] != attacks)) break;

                magicChecker[index] = attacks;
                BISHOP_ATTACKS[attackIndex] = attacks;
                subset = BISHOP_MAGICS[sq].mask & (subset - BISHOP_MAGICS[sq].mask);

                // all attacks for this sq processed so correct magic num
                if (subset == 0) found = true;

            } while (subset != 0);
        }
    }

    void rookMagicFinder(int sq, Random64& rand) {

        Bitboard edges = ((RANK_1 | RANK_8) & ~RANKS[getRankFromSquare(sq)]) | ((FILE_A | FILE_H) & ~FILES[getFileFromSquare(sq)]);
        ROOK_MAGICS[sq].mask = getAttackMask<ROOK>(sq) & ~edges;
        ROOK_MAGICS[sq].magic = 0;
        ROOK_MAGICS[sq].offset = sq == 0 ? 0 : ROOK_MAGICS[sq - 1].offset + std::pow(2, ROOK_MAGICS[sq - 1].shift);
        ROOK_MAGICS[sq].shift = static_cast<uint8_t>(popCount(ROOK_MAGICS[sq].mask));

        bool found = false;

        while (!found) {

            // temp array to check hash collisions
            Bitboard magicChecker[4096]{};
            Bitboard subset = 0;
            ROOK_MAGICS[sq].magic = rand.sparseRand();

            do {
                
                Bitboard attacks = slidingAttacks<ROOK>(sq, subset);
                uint64_t index = (subset * ROOK_MAGICS[sq].magic) >> (64 - ROOK_MAGICS[sq].shift);
                uint64_t attackIndex = ROOK_MAGICS[sq].offset + ((subset * ROOK_MAGICS[sq].magic)
                    >> (64 - ROOK_MAGICS[sq].shift));

                // hash collision check
                if (magicChecker[index] && (magicChecker[index] != attacks)) break;

                magicChecker[index] = attacks;
                ROOK_ATTACKS[attackIndex] = attacks;
                subset = ROOK_MAGICS[sq].mask & (subset - ROOK_MAGICS[sq].mask);

                // all attacks for this sq processed so correct magic num
                if (subset == 0) found = true;
                
            } while (subset != 0);
        }
    }

    void initMagics() {

        auto rand = Random64(0x5330);

        for (Square sq = A1; sq != SQ_NONE; ++sq) {

            bishopMagicFinder(sq, rand);
            rookMagicFinder(sq, rand);
        }
    }
}