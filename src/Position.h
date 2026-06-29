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
	
	template<Color side, Piece p>
	inline Bitboard getPiece() const {

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

	// checks if a particular square is being attacked by enemy pieces
	template <Color side>
	Bitboard squareAttackers(Square sq);

	// checks if king is being attacked by enemy pieces
	inline Bitboard isKingInCheck() {

		if (board.sideToMove == WHITE) {
			Square kSq = getLSB(board.pieces[WHITE][KING]);
			return squareAttackers<WHITE>(kSq);
		}
		else {
			Square kSq = getLSB(board.pieces[BLACK][KING]);
			return squareAttackers<BLACK>(kSq);
		}
	}

	// calculates check info, pinned pieces, checkers and check masks
	template <Color side>
	void getCheckInfo();

private:
	
	Board board;
	CheckInfo checkInfo;
	std::array<Piece, SQ_COUNT> pieceSq;
	int halfMoveClock;
	int fullMoveClock;
};

template <Color side>
__attribute__((always_inline)) inline void makeMoveT(int move, Position& position, History& history) {

	Square start = getStartSq(move);
	Square target = getTargetSq(move);
	MoveType moveType = getMoveType(move);

	history.move = move;
    history.enPassantSq = position.board.enPassantSq;
    history.castlingRights = position.board.castlingRights;
	history.halfMoveClock = position.halfMoveClock;

	position.board.enPassantSq = SQ_NONE;

	// set castling right to false if not moved rook is target
	switch (target) {
		
		case H1:
			position.board.castlingRights &= ~WHITE_OO; break;
		case A1:
			position.board.castlingRights &= ~WHITE_OOO; break;
		case H8:
			position.board.castlingRights &= ~BLACK_OO; break;
		case A8:
			position.board.castlingRights &= ~BLACK_OOO; break;
		default: break;
	}

	// set castling right to false if anything moves from rook sq
	switch (start) {
		
		case H1:
			position.board.castlingRights &= ~WHITE_OO; break;
		case A1:
			position.board.castlingRights &= ~WHITE_OOO; break;
		case H8:
			position.board.castlingRights &= ~BLACK_OO; break;
		case A8:
			position.board.castlingRights &= ~BLACK_OOO; break;
		default: break;
	}

	history.moved = position.board.findPiece<side>(start);
	if (history.moved == PAWN) {

		// set en passant sq if double push
		if (start + 16 == target) position.board.enPassantSq = start + 8;
		else if (start - 16 == target) position.board.enPassantSq = start - 8;

		// reset half move clock since pawn move
		position.halfMoveClock = 0;
	}
	// king move cancels all castling
	else if (history.moved == KING) {

		position.board.castlingRights &= (side == WHITE) ? ~WHITE_CASTLE : ~BLACK_CASTLE;
	}

	history.captured = position.board.findPiece<!side>(target);
	if (history.captured != PIECE_NONE) {

		// reset half move clock since capture
		position.halfMoveClock = 0;
		position.board.removePiece<!side>(history.captured, target);
	}

	// move the piece
	position.board.movePiece<side>(history.moved, start, target);

	// if enPassant capture remove the captured piece
	if (moveType == ENPASSANT) {

		position.board.removePiece<!side>(PAWN, target + (side == WHITE ? -8 : 8));
        history.captured = PAWN;
	}

	// if promotion piece set the relevant bit to promoted piece
	else if (moveType == PROMOTION) {

		// remove moved pawn and add promotion piece
		position.board.removePiece<side>(PAWN, target);
		position.board.addPiece<side>(getPromotionPiece(move), target);
	}
	else if (moveType == CASTLING) {

		Square rookFrom, rookTo;
        if (target == G1) {
            rookFrom = H1;
            rookTo = F1;
        }
        else if (target == G8) {
            rookFrom = H8;
            rookTo = F8;
        }
        else if (target == C1) {
            rookFrom = A1;
            rookTo = D1;
        }
        else {
            rookFrom = A8;
            rookTo = D8;
        }
		position.board.movePiece<side>(ROOK, rookFrom, rookTo);
	}

	position.board.updateCombinedOccupancy();
	position.board.sideToMove = (side == WHITE) ? BLACK : WHITE;
}

template <Color side>
__attribute__((always_inline)) inline void unmakeMoveT(Position& position, History& history)
{
	Square start = getStartSq(history.move);
	Square target = getTargetSq(history.move);
	MoveType moveType = getMoveType(history.move);

    position.board.enPassantSq = history.enPassantSq;
    position.board.castlingRights = history.castlingRights;
    position.halfMoveClock = history.halfMoveClock;

    if (moveType == PROMOTION) {

		position.board.removePiece<!side>(position.board.findPiece<!side>(target), target);
		position.board.addPiece<!side>(PAWN, start);
    }
    else if (moveType == CASTLING) {

        Square rookFrom, rookTo;
        if (target == G1) {
            rookFrom = F1;
            rookTo = H1;
        }
        else if (target == G8) {
            rookFrom = F8;
            rookTo = H8;
        }
        else if (target == C1) {
            rookFrom = D1;
            rookTo = A1;
        }
        else {
            rookFrom = D8;
            rookTo = A8;
        }

		position.board.movePiece<!side>(ROOK, rookFrom, rookTo);
		position.board.movePiece<!side>(KING, target, start);
    }
	else {
		position.board.movePiece<!side>(history.moved, target, start);
	}

    if (history.captured != PIECE_NONE) {

		if (moveType != ENPASSANT) {
			position.board.addPiece<side>(history.captured, target);
        }
		else {
			position.board.addPiece<side>(PAWN, target + (side == WHITE ? 8 : -8));
        }
    }

	position.board.updateCombinedOccupancy();
    position.board.sideToMove = side == WHITE ? BLACK : WHITE;
}

void makeMove(int move, Position& position, History& history);
void unmakeMove(Position& position, History& history);