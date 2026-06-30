#include "MoveGen.h"

#include "Attacks.h"
#include "Magic.h"

namespace Engine {

	template <Color side, Square ePSq, bool captures>
	void generatePawnMoves(const Position& position, MoveList& moveList) {

		Bitboard pawns = position.getPiece<side, PAWN>();
		Bitboard enemy = position.getOccupancy<!side>();
		Bitboard empty = position.getEmpty();

		Square start, target;

		if constexpr (!captures) {
			Bitboard singlePush = pawnSinglePush<side>(pawns, empty);
			while (singlePush) {

				target = popLSB(singlePush);
				start = target - (side== WHITE ? 8 : -8);

				// promotion check
				if (getRankFromSquare(target) == 0 || getRankFromSquare(target) == 7) {
					moveList.addMove(createMove(start, target, QUEEN, PROMOTION));
					moveList.addMove(createMove(start, target, ROOK, PROMOTION));
					moveList.addMove(createMove(start, target, BISHOP, PROMOTION));
					moveList.addMove(createMove(start, target, KNIGHT, PROMOTION));
				}
				else {
					moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
				}
			}

			Bitboard doublePush = pawnDoublePush<side>(pawns, empty);
			while (doublePush) {

				target = popLSB(doublePush);
				start = target - (side == WHITE ? 16 : -16);

				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}

		Bitboard leftAttack = pawnLeftAttack<side, ePSq>(pawns, enemy);
		while (leftAttack) {

			target = popLSB(leftAttack);
			start = target - (side == WHITE ? 7 : -9);

			// promotion check
			if (getRankFromSquare(target) == 0 || getRankFromSquare(target) == 7) {

				moveList.addMove(createMove(start, target, QUEEN, PROMOTION));
				moveList.addMove(createMove(start, target, ROOK, PROMOTION));
				moveList.addMove(createMove(start, target, BISHOP, PROMOTION));
				moveList.addMove(createMove(start, target, KNIGHT, PROMOTION));
			}
			// enPassant
			else if (target == ePSq) {

				moveList.addMove(createMove(start, target, NO_PROMOTION, ENPASSANT));
			}
			else {

				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}

		Bitboard rightAttack = pawnRightAttack<side, ePSq>(pawns, enemy);
		while (rightAttack) {

			target = popLSB(rightAttack);
			start = target - (side == WHITE ? 9 : -7);

			// promotion check
			if (getRankFromSquare(target) == 0 || getRankFromSquare(target) == 7) {

				moveList.addMove(createMove(start, target, QUEEN, PROMOTION));
				moveList.addMove(createMove(start, target, ROOK, PROMOTION));
				moveList.addMove(createMove(start, target, BISHOP, PROMOTION));
				moveList.addMove(createMove(start, target, KNIGHT, PROMOTION));
			}
			// enPassant
			else if (target == ePSq) {

				moveList.addMove(createMove(start, target, NO_PROMOTION, ENPASSANT));
			}
			else {

				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// generate pseudo knight moves
	template <Color side, bool captures>
	void generateKnightMove(const Position& position, MoveList& moveList) {

		Bitboard knights = position.getPiece<side, KNIGHT>();
		Bitboard friends = position.getOccupancy<side>();
		Bitboard enemies = position.getOccupancy<!side>();

		while (knights) {
			
			Square start = popLSB(knights);
			Bitboard attacks;
			if constexpr (captures) attacks = getAttackMask<KNIGHT>(start) & enemies;
			else attacks = getAttackMask<KNIGHT>(start) & ~friends;

			while (attacks) {

				Square target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// generate pseudo king moves
	template <Color side, bool captures>
	void generateKingMove(const Position& position, MoveList& moveList) {

		Bitboard king    = position.getPiece<side, KING>();
		Bitboard friends = position.getOccupancy<side>();
		Bitboard enemies = position.getOccupancy<!side>();

		int start = getLSB(king);
		Bitboard attacks;
		if constexpr (captures) attacks =getAttackMask<KING>(start) & enemies;
		else attacks = getAttackMask<KING>(start) & ~friends;

		while (attacks) {

			int target = popLSB(attacks);
			moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
		}
	}

	// generate king castle moves
	template <CastlingRights castlingRights>
	void generateWhiteCastles(MoveList& moveList) {

		if (castlingRights == WHITE_OO)
			moveList.addMove(createMove(E1, G1, NO_PROMOTION, CASTLING));
		if (castlingRights == WHITE_OOO)
			moveList.addMove(createMove(E1, C1, NO_PROMOTION, CASTLING));
	}

	template <CastlingRights castlingRights>
	void generateBlackCastles(MoveList& moveList) {

		if (castlingRights == BLACK_OO)
			moveList.addMove(createMove(E8, G8, NO_PROMOTION, CASTLING));
		if (castlingRights == BLACK_OOO)
			moveList.addMove(createMove(E8, C8, NO_PROMOTION, CASTLING));
	}

	// generate pseudo bishop moves
	template <Color side, bool captures>
	void generateBishopMove(const Position& position, MoveList& moveList) {

		Bitboard bishops = position.getPiece<side, BISHOP>();
		Bitboard friends = position.getOccupancy<side>();
		Bitboard enemies = position.getOccupancy<!side>();
		Bitboard both =    position.getBothOccupancy();

		while (bishops) {
			
			Square start = popLSB(bishops);
			Bitboard attacks;
			if constexpr (captures) attacks = getBishopAttacks(both, start) & enemies;
			else attacks = getBishopAttacks(both, start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}


	// generate pseudo rook moves
	template <Color side, bool captures>
	void generateRookMove(const Position& position, MoveList& moveList) {
		
		Bitboard rooks   = position.getPiece<side, ROOK>();
		Bitboard friends = position.getOccupancy<side>();
		Bitboard enemies = position.getOccupancy<!side>();
		Bitboard both    = position.getBothOccupancy();

		while (rooks) {
			
			Square start = popLSB(rooks);
			Bitboard attacks;
			if constexpr (captures) attacks = getRookAttacks(both, start) & enemies;
			else attacks = getRookAttacks(both, start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// generate pseudo queen moves
	template <Color side, bool captures>
	void generateQueenMove(const Position& position, MoveList& moveList) {

		Bitboard queens  = position.getPiece<side, QUEEN>();
		Bitboard friends = position.getOccupancy<side>();
		Bitboard enemies = position.getOccupancy<!side>();
		Bitboard both    = position.getBothOccupancy();

		while (queens) {
			
			Square start = popLSB(queens);
			Bitboard attacks;
			if constexpr (captures) attacks = getQueenAttacks(both, start) & enemies;
			else attacks =  getQueenAttacks(both, start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// all pseudo moves
	template <Color side, Square ePSq, bool captures>
	void generatePseudoMove(const Position& position, MoveList& moveList) {

		generatePawnMoves<side, ePSq, captures>(position, moveList);
		generateKnightMove<side, captures>(position, moveList);
		generateKingMove<side, captures>(position, moveList);
		generateBishopMove<side, captures>(position, moveList);
		generateRookMove<side, captures>(position, moveList);
		generateQueenMove<side, captures>(position, moveList);
	}

	// gets rays between two squares from precalculated array
	inline Bitboard getRaysBetweenSquare(Square sq1, Square sq2) {
		return RAYS[sq1][sq2];
	}

	template <Color side>
	Bitboard squareAttackers(const Board& board, Square sq) {

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

	// checks if king is being attacked by enemy pieces
	Bitboard isKingInCheck(Position& position) {

		if (position.side() == WHITE) {
			Square kSq = getLSB(position.getPiece<WHITE, KING>());
			return squareAttackers<WHITE>(position.getBoard(), kSq);
		}
		else {
			Square kSq = getLSB(position.getPiece<BLACK, KING>());
			return squareAttackers<BLACK>(position.getBoard(), kSq);
		}
	}

	template <Color side>
	CheckInfo getCheckInfo(Position& position) {

		CheckInfo ci;
		Bitboard occ = position.getBothOccupancy();

		Square kSq            = getLSB(position.getPiece<side, KING>());
		Bitboard enemyB       = position.getPiece<!side, BISHOP>();
		Bitboard enemyR       = position.getPiece<!side, ROOK>();
		Bitboard enemyQ       = position.getPiece<!side, QUEEN>();
		Bitboard friends      = position.getOccupancy<side>();
		Bitboard enemySliders = enemyB | enemyR | enemyQ;

		// checkers are all pieces attacking the king currently
		ci.checkers = squareAttackers<side>(position.getBoard(), kSq);

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
			ci.pinned |= getRaysBetweenSquare(kSq, pinSq) & friends;
		}

		occ = position.getBothOccupancy();

		// get rid of any friendly piece in the bishop rays from king sq and calculate again
		bishopAttacks &= friends;
		occ ^= bishopAttacks;
		bishopAttacks = getBishopAttacks(occ, kSq);

		// find enemy pinners if any
		bishopAttacks &= enemyQ | enemyB;

		while (bishopAttacks) {
			Square pinSq = popLSB(bishopAttacks);
			ci.pinned |= getRaysBetweenSquare(kSq, pinSq) & friends;
		}

		// no checkers no check mask
		if (!ci.checkers) {

			ci.checkMask = FULL_BOARD;
		}
		// single check
		else if (popCount(ci.checkers) == 1) {

			Square checkerSq = getLSB(ci.checkers);
			Bitboard mask = EMPTY_BOARD;
			setBit(mask, checkerSq);

			// to allow blocking moves for slider checks
			if (mask & enemySliders)
				mask |= getRaysBetweenSquare(kSq, checkerSq);

			ci.checkMask = mask;
		}
		// multiple check only king move
		else {

			ci.checkMask = EMPTY_BOARD;
		}

		return ci;
	}

	// verifies if a given move is legal
	template <Color side>
	bool isLegalMove(Move move, Position& position) {

		Square start = getStartSq(move);
		Square target = getTargetSq(move);
		Square moveType = getMoveType(move);
		Square kSq = getLSB(position.getPiece<side, KING>());

		// King moves: target square must NOT be attacked
		if (start == kSq) {

			// We need to check a board without king to prevent x-ray moves from sneaking in
			position.removePiece<side>(KING, kSq);
			position.updateCombinedOccupancy();

			if (squareAttackers<side>(position.getBoard(), target)) {
				return false;
			}

			// add piece back
			position.addPiece<side>(KING, kSq);
			position.updateCombinedOccupancy();

			if (moveType == CASTLING)
			{
				// if check no castling
				if(squareAttackers<side>(position.getBoard(), start))
					return false;

				switch (target)
				{
					// nothing between king and rook and middle sq not checked
				case G1:
					return !(squareAttackers<side>(position.getBoard(), F1) || getBit(position.getBothOccupancy(), F1) ||
						getBit(position.getBothOccupancy(), G1));
				case C1:
					return !(squareAttackers<side>(position.getBoard(), D1) || getBit(position.getBothOccupancy(), D1) ||
						getBit(position.getBothOccupancy(), C1) || getBit(position.getBothOccupancy(), B1));
				case G8:
					return !(squareAttackers<side>(position.getBoard(), F8) || getBit(position.getBothOccupancy(), F8) ||
						getBit(position.getBothOccupancy(), G8));
				case C8:
					return !(squareAttackers<side>(position.getBoard(), D8) || getBit(position.getBothOccupancy(), D8) ||
						getBit(position.getBothOccupancy(), C8) || getBit(position.getBothOccupancy(), B8));
				}
			}
			return true;
		}

		// if double-check, only king moves are allowed
		if (popCount(position.getCheckers()) >= 2) return false;

		// En-passant: simulate the capture and ensure king is not left in check
		if (moveType == ENPASSANT) {

			Board b = position.getBoard();

			b.movePiece<side>(PAWN, start, b.enPassantSq);
			b.removePiece<!side>(PAWN, side == WHITE ? b.enPassantSq - 8 : b.enPassantSq + 8);
			b.updateCombinedOccupancy();

			// if king is attacked after en-passant, the move is illegal else legal
			if (squareAttackers<side>(b, kSq)) return false;
			else return true;
		}

		// If single check, move must block or capture the checker 
		if (!getBit(position.getCheckMask(), target)) return false;

		// If piece is pinned, it must move along the ray between king and target or the target must be between king
		// and current
		if (getBit(position.getPinned(), start))
		{
			if (!getBit(getRaysBetweenSquare(kSq, target), start) &&
				!getBit(getRaysBetweenSquare(kSq, start), target))
				return false;
		}
		
		return true;
	}

	// calculate MVV_LVA scores
	template<Color side>
	int calculateScore(const Position& position, Move m) {

		Square attackerSq = getStartSq(m);
		Square victimSq = getTargetSq(m);

		Piece attacker = PIECE_NONE;
		Piece victim = PIECE_NONE;

		for (Piece p = PAWN; p != PIECE_COUNT; ++p) {
			if (getBit(position.getPiece<side>(p), attackerSq)) {
				attacker = p;
			}
			if (getBit(position.getPiece<!side>(p), victimSq)) {
				victim = p;
			}
		}

		if (victim == PIECE_NONE) return 0;
		return materialVal[victim] * 10 - materialVal[attacker];
	}

	// generate all legal moves by passing all pseduo moves through is legal function
	template <bool ScoreMoves, bool captures>
	void generateLegalMoves(Position& position, MoveList& moveList)
	{
		if (position.side() == WHITE) {

			position.setCheckInfo(getCheckInfo<WHITE>(position));

			switch (position.epSq()) {

			case A6: generatePseudoMove<WHITE, A6, captures>(position, moveList);  break;
			case B6: generatePseudoMove<WHITE, B6, captures>(position, moveList);  break;
			case C6: generatePseudoMove<WHITE, C6, captures>(position, moveList);  break;
			case D6: generatePseudoMove<WHITE, D6, captures>(position, moveList);  break;
			case E6: generatePseudoMove<WHITE, E6, captures>(position, moveList);  break;
			case F6: generatePseudoMove<WHITE, F6, captures>(position, moveList);  break;
			case G6: generatePseudoMove<WHITE, G6, captures>(position, moveList);  break;
			case H6: generatePseudoMove<WHITE, H6, captures>(position, moveList);  break;
			default: generatePseudoMove<WHITE, SQ_NONE, captures>(position, moveList);  break;
			}

			if constexpr (!captures) {
				if (position.getCR() & WHITE_OO) {
					generateWhiteCastles<WHITE_OO>(moveList);
				}
				if (position.getCR() & WHITE_OOO) {
					generateWhiteCastles<WHITE_OOO>(moveList);
				}
			}

			int index = 0;
			while(index < moveList.end) {

				if (!isLegalMove<WHITE>(moveList.list[index], position)) {
					moveList.removeMove(index);
				}
				else {
					if (ScoreMoves) moveList.score[index] = calculateScore<WHITE>(position, moveList.list[index]);
					++index;
				}
			}
		}
		else {
			position.setCheckInfo(getCheckInfo<BLACK>(position));
			switch (position.epSq()) {

			case A3: generatePseudoMove<BLACK, A3, captures>(position, moveList);  break;
			case B3: generatePseudoMove<BLACK, B3, captures>(position, moveList);  break;
			case C3: generatePseudoMove<BLACK, C3, captures>(position, moveList);  break;
			case D3: generatePseudoMove<BLACK, D3, captures>(position, moveList);  break;
			case E3: generatePseudoMove<BLACK, E3, captures>(position, moveList);  break;
			case F3: generatePseudoMove<BLACK, F3, captures>(position, moveList);  break;
			case G3: generatePseudoMove<BLACK, G3, captures>(position, moveList);  break;
			case H3: generatePseudoMove<BLACK, H3, captures>(position, moveList);  break;
			default: generatePseudoMove<BLACK, SQ_NONE, captures>(position, moveList);  break;
			}

			if constexpr (!captures) {
				if (position.getCR() & BLACK_OO) {
					generateBlackCastles<BLACK_OO>(moveList);
				}
				if (position.getCR() & BLACK_OOO) {
					generateBlackCastles<BLACK_OOO>(moveList);
				}
			}

			int index = 0;
			while(index < moveList.end) {

				if (!isLegalMove<BLACK>(moveList.list[index], position)) {
					moveList.removeMove(index);
					}
				else {
					if (ScoreMoves) moveList.score[index] = calculateScore<BLACK>(position, moveList.list[index]);
					++index;
				}
			}
		}
	}
	
	template void generateLegalMoves<true, true>(Position& position, MoveList& moveList);
	template void generateLegalMoves<true, false>(Position& position, MoveList& moveList);
	template void generateLegalMoves<false, true>(Position& position, MoveList& moveList);
	template void generateLegalMoves<false, false>(Position& position, MoveList& moveList);
}
