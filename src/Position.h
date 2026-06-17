#pragma once

#include "Board.h"
#include "Move.h"

struct History {
	Move move;
	Piece moved = PIECE_NONE;
	Piece captured = PIECE_NONE;
	CastlingRight castlingRights = NO_CASTLING;
	Square enPassantSq = SQ_NONE;
	int halfMoveClock = 0;
};

struct HistoryList {
	History list[1024];
	int end = 0;

	void addHistory(History h) {
		list[end++] = h;
	}
};

struct CheckInfo {
	Bitboard checkers = 0ULL;
	Bitboard checkMask = 0ULL;
	Bitboard pinned = 0ULL;
};

void makeMove(int move, Board& board, History& history);
void unmakeMove(Board& board, History& history);