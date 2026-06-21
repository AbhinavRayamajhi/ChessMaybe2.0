#pragma once

#include <iostream>
#include <bit>
#include "Definitions.h"

inline int popCount(Bitboard b) {

	return __builtin_popcountll(b);
}

inline void resetBit(Bitboard& b, Square sq) {

	b &= ~(1ULL << sq);
}

inline void setBit(Bitboard& b, Square sq) {

	b |= (1ULL << sq);
}

inline bool getBit(Bitboard b, Square sq) {

	return b & (1ULL << sq);
}

inline void clearLSB(Bitboard& b) {

	b &= (b - 1);
}

inline Square getLSB(Bitboard b) {

	return __builtin_ctzll(b);
}

inline Square popLSB(Bitboard& b) {

	Square lsb = getLSB(b);
	clearLSB(b);
	return lsb;
}

// char to file/rank conversions
inline File getFileFromChar(char c) {

	return fileToInt.at(c);
}

inline Rank getRankFromChar(char c) {

	return c - '1';
}

// square index to rank and file Conversions
inline Rank getRankFromInt(Square square) {

	return (square >> 3) & 0b111;
}

inline File getFileFromInt(Square square) {

	return square & 0b111;
}

// rank and file to square index conversion
inline Square getSquareFromRF(Rank rank, File file) {

	return (rank << 3) + file;
}

// square index to string conversion
inline std::string squareToString(int square) {
	
	char rank = '1' + getRankFromInt(square);
	char file = 'a' + getFileFromInt(square);
	return std::string() + file + rank;
}

inline Square stringToSquare(std::string s) {
	
	Rank rank = getRankFromChar(s[1]);
	File file = getFileFromChar(s[0]);

	return getSquareFromRF(rank, file);
}

inline void printBitboard(Bitboard bb) {
	
	for (Rank rank = 7; rank >= 0; --rank) {

		std::cout << rank + 1 << "  ";
		for (File file = 0; file < 8; ++file) {
			
			Square sq = getSquareFromRF(rank, file);
			std::cout << (((bb >> sq) & 1ULL) ? '1' : '0') << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n   a b c d e f g h\n\n";

}

template<Direction dir>
inline Bitboard bitboardShift(Bitboard bb) {

    if constexpr (dir == NORTH)      return bb << 8; 
    if constexpr (dir == SOUTH)      return bb >> 8; 
    if constexpr (dir == EAST)       return (bb & ~FILE_H) << 1; 
    if constexpr (dir == WEST)       return (bb & ~FILE_A) >> 1; 
    if constexpr (dir == NORTH_EAST) return (bb & ~FILE_H) << 9; 
    if constexpr (dir == NORTH_WEST) return (bb & ~FILE_A) << 7; 
    if constexpr (dir == SOUTH_EAST) return (bb & ~FILE_H) >> 7; 
    if constexpr (dir == SOUTH_WEST) return (bb & ~FILE_A) >> 9; 
    return 0ULL;
}