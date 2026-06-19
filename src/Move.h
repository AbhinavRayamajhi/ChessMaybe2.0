#pragma once

#include <iostream>
#include <array>
#include "Definitions.h"
#include "Helpers.h"

#define Move uint16_t

// encoding move information into 16-bit int: 
// 0 - 5 start,
// 6 - 11 target,
// 12 - 13 promotion piece,
// 14 - 15 move type: 00 Normal, 01 En Passant, 10 Castling, 11 Promotion
inline Move createMove(Square start, Square target, Piece promPiece, MoveType moveType) {

	return start | (target << 6) | (promPiece << 12) | (moveType << 14);
}

// extracting info from move integer
inline Square getStartSq(Move m) {
	return m & 0x3F;
}

inline Square getTargetSq(Move m) {
	return (m >> 6) & 0x3F;
}

inline Piece getPromotionPiece(Move m) {
	return ((m >> 12) & 0b11) + 1;
}

inline MoveType getMoveType(Move m) {
	return (m >> 14) & 0b11;
}

inline void printMove(Move move) {

	std::cout << squareToString(getStartSq(move)) << squareToString(getTargetSq(move));

	if (getMoveType(move) == PROMOTION) {
		std::cout << intToPieceString[getPromotionPiece(move)];
	}
}

inline void printMoveList(Move moveList[]) {

	std::cout << "Possible Moves: " << '\n';
	for (int i = 0; i < 256; ++i) {

		if (moveList[i] == 0) {
			break;
		}

		printMove(moveList[i]);
		std::cout << ", ";
	}
	std::cout << "\n\n";
}

struct MoveList {
	std::array<Move, 256> list;
	std::array<int, 256> score;
	uint8_t end;

	MoveList() :
		list{},  score{}, end{ 0 } {
	}

	inline void addMove(Move m) {
		list[end++] = m;
	}

	inline void removeMove(int index) {
		score[index] = score[--end];
		list[index] = list[end];
	}

	inline void swapMoveScore(int index, int bestIndex) {
		std::swap(list[index], list[bestIndex]);
    	std::swap(score[index], score[bestIndex]);
	}
};