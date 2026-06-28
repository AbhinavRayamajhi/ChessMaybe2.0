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
private:
	Board board;
	CheckInfo checkInfo;
	std::array<Piece, SQ_COUNT> pieceSq;
	int halfMoveClock;
	int fullMoveClock;
};

template <Color side>
__attribute__((always_inline)) inline void makeMoveT(int move, Board& board, History& history) {

	Square start = getStartSq(move);
	Square target = getTargetSq(move);
	MoveType moveType = getMoveType(move);

	history.move = move;
    history.enPassantSq = board.enPassantSq;
    history.castlingRights = board.castlingRights;
	history.halfMoveClock = board.halfMoveClock;
	board.enPassantSq = SQ_NONE;

	// set castling right to false if not moved rook is target
	switch (target) {
		
		case H1:
			board.castlingRights &= ~WHITE_OO; break;
		case A1:
			board.castlingRights &= ~WHITE_OOO; break;
		case H8:
			board.castlingRights &= ~BLACK_OO; break;
		case A8:
			board.castlingRights &= ~BLACK_OOO; break;
		default: break;
	}

	// set castling right to false if anything moves from rook sq
	switch (start) {
		
		case H1:
			board.castlingRights &= ~WHITE_OO; break;
		case A1:
			board.castlingRights &= ~WHITE_OOO; break;
		case H8:
			board.castlingRights &= ~BLACK_OO; break;
		case A8:
			board.castlingRights &= ~BLACK_OOO; break;
		default: break;
	}

	history.moved = board.findPiece<side>(start);
	if (history.moved == PAWN) {

		// set en passant sq if double push
		if (start + 16 == target) board.enPassantSq = start + 8;
		else if (start - 16 == target) board.enPassantSq = start - 8;

		// reset half move clock since pawn move
		board.halfMoveClock = 0;
	}
	// king move cancels all castling
	else if (history.moved == KING) {

		board.castlingRights &= (side == WHITE) ? ~WHITE_CASTLE : ~BLACK_CASTLE;
	}

	history.captured = board.findPiece<!side>(target);
	if (history.captured != PIECE_NONE) {

		// reset half move clock since capture
		board.halfMoveClock = 0;
		board.removePiece<!side>(history.captured, target);
	}

	// move the piece
	board.movePiece<side>(history.moved, start, target);

	// if enPassant capture remove the captured piece
	if (moveType == ENPASSANT) {

		board.removePiece<!side>(PAWN, target + (side == WHITE ? -8 : 8));
        history.captured = PAWN;
	}

	// if promotion piece set the relevant bit to promoted piece
	else if (moveType == PROMOTION) {

		// remove moved pawn and add promotion piece
		board.removePiece<side>(PAWN, target);
		board.addPiece<side>(getPromotionPiece(move), target);
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
		board.movePiece<side>(ROOK, rookFrom, rookTo);
	}

	board.updateCombinedOccupancy();
	board.sideToMove = (side == WHITE) ? BLACK : WHITE;
}

template <Color side>
__attribute__((always_inline)) inline void unmakeMoveT(Board& board, History& history)
{
	Square start = getStartSq(history.move);
	Square target = getTargetSq(history.move);
	MoveType moveType = getMoveType(history.move);

    board.enPassantSq = history.enPassantSq;
    board.castlingRights = history.castlingRights;
    board.halfMoveClock = history.halfMoveClock;

    if (moveType == PROMOTION) {

		board.removePiece<!side>(board.findPiece<!side>(target), target);
		board.addPiece<!side>(PAWN, start);
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

		board.movePiece<!side>(ROOK, rookFrom, rookTo);
		board.movePiece<!side>(KING, target, start);
    }
	else {
		board.movePiece<!side>(history.moved, target, start);
	}

    if (history.captured != PIECE_NONE) {

		if (moveType != ENPASSANT) {
			board.addPiece<side>(history.captured, target);
        }
		else {
			board.addPiece<side>(PAWN, target + (side == WHITE ? 8 : -8));
        }
    }

	board.updateCombinedOccupancy();
    board.sideToMove = side == WHITE ? BLACK : WHITE;
}

void makeMove(int move, Board& board, History& history);
void unmakeMove(Board& board, History& history);