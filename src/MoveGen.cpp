#include "MoveGen.h"

namespace Engine {

	// bishop and rook attack arrays
	std::array<Bitboard, 88772> BISHOP_ATTACKS{ 0 };
	std::array<Bitboard, 88772> ROOK_ATTACKS{ 0 };

	// pawn pushes
	template <Color sideToMove>
	inline Bitboard pawnSinglePush(Bitboard pawns, Bitboard empty) {

		if (sideToMove == WHITE)
			return (pawns << 8) & empty;
		else
			return (pawns >> 8) & empty;
	}

	template <Color sideToMove>
	inline Bitboard pawnDoublePush(Bitboard pawns, Bitboard empty) {

		if (sideToMove == WHITE) {

			pawns = pawnSinglePush<WHITE>(pawns, empty);
			return ((pawns & RANK_3) << 8) & empty;
		}
		else {

			pawns = pawnSinglePush<BLACK>(pawns, empty);
			return ((pawns & RANK_6) >> 8) & empty;
		}
	}

	// pawn attacks
	template <Color sideToMove, Square ePSq>
	inline Bitboard pawnLeftAttack(Bitboard pawns, Bitboard enemy) {

		// set enPassantSq in enemy to 1, if not -1, so pawns can attack it
		if (ePSq != SQ_NONE) setBit(enemy, ePSq);

		if (sideToMove == WHITE)
			return (pawns << 7) & ~FILE_H & enemy;
		else
			return (pawns >> 9) & ~FILE_H & enemy;
	}

	template <Color sideToMove, Square ePSq>
	inline Bitboard pawnRightAttack(Bitboard pawns, Bitboard enemy) {

		// set enPassantSq in enemy to 1, if not -1, so pawns can attack it
		if (ePSq != SQ_NONE) setBit(enemy, ePSq);

		if (sideToMove == WHITE)
			return (pawns << 9) & ~FILE_A & enemy;
		else
			return (pawns >> 7) & ~FILE_A & enemy;
	}

	// generate pseudo pawn pushes
	template <Color sideToMove, Square ePSq>
	void generatePawnMoves(const Board& board, MoveList& moveList) {

		Bitboard pawns = sideToMove == WHITE ? board.pieces[WHITE][PAWN] : board.pieces[BLACK][PAWN];
		Bitboard enemy = sideToMove == WHITE ? board.occupancy[BLACK] : board.occupancy[WHITE];
		Bitboard empty = ~board.occupancy[BOTH];

		Square start, target;

		Bitboard singlePush = pawnSinglePush<sideToMove>(pawns, empty);
		while (singlePush) {

			target = popLSB(singlePush);
			start = target - (sideToMove == WHITE ? 8 : -8);

			// promotion check
			if (getRankFromInt(target) == 0 || getRankFromInt(target) == 7) {
				moveList.addMove(createMove(start, target, QUEEN, PROMOTION));
				moveList.addMove(createMove(start, target, ROOK, PROMOTION));
				moveList.addMove(createMove(start, target, BISHOP, PROMOTION));
				moveList.addMove(createMove(start, target, KNIGHT, PROMOTION));
			}
			else {
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}

		Bitboard doublePush = pawnDoublePush<sideToMove>(pawns, empty);
		while (doublePush) {
			
			target = popLSB(doublePush);
			start = target - (sideToMove == WHITE ? 16 : -16);

			moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
		}

		Bitboard leftAttack = pawnLeftAttack<sideToMove, ePSq>(pawns, enemy);
		while (leftAttack) {

			target = popLSB(leftAttack);
			start = target - (sideToMove == WHITE ? 7 : -9);

			// promotion check
			if (getRankFromInt(target) == 0 || getRankFromInt(target) == 7) {

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

		Bitboard rightAttack = pawnRightAttack<sideToMove, ePSq>(pawns, enemy);
		while (rightAttack) {

			target = popLSB(rightAttack);
			start = target - (sideToMove == WHITE ? 9 : -7);

			// promotion check
			if (getRankFromInt(target) == 0 || getRankFromInt(target) == 7) {

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
	template <Color sideToMove>
	void generateKnightMove(const Board& board, MoveList& moveList) {

		Bitboard knights = sideToMove == WHITE? board.pieces[WHITE][KNIGHT] : board.pieces[BLACK][KNIGHT];
		Bitboard friends = sideToMove == WHITE? board.occupancy[WHITE] : board.occupancy[BLACK];

		while (knights) {
			
			Square start = popLSB(knights);
			Bitboard attacks = KNIGHT_ATTACKS[start] & ~friends;

			while (attacks) {

				Square target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// generate pseudo king moves
	template <Color sideToMove>
	void generateKingMove(const Board& board, MoveList& moveList) {

		Bitboard king = sideToMove == WHITE ? board.pieces[WHITE][KING] : board.pieces[BLACK][KING];
		Bitboard friends = sideToMove == WHITE ? board.occupancy[WHITE] : board.occupancy[BLACK];

		int start = getLSB(king);
		Bitboard attacks = KING_ATTACKS[start] & ~friends;

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

	// retrieve bishop moves using magic 
	inline Bitboard getBishopAttacks(Bitboard occ, int sq) {

		occ &= BISHOP_MASKS[sq];
		uint64_t index = ((occ * BISHOP_MAGICS[sq]) >> 55) + BISHOP_OFFSETS[sq];
		return BISHOP_ATTACKS[index];
	}

	// generate pseudo bishop moves
	template <Color sideToMove>
	void generateBishopMove(const Board& board, MoveList& moveList) {

		Bitboard bishops = sideToMove == WHITE? board.pieces[WHITE][BISHOP] : board.pieces[BLACK][BISHOP];
		Bitboard friends = sideToMove == WHITE? board.occupancy[WHITE] : board.occupancy[BLACK];

		while (bishops) {
			
			Square start = popLSB(bishops);
			Bitboard attacks = getBishopAttacks(board.occupancy[BOTH], start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// Rook

	// retrieve rook moves using magic 
	inline Bitboard getRookAttacks(Bitboard occ, int sq) {

		occ &= ROOK_MASKS[sq];
		uint64_t index = ((occ * ROOK_MAGICS[sq]) >> 52) + ROOK_OFFSETS[sq];
		return ROOK_ATTACKS[index];
	}

	// generate pseudo bishop moves
	template <Color sideToMove>
	void generateRookMove(const Board& board, MoveList& moveList) {
		
		Bitboard rooks = sideToMove == WHITE ? board.pieces[WHITE][ROOK] : board.pieces[BLACK][ROOK];
		Bitboard friends = sideToMove == WHITE? board.occupancy[WHITE] : board.occupancy[BLACK];

		while (rooks) {
			
			Square start = popLSB(rooks);
			Bitboard attacks = getRookAttacks(board.occupancy[BOTH], start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// retrieve queen moves from rook and bishop 
	inline Bitboard getQueenAttacks(Bitboard occ, int sq)
	{
		return getRookAttacks(occ, sq) | getBishopAttacks(occ, sq);
	}

	// generate pseudo queen moves
	template <Color sideToMove>
	void generateQueenkMove(const Board& board, MoveList& moveList) {

		Bitboard queens = sideToMove == WHITE ? board.pieces[WHITE][QUEEN] : board.pieces[BLACK][QUEEN];
		Bitboard friends = sideToMove == WHITE? board.occupancy[WHITE] : board.occupancy[BLACK];
		
		while (queens) {
			
			Square start = popLSB(queens);
			Bitboard attacks = getQueenAttacks(board.occupancy[BOTH], start) & ~friends;

			while (attacks) {

				int target = popLSB(attacks);
				moveList.addMove(createMove(start, target, NO_PROMOTION, NORMAL));
			}
		}
	}

	// all pseudo moves
	template <Color sideToMove, Square ePSq>
	void generatePseudoMove(const Board& board, MoveList& moveList) {

		generatePawnMoves<sideToMove, ePSq>(board, moveList);
		generateKnightMove<sideToMove>(board, moveList);
		generateKingMove<sideToMove>(board, moveList);
		generateBishopMove<sideToMove>(board, moveList);
		generateRookMove<sideToMove>(board, moveList);
		generateQueenkMove<sideToMove>(board, moveList);
	}

	// checks if a particular square is being attacked by enemy pieces
	template <Color sideToMove>
	inline Bitboard squareAttackers(const Board& board, Square sq) {

		Bitboard attackers = 0ULL;
		Bitboard self = 1ULL << sq;
		Bitboard enemyP = sideToMove == WHITE ? board.pieces[BLACK][PAWN] : board.pieces[WHITE][PAWN];
		Bitboard enemyN = sideToMove == WHITE ? board.pieces[BLACK][KNIGHT] : board.pieces[WHITE][KNIGHT];
		Bitboard enemyB = sideToMove == WHITE ? board.pieces[BLACK][BISHOP] : board.pieces[WHITE][BISHOP];
		Bitboard enemyR = sideToMove == WHITE ? board.pieces[BLACK][ROOK]   : board.pieces[WHITE][ROOK];
		Bitboard enemyQ = sideToMove == WHITE ? board.pieces[BLACK][QUEEN]  : board.pieces[WHITE][QUEEN];
		Bitboard enemyK = sideToMove == WHITE ? board.pieces[BLACK][KING]   : board.pieces[WHITE][KING];
		
		// calculate all attacks from the sq and see if the corresponding enemy piece is in that attack
		attackers |= pawnLeftAttack<sideToMove, SQ_NONE>(self, enemyP);
		attackers |= pawnRightAttack<sideToMove, SQ_NONE>(self, enemyP);
		attackers |= KNIGHT_ATTACKS[sq] & enemyN;
		attackers |= getBishopAttacks(board.occupancy[BOTH], sq) & enemyB;
		attackers |= getRookAttacks(board.occupancy[BOTH], sq) & enemyR;
		attackers |= getQueenAttacks(board.occupancy[BOTH], sq) & enemyQ;
		attackers |= KING_ATTACKS[sq] & enemyK;

		return attackers;
	}

	// checks if king is being attacked by enemy pieces
	Bitboard isKingInCheck(const Board& board) {

		if (board.sideToMove == WHITE) {
			Square kSq = getLSB(board.pieces[WHITE][KING]);
			return squareAttackers<WHITE>(board, kSq);
		}
		else {
			Square kSq = getLSB(board.pieces[BLACK][KING]);
			return squareAttackers<BLACK>(board, kSq);
		}
	}

	// gets rays between two squares from precalculated array
	inline Bitboard getRaysBetweenSquare(Square sq1, Square sq2) {
		return RAYS[sq1][sq2];
	}

	// calculates check info, pinned pieces, checkers and check masks
	template <Color sideToMove>
	inline CheckInfo getCheckInfo(const Board& board) {

		CheckInfo ci;
		Bitboard occ = board.occupancy[BOTH];

		Square kSq = sideToMove == WHITE ? getLSB(board.pieces[WHITE][KING]) : getLSB(board.pieces[BLACK][KING]);
		Bitboard enemyB   = sideToMove == WHITE ? board.pieces[BLACK][BISHOP]: board.pieces[WHITE][BISHOP];
		Bitboard enemyR   = sideToMove == WHITE ? board.pieces[BLACK][ROOK] : board.pieces[WHITE][ROOK];
		Bitboard enemyQ   = sideToMove == WHITE ? board.pieces[BLACK][QUEEN]: board.pieces[WHITE][QUEEN];
		Bitboard friends  = sideToMove == WHITE ? board.occupancy[WHITE] : board.occupancy[BLACK];
		Bitboard enemySliders = enemyB | enemyR | enemyQ;

		// checkers are all pieces attacking the king currently
		ci.checkers = squareAttackers<sideToMove>(board, kSq);

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

		occ = board.occupancy[BOTH];

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
	template <Color sideToMove>
	bool isLegalMove(Move move, const Board& board, const CheckInfo& ci) {

		Square kSq = sideToMove ? getLSB(board.pieces[WHITE][KING]) : getLSB(board.pieces[BLACK][KING]);

		// King moves: target square must NOT be attacked
		if (getStartSq(move) == kSq) {
			// We need to check a board without king to prevent x-ray moves from sneaking in
			Board b = board;
			b.removePiece<sideToMove>(KING, kSq);
			b.updateCombinedOccupancy();

			if (squareAttackers<sideToMove>(b, getTargetSq(move))) {
				return false;
			}
				
			if (getMoveType(move) == CASTLING)
			{
				// if check no castling
				if(squareAttackers<sideToMove>(board, getStartSq(move)))
					return false;

				switch (getTargetSq(move))
				{
					// nothing between king and rook and middle sq not checked
				case G1:
					return !(squareAttackers<sideToMove>(board, F1) || getBit(board.occupancy[BOTH], F1) ||
						getBit(board.occupancy[BOTH], G1));
				case C1:
					return !(squareAttackers<sideToMove>(board, D1) || getBit(board.occupancy[BOTH], D1) ||
						getBit(board.occupancy[BOTH], C1) || getBit(board.occupancy[BOTH], B1));
				case G8:
					return !(squareAttackers<sideToMove>(board, F8) || getBit(board.occupancy[BOTH], F8) ||
						getBit(board.occupancy[BOTH], G8));
				case C8:
					return !(squareAttackers<sideToMove>(board, D8) || getBit(board.occupancy[BOTH], D8) ||
						getBit(board.occupancy[BOTH], C8) || getBit(board.occupancy[BOTH], B8));
				}
			}
			return true;
		}

		// if double-check, only king moves are allowed
		if (popCount(ci.checkers) >= 2) return false;

		// En-passant: simulate the capture and ensure king is not left in check
		if (getMoveType(move) == ENPASSANT) {

			Board b = board;

			b.movePiece<sideToMove>(PAWN, getStartSq(move), board.enPassantSq);
			b.removePiece<!sideToMove>(PAWN, sideToMove == WHITE ? board.enPassantSq - 8 : board.enPassantSq + 8);
			b.updateCombinedOccupancy();

			// if king is attacked after en-passant, the move is illegal else legal
			if (squareAttackers<sideToMove>(b, kSq)) return false;
			else return true;
		}

		// If single check, move must block or capture the checker 
		if (!getBit(ci.checkMask, getTargetSq(move))) return false;

		// If piece is pinned, it must move along the ray between king and target or the target must be between king
		// and current
		if (getBit(ci.pinned, getStartSq(move)))
		{
			if (!getBit(getRaysBetweenSquare(kSq, getTargetSq(move)), getStartSq(move)) &&
				!getBit(getRaysBetweenSquare(kSq, getStartSq(move)), getTargetSq(move)))
				return false;
		}
		
		return true;
	}

	// calculate MVV_LVA scores
	template<Color side>
	int calculateScore(const Board& board, Move m) {

		Square attackerSq = getStartSq(m);
		Square victimSq = getTargetSq(m);

		Piece attacker = PIECE_NONE;
		Piece victim = PIECE_NONE;

		for (Piece p = PAWN; p != PIECE_COUNT; ++p) {
			if (getBit(board.pieces[side][p], attackerSq)) {
				attacker = p;
			}
			if (getBit(board.pieces[!side][p], victimSq)) {
				victim = p;
			}
		}

		if (victim == PIECE_NONE) return 0;
		return materialVal[victim] * 10 - materialVal[attacker];
	}

	// generate all legal moves by passing all pseduo moves through is legal function
	template <bool ScoreMoves>
	void generateLegalMoves(const Board& board, MoveList& moveList)
	{
		CheckInfo ci;

		if (board.sideToMove == WHITE) {

			ci = getCheckInfo<WHITE>(board);

			switch (board.enPassantSq) {

			case A6: generatePseudoMove<WHITE, A6>(board, moveList);  break;
			case B6: generatePseudoMove<WHITE, B6>(board, moveList);  break;
			case C6: generatePseudoMove<WHITE, C6>(board, moveList);  break;
			case D6: generatePseudoMove<WHITE, D6>(board, moveList);  break;
			case E6: generatePseudoMove<WHITE, E6>(board, moveList);  break;
			case F6: generatePseudoMove<WHITE, F6>(board, moveList);  break;
			case G6: generatePseudoMove<WHITE, G6>(board, moveList);  break;
			case H6: generatePseudoMove<WHITE, H6>(board, moveList);  break;
			default: generatePseudoMove<WHITE, SQ_NONE>(board, moveList);  break;
			}

			if (board.castlingRights & WHITE_OO) {
				generateWhiteCastles<WHITE_OO>(moveList);
			}
			if (board.castlingRights & WHITE_OOO) {
				generateWhiteCastles<WHITE_OOO>(moveList);
			}

			int index = 0;
			while(index < moveList.end) {

				if (!isLegalMove<WHITE>(moveList.list[index], board, ci)) {
					moveList.removeMove(index);
				}
				else {
					if (ScoreMoves) moveList.score[index] = calculateScore<WHITE>(board, moveList.list[index]);
					++index;
				}
			}
		}
		else {
			ci = getCheckInfo<BLACK>(board);
			switch (board.enPassantSq) {

			case A3: generatePseudoMove<BLACK, A3>(board, moveList);  break;
			case B3: generatePseudoMove<BLACK, B3>(board, moveList);  break;
			case C3: generatePseudoMove<BLACK, C3>(board, moveList);  break;
			case D3: generatePseudoMove<BLACK, D3>(board, moveList);  break;
			case E3: generatePseudoMove<BLACK, E3>(board, moveList);  break;
			case F3: generatePseudoMove<BLACK, F3>(board, moveList);  break;
			case G3: generatePseudoMove<BLACK, G3>(board, moveList);  break;
			case H3: generatePseudoMove<BLACK, H3>(board, moveList);  break;
			default: generatePseudoMove<BLACK, SQ_NONE>(board, moveList);  break;
			}

			if (board.castlingRights & BLACK_OO) {
				generateBlackCastles<BLACK_OO>(moveList);
			}
			if (board.castlingRights & BLACK_OOO) {
				generateBlackCastles<BLACK_OOO>(moveList);
			}

			int index = 0;
			while(index < moveList.end) {

				if (!isLegalMove<BLACK>(moveList.list[index], board, ci)) {
					moveList.removeMove(index);
					}
				else {
					if (ScoreMoves) moveList.score[index] = calculateScore<BLACK>(board, moveList.list[index]);
					++index;
				}
			}
		}
	}

	
	template void generateLegalMoves<true>(const Board& board, MoveList& moveList);
	template void generateLegalMoves<false>(const Board& board, MoveList& moveList);
}
