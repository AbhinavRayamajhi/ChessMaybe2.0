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

class Position {
public:
	
	inline Board getBoard() {

		return board;
	}

	inline void setBoard(std::string FEN) {

		board = Board(FEN);
	}

	template<Color side, Piece p>
	inline Bitboard getPiece() const {

		return board.pieces[side][p];
	}

	template<Color side>
	inline Bitboard getPiece(Piece p) const {

		return board.pieces[side][p];
	}

	template<Color side>
	inline Bitboard getOccupancy() const {

		return board.occupancy[side];
	}

	inline Bitboard getBothOccupancy() const {

		return board.occupancy[BOTH];
	}

	inline Bitboard getEmpty() const {

		return ~board.occupancy[BOTH];
	}

	inline Color side() const {

		return board.sideToMove;
	}

	inline Square epSq() {

		return board.enPassantSq;
	}

	inline void setEpSq(Square epSq) {

		board.enPassantSq = epSq;
	}

	inline CastlingRight cr() {

		return board.castlingRights;
	}

	inline void setCastlingRights(CastlingRight cr) {

		board.castlingRights = cr;
	}

	inline int hclock() {

		return halfMoveClock;
	}

	inline void setHalfMoveClock(int hclock) {

		halfMoveClock = hclock;
	}

	inline CastlingRight getCR() {

		return board.castlingRights;
	}

	template <Color side>
	inline void removePiece(Piece piece, Square sq) {

		board.removePiece<side>(piece, sq);
	}

	template <Color side>
	inline void addPiece(Piece piece, Square sq) {

		board.addPiece<side>(piece, sq);
	}

	template <Color side>
	inline void movePiece(Piece piece, Square startSq, Square targetSq) {

		board.movePiece<side>(piece, startSq, targetSq);
	}

	template <Color side>
	inline Piece findPiece(Square sq) {

		return board.findPiece<side>(sq);
	}

	inline void updateAllOccupancy() {

		board.updateAllOccupancy();
	}

	inline void updateCombinedOccupancy() {

		board.updateCombinedOccupancy();
	}

	inline Bitboard getCheckers() const {

		return checkInfo.checkers;
	}

	inline Bitboard getCheckMask() const {

		return checkInfo.checkMask;
	}

	inline Bitboard getPinned() const {

		return checkInfo.pinned;
	}

	inline void setCheckInfo(CheckInfo ci) {

		checkInfo = ci;
	}

	inline void flipSide() {

		board.sideToMove = board.sideToMove == WHITE ? BLACK : WHITE;
	}

	inline void printBoard() {

		board.printBoard();
	}

	private:
	Board board;
	CheckInfo checkInfo;
	std::array<Piece, SQ_COUNT> pieceSq;
	int halfMoveClock;
	int fullMoveClock;
};

void makeMove(int move, Position& position, History& history);
void unmakeMove(Position& position, History& history);