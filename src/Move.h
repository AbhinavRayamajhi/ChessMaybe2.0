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

inline Move convertMoveFromString(const Board& board, std::string input) {

	Square start = stringToSquare(input.substr(0, 2));
	Square target = stringToSquare(input.substr(2, 2));
	MoveType moveType = NORMAL;
	Piece promPiece = NO_PROMOTION;

	if (input.length() == 5) {
		moveType = PROMOTION;
		promPiece = promotionMap.at(input[4]);
	}
	else if (target == board.enPassantSq) {
		std::cout << "EP branch hit: target=" << (int)target << " epSq=" << (int)board.enPassantSq << std::endl;
		moveType = ENPASSANT;
	}
	else if (board.sideToMove == WHITE && getBit(board.pieces[WHITE][KING], E1) && (target == C1 || target == G1)) {
		moveType = CASTLING;
	}
	else if (board.sideToMove == BLACK && getBit(board.pieces[BLACK][KING], E8) && (target == C8 || target == G8)) {
		moveType = CASTLING;
	}

	return createMove(start, target, promPiece, moveType);
}

inline std::string convertMoveToString(Move move) {

	std::string res;
	res += squareToString(getStartSq(move));
	res += squareToString(getTargetSq(move));

	if (getMoveType(move) == PROMOTION) {
		res += intToPieceString[getPromotionPiece(move)];
	}

	return res;
}

inline void printMove(Move move) {

	std::cout << convertMoveToString(move);
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