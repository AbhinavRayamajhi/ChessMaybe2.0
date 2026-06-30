#include "Position.h"


template <Color side>
__attribute__((always_inline)) inline void makeMoveT(int move, Position& position, History& history) {

	Square start = getStartSq(move);
	Square target = getTargetSq(move);
	MoveType moveType = getMoveType(move);

	history.move = move;
    history.enPassantSq = position.epSq();
    history.castlingRights = position.cr();
	history.halfMoveClock = position.hclock();

	position.setEpSq(SQ_NONE);

	// set castling right to false if not moved rook is target
	switch (target) {
		
		case H1:
			position.setCastlingRights(history.castlingRights & WHITE_OO); break;
		case A1:
			position.setCastlingRights(history.castlingRights & WHITE_OOO); break;
		case H8:
			position.setCastlingRights(history.castlingRights & BLACK_OO); break;
		case A8:
			position.setCastlingRights(history.castlingRights & BLACK_OOO); break;
		default: break;
	}

	// set castling right to false if anything moves from rook sq
	switch (start) {
		
		case H1:
			position.setCastlingRights(history.castlingRights & WHITE_OO); break;
		case A1:
			position.setCastlingRights(history.castlingRights & WHITE_OOO); break;
		case H8:
			position.setCastlingRights(history.castlingRights & BLACK_OO); break;
		case A8:
			position.setCastlingRights(history.castlingRights & BLACK_OOO); break;
		default: break;
	}

	history.moved = position.findPiece<side>(start);
	if (history.moved == PAWN) {

		// set en passant sq if double push
		if (start + 16 == target) position.setEpSq(start + 8);
		else if (start - 16 == target) position.setEpSq(start - 8);

		// reset half move clock since pawn move
		position.setHalfMoveClock(0);
	}
	// king move cancels all castling
	else if (history.moved == KING) {

		position.setCastlingRights(position.cr() & ((side == WHITE) ? ~WHITE_CASTLE : ~BLACK_CASTLE));
	}

	history.captured = position.findPiece<!side>(target);
	if (history.captured != PIECE_NONE) {

		// reset half move clock since capture
		position.setHalfMoveClock(0);
		position.removePiece<!side>(history.captured, target);
	}

	// move the piece
	position.movePiece<side>(history.moved, start, target);

	// if enPassant capture remove the captured piece
	if (moveType == ENPASSANT) {

		position.removePiece<!side>(PAWN, target + (side == WHITE ? -8 : 8));
        history.captured = PAWN;
	}

	// if promotion piece set the relevant bit to promoted piece
	else if (moveType == PROMOTION) {

		// remove moved pawn and add promotion piece
		position.removePiece<side>(PAWN, target);
		position.addPiece<side>(getPromotionPiece(move), target);
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
		position.movePiece<side>(ROOK, rookFrom, rookTo);
	}

	position.updateCombinedOccupancy();
	position.flipSide();
}

template <Color side>
__attribute__((always_inline)) inline void unmakeMoveT(Position& position, History& history)
{
	Square start = getStartSq(history.move);
	Square target = getTargetSq(history.move);
	MoveType moveType = getMoveType(history.move);

    position.setEpSq(history.enPassantSq);
    position.setCastlingRights(history.castlingRights);
    position.setHalfMoveClock(history.halfMoveClock);

    if (moveType == PROMOTION) {

		position.removePiece<!side>(position.findPiece<!side>(target), target);
		position.addPiece<!side>(PAWN, start);
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

		position.movePiece<!side>(ROOK, rookFrom, rookTo);
		position.movePiece<!side>(KING, target, start);
    }
	else {
		position.movePiece<!side>(history.moved, target, start);
	}

    if (history.captured != PIECE_NONE) {

		if (moveType != ENPASSANT) {
			position.addPiece<side>(history.captured, target);
        }
		else {
			position.addPiece<side>(PAWN, target + (side == WHITE ? 8 : -8));
        }
    }

	position.updateCombinedOccupancy();
	position.flipSide();
}

void makeMove(int move, Position& position, History& history) {

	if (position.side()== WHITE) {
		makeMoveT<WHITE>(move, position, history);
	}
	else {
		makeMoveT<BLACK>(move, position, history);
	}
}

void unmakeMove(Position& position, History& history) {

	if (position.side() == WHITE) {
		unmakeMoveT<WHITE>(position, history);
	}
	else {
		unmakeMoveT<BLACK>(position, history);
	}
}