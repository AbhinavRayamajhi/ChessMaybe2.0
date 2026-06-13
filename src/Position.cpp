#include "Position.h"

void makeMove(int move, Board& board, History& history) {

	Square start = getStartSq(move);
	Square target = getTargetSq(move);
	Piece promotionPiece = getPromotionPiece(move);
	MoveType moveType = getMoveType(move);

	int side = board.sideToMove == WHITE ? 1 : 0;

	history.move = move;
    history.enPassantSq = board.enPassantSq;
    history.castlingRights = board.castlingRights;
	history.halfMoveClock = board.halfMoveClock;
	// reset enPassant sq
	board.enPassantSq = SQ_NONE;

	// set castling right to false if not moved rook is target
	switch (target)
	{
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

	for (Piece piece = PAWN; piece < PIECE_COUNT; ++piece) {

		if (getBit(board.pieces[side][piece], start)) {

			history.moved = piece;

			// move piece
			flipBit(board.pieces[board.sideToMove][piece], (1ULL << start) | (1ULL << target));

			if (piece == PAWN) {
				// set en passant sq if double push
				if (start + 16 == target) board.enPassantSq = start + 8;
                else if (start - 16 == target) board.enPassantSq = start - 8;
				// reset half move clock since pawn move
				board.halfMoveClock = 0;
			}

			// king move cancels all castling
			if (piece == KING) {

				if (side == WHITE) board.castlingRights &= ~BLACK_CASTLE;
				else board.castlingRights &= ~WHITE_CASTLE;
			}
		}
		if (getBit(board.pieces[!side][piece], target)) {

			history.captured = piece;
			// reset half move clock since capture
			board.halfMoveClock = 0;
			// delete captured piece
			flipBit(board.pieces[!side][piece], 1ULL << target);
		}		
	}

	// if enPassant capture remove the captured piece
	if (moveType == ENPASSANT) {
		flipBit(board.pieces[!side][PAWN], 1ULL << (start + (side == WHITE? 8 : -8)));
        history.captured = PAWN;
	}

	// if promotion piece set the relevant bit to promoted piece
	else if (moveType == PROMOTION) {
		// remove moved pawn
        resetBit(board.pieces[side][PAWN], target);
        setBit(board.pieces[side][getPromotionPiece(move)], target);
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
        flipBit(board.pieces[side][ROOK], (1ULL << rookFrom) | (1ULL << rookTo));
	}

	updateOccupancy(board);
	board.sideToMove = side == WHITE ? BLACK : WHITE;
}

void unmakeMove(Board& board, History& history)
{
	Square start = getStartSq(history.move);
	Square target = getTargetSq(history.move);

	int side = board.sideToMove == WHITE ? 1 : 0;
	MoveType moveType = getMoveType(history.move);

    board.enPassantSq = history.enPassantSq;
    board.castlingRights = history.castlingRights;
    board.halfMoveClock = history.halfMoveClock;

    if (history.captured != NO_PIECE) {

        if (moveType != ENPASSANT) {
            flipBit(board.pieces[side][history.captured], 1ULL << target);
        }
        else {
            flipBit(board.pieces[side][PAWN], 1ULL << (start + (side == WHITE ? -8 : 8)));
        }
    }

    if (moveType == PROMOTION) {

        resetBit(board.pieces[!side][getPromotionPiece(history.move)], target);
        setBit(board.pieces[!side][PAWN], start);
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

        flipBit(board.pieces[!side][ROOK], (1ULL << rookFrom) | (1ULL << rookTo));
        flipBit(board.pieces[!side][KING], (1ULL << start) | (1ULL << target));
    }
    else {
        flipBit(board.pieces[!side][history.moved], (1ULL << start) | (1ULL << target));
	}
	
	updateOccupancy(board);
    board.sideToMove = side == WHITE ? BLACK : WHITE;
}