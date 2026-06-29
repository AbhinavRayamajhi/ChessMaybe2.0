#include "Position.h"

template <Color side>
Bitboard Position::squareAttackers(Square sq) {

	Bitboard attackers = 0ULL;
	Bitboard self = 1ULL << sq;

	// calculate all attacks from the sq and see if the corresponding enemy piece is in that attack
	attackers |= pawnLeftAttack<side, SQ_NONE>(self, board.pieces[!side][PAWN]);
	attackers |= pawnRightAttack<side, SQ_NONE>(self, board.pieces[!side][PAWN]);
	attackers |= getAttackMask<KNIGHT>(sq) & board.pieces[!side][KNIGHT];
	attackers |= getBishopAttacks(board.occupancy[BOTH], sq) & board.pieces[!side][BISHOP];
	attackers |= getRookAttacks(board.occupancy[BOTH], sq) & board.pieces[!side][ROOK];
	attackers |= getQueenAttacks(board.occupancy[BOTH], sq) & board.pieces[!side][QUEEN];
	attackers |= getAttackMask<KING>(sq)& board.pieces[!side][KING];

	return attackers;
}

template <Color side>
void Position::getCheckInfo() {

	Bitboard occ = board.occupancy[BOTH];

	Square kSq            = getLSB(board.pieces[side][KING]);
	Bitboard enemyB       = board.pieces[!side][BISHOP];
	Bitboard enemyR       = board.pieces[!side][ROOK];
	Bitboard enemyQ       = board.pieces[!side][QUEEN];
	Bitboard friends      = board.occupancy[side];
	Bitboard enemySliders = enemyB | enemyR | enemyQ;

	// checkers are all pieces attacking the king currently
	checkInfo.checkers = squareAttackers<side>(kSq);

	Bitboard rookAttacks = getRookAttacks(occ, kSq);
	Bitboard bishopAttacks = getBishopAttacks(occ, kSq);

	// get rid of any friendly piece in the rook rays from king sq and calculate again
	rookAttacks &= friends;
	occ ^= rookAttacks;
	rookAttacks = getRookAttacks(occ, kSq);

	// find enemy pinners if any
	rookAttacks &= enemyQ | enemyR;

	while (rookAttacks) {
		Square pinSq = popLSB(rookAttacks);
		checkInfo.pinned |= getRaysBetweenSquare(kSq, pinSq) & friends;
	}

	occ = board.occupancy[BOTH];

	// get rid of any friendly piece in the bishop rays from king sq and calculate again
	bishopAttacks &= friends;
	occ ^= bishopAttacks;
	bishopAttacks = getBishopAttacks(occ, kSq);

	// find enemy pinners if any
	bishopAttacks &= enemyQ | enemyB;

	while (bishopAttacks) {
		Square pinSq = popLSB(bishopAttacks);
		checkInfo.pinned |= getRaysBetweenSquare(kSq, pinSq) & friends;
	}

	// no checkers no check mask
	if (!checkInfo.checkers) {

		checkInfo.checkMask = FULL_BOARD;
	}
	// single check
	else if (popCount(ci.checkers) == 1) {

		Square checkerSq = getLSB(checkInfo.checkers);
		Bitboard mask = EMPTY_BOARD;
		setBit(mask, checkerSq);

		// to allow blocking moves for slider checks
		if (mask & enemySliders)
			mask |= getRaysBetweenSquare(kSq, checkerSq);

		checkInfo.checkMask = mask;
	}
	// multiple check only king move
	else {

		checkInfo.checkMask = EMPTY_BOARD;
	}
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