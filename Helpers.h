#pragma once

#include <iostream>
#include <bit>
#include "Definitions.h"

inline uint64_t popCount(Bitboard b) { return __popcnt64(b); }
inline void popBit(Bitboard& b, int sq) { b = b & ~(1ULL << sq); }
inline void setBit(Bitboard& b, int sq) { b = b | (1ULL << sq); }
inline bool getBit(Bitboard b, int sq) { return b & (1ULL << sq); }
inline void clearLSB(Bitboard& b) { b &= (b - 1); }
inline  int lsb(Bitboard b) {return std::countr_zero(b);}

// char to file/rank conversions
inline int getFileFromChar(char c) { return fileToInt.at(c); }
inline int getRankFromChar(char c) { return c - '1'; }

// square index to rank and file Conversions
inline int getRankFromInt(int square) { return square >> 3; }
inline int getFileFromInt(int square) { return square & 0b111UL; }

// rank and file to square index conversion
inline int getSquareFromRF(int rank, int file) { return (rank << 3) + file; }

// square index to string conversion
inline std::string squareToString(int square)
{
	char rank = '1' + getRankFromInt(square);
	char file = 'a' + getFileFromInt(square);
	return std::string() + file + rank;
}

inline void printBitboard(Bitboard bb)
{
	for (int rank = 7; rank >= 0; --rank)
	{
		std::cout << rank + 1 << "  ";

		for (int file = 0; file < 8; ++file)
		{
			int sq = rank * 8 + file;
			std::cout << (((bb >> sq) & 1ULL) ? '1' : '0') << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n   a b c d e f g h\n\n";
}