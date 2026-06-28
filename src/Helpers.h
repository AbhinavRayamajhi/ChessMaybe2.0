#pragma once

#include <iostream>
#include <bit>
#include "Definitions.h"

inline constexpr int popCount(Bitboard b) {

	return __builtin_popcountll(b);
}

inline constexpr void resetBit(Bitboard& b, Square sq) {

	b &= ~(1ULL << sq);
}

inline constexpr void setBit(Bitboard& b, Square sq) {

	b |= (1ULL << sq);
}

inline constexpr bool getBit(Bitboard b, Square sq) {

	return b & (1ULL << sq);
}

inline constexpr void clearLSB(Bitboard& b) {

	b &= (b - 1);
}

inline constexpr Square getLSB(Bitboard b) {

	return __builtin_ctzll(b);
}

inline constexpr Square popLSB(Bitboard& b) {

	Square lsb = getLSB(b);
	clearLSB(b);
	return lsb;
}

inline constexpr Bitboard boardFromSq(Square sq) {

	return 1ULL << sq;
}

// char to file/rank conversions
inline constexpr File getFileFromChar(char c) {
	
	return c - 'a';
}

inline constexpr Rank getRankFromChar(char c) {

	return c - '1';
}

// square index to rank and file Conversions
inline constexpr Rank getRankFromSquare(Square square) {

	return (square >> 3) & 0b111;
}

inline constexpr File getFileFromSquare(Square square) {

	return square & 0b111;
}

// rank and file to square index conversion
inline constexpr Square getSquareFromRF(Rank rank, File file) {

	return (rank << 3) + file;
}

// square index to string conversion
inline std::string squareToString(int square) {
	
	char rank = '1' + getRankFromSquare(square);
	char file = 'a' + getFileFromSquare(square);
	return std::string() + file + rank;
}

inline Square stringToSquare(std::string s) {
	
	Rank rank = getRankFromChar(s[1]);
	File file = getFileFromChar(s[0]);

	return getSquareFromRF(rank, file);
}

inline void printBitboard(Bitboard bb) {
	
	for (int rank = 7; rank >= 0; --rank) {

		std::cout << rank + 1 << "  ";
		for (int file = 0; file < 8; ++file) {
			
			Square sq = getSquareFromRF(rank, file);
			std::cout << (((bb >> sq) & 1ULL) ? '1' : '0') << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n   a b c d e f g h\n\n";

}

template<Direction dir>
inline constexpr Bitboard bitboardShift(Bitboard bb) {

    if constexpr (dir == NORTH)      return bb << 8; 
    if constexpr (dir == SOUTH)      return bb >> 8; 
    if constexpr (dir == EAST)       return (bb & ~FILE_H) << 1; 
    if constexpr (dir == WEST)       return (bb & ~FILE_A) >> 1; 
    if constexpr (dir == NORTH_EAST) return (bb & ~FILE_H) << 9; 
    if constexpr (dir == NORTH_WEST) return (bb & ~FILE_A) << 7; 
    if constexpr (dir == SOUTH_EAST) return (bb & ~FILE_H) >> 7; 
	if constexpr (dir == SOUTH_WEST) return (bb & ~FILE_A) >> 9;
	if constexpr (dir == NORTH_NORTH_EAST) return (bb & ~FILE_H) << 17; 
	if constexpr (dir == NORTH_NORTH_WEST) return (bb & ~FILE_A) << 15; 
	if constexpr (dir == SOUTH_SOUTH_EAST) return (bb & ~FILE_H) >> 15; 
	if constexpr (dir == SOUTH_SOUTH_WEST) return (bb & ~FILE_A) >> 17; 
	if constexpr (dir == NORTH_EAST_EAST ) return (bb & ~FILE_G & ~FILE_H) << 10; 
	if constexpr (dir == NORTH_WEST_WEST ) return (bb & ~FILE_A & ~FILE_B) << 6; 
	if constexpr (dir == SOUTH_EAST_EAST ) return (bb & ~FILE_G & ~FILE_H) >> 6; 
	if constexpr (dir == SOUTH_WEST_WEST ) return (bb & ~FILE_A & ~FILE_B) >> 10; 
	return 0ULL;
}