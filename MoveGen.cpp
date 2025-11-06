#include "MoveGen.h"

namespace MoveGen
{
	// bishop and rook attack arrays
	std::array<Bitboard, 88772> BISHOP_ATTACKS{ 0 };
	std::array<Bitboard, 88772> ROOK_ATTACKS{ 0 };


	// pawn

	// pawn pushes
	template <bool sideToMove>
	Bitboard pawnSinglePush(Bitboard pawns, Bitboard empty)
	{
		if (sideToMove == White)
			return (pawns << 8) & empty;
		else
			return (pawns >> 8) & empty;
	}

	template <bool sideToMove>
	Bitboard pawnDoublePush(Bitboard pawns, Bitboard empty)
	{
		if (sideToMove == White)
		{
			pawns = pawnSinglePush<White>(pawns, empty);
			return ((pawns & RANK_3) << 8) & empty;
		}
		else
		{
			pawns = pawnSinglePush<Black>(pawns, empty);
			return ((pawns & RANK_6) >> 8) & empty;
		}
	}

	// pawn attacks
	template <bool sideToMove, int ePSq>
	Bitboard pawnLeftAttack(Bitboard pawns, Bitboard enemy)
	{
		// set enPassantSq in enemy to 1, if not -1, so pawns can attack it
		if (ePSq != -1) setBit(enemy, ePSq);

		if (sideToMove == White)
			return (pawns << 7) & ~FILE_H & enemy;
		else
			return (pawns >> 9) & ~FILE_H & enemy;
	}

	template <bool sideToMove, int ePSq>
	Bitboard pawnRightAttack(Bitboard pawns, Bitboard enemy)
	{
		// set enPassantSq in enemy to 1, if not -1, so pawns can attack it
		if (ePSq != -1) setBit(enemy, ePSq);

		if (sideToMove == White)
			return (pawns << 9) & ~FILE_A & enemy;
		else
			return (pawns >> 7) & ~FILE_A & enemy;
	}

	template <bool sideToMove>
	Bitboard pawnLeftAttacksFromSquare(int sq, Bitboard enemy)
	{
		if (sideToMove == White)
			return (1ULL << (sq + 7)) & ~FILE_H & enemy;
		else
			return (1ULL << (sq - 9)) & ~FILE_H & enemy;
	}

	template <bool sideToMove>
	Bitboard pawnRightAttacksFromSquare(int sq, Bitboard enemy)
	{
		if (sideToMove == White)
			return (1ULL << (sq + 9)) & ~FILE_A & enemy;
		else
			return (1ULL << (sq - 7)) & ~FILE_A & enemy;
	}

	// generate pseudo pawn pushes
	template <bool sideToMove, int ePSq>
	int* generatePawnMoves(const Board& board, int* moveList)
	{
		Bitboard pawns = sideToMove ? board.pieces[WhitePawn] : board.pieces[BlackPawn];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];
		Bitboard empty = ~board.occupancy[Both];

		Bitboard singlePush = pawnSinglePush<sideToMove>(pawns, empty);
		while (singlePush)
		{
			int to = lsb(singlePush);
			clearLSB(singlePush);
			int from = to - (sideToMove ? 8 : -8);

			// promotion check
			if (getRankFromInt(to) == 0 || getRankFromInt(to) == 7)
			{
				*moveList++ = createMove(from, to, Pawn, Knight, false, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Bishop, false, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Rook, false, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Queen, false, false, false, false);
			}
			else
			{
				*moveList++ = createMove(from, to, Pawn, None, false, false, false, false);
			}
		}

		Bitboard doublePush = pawnDoublePush<sideToMove>(pawns, empty);
		while (doublePush)
		{
			int to = lsb(doublePush);
			clearLSB(doublePush);
			int from = to - (sideToMove ? 16 : -16);

			*moveList++ = createMove(from, to, Pawn, None, false, true, false, false);
		}

		Bitboard leftAttack = pawnLeftAttack<sideToMove, ePSq>(pawns, enemy);
		while (leftAttack)
		{
			int to = lsb(leftAttack);
			clearLSB(leftAttack);
			int from = to - (sideToMove ? 7 : -9);

			// promotion check
			if (getRankFromInt(to) == 0 || getRankFromInt(to) == 7)
			{
				*moveList++ = createMove(from, to, Pawn, Knight, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Bishop, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Rook, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Queen, true, false, false, false);
			}
			// enPassant
			else if (to == ePSq)
			{
				*moveList++ = createMove(from, to, Pawn, None, true, false, true, false);
			}
			else
			{
				*moveList++ = createMove(from, to, Pawn, None, true, false, false, false);
			}
		}

		Bitboard rightAttack = pawnRightAttack<sideToMove, ePSq>(pawns, enemy);
		while (rightAttack)
		{

			int to = lsb(rightAttack);
			clearLSB(rightAttack);
			int from = to - (sideToMove ? 9 : -7);

			// promotion check
			if (getRankFromInt(to) == 0 || getRankFromInt(to) == 7)
			{

				*moveList++ = createMove(from, to, Pawn, Knight, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Bishop, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Rook, true, false, false, false);
				*moveList++ = createMove(from, to, Pawn, Queen, true, false, false, false);
			}
			// enPassant
			else if (to == ePSq)
			{

				*moveList++ = createMove(from, to, Pawn, None, true, false, true, false);
			}
			else
			{
				*moveList++ = createMove(from, to, Pawn, None, true, false, false, false);
			}
		}

		return moveList;
	}
	// knight

	// single knight attack
	inline Bitboard getKnightAttacks(Bitboard self, int sq)
	{
		return KNIGHT_ATTACKS[sq] & ~self;
	}

	// generate pseudo knight moves
	template <bool sideToMove>
	int* generateKnightMove(const Board& board, int* moveList)
	{
		Bitboard knights = sideToMove ? board.pieces[WhiteKnight] : board.pieces[BlackKnight];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];

		while (knights)
		{
			int from = lsb(knights);
			clearLSB(knights);

			Bitboard knightAttack = getKnightAttacks(self, from);
			while (knightAttack)
			{

				int to = lsb(knightAttack);
				clearLSB(knightAttack);

				if (getBit(enemy, to))
				{
					*moveList++ = createMove(from, to, Knight, None, true, false, false, false);
				}
				else
				{
					*moveList++ = createMove(from, to, Knight, None, false, false, false, false);
				}
			}
		}

		return moveList;
	}

	// king

	// generate pseudo king moves
	template <bool sideToMove>
	int* generateKingMove(const Board& board, int* moveList)
	{
		Bitboard king = sideToMove ? board.pieces[WhiteKing] : board.pieces[BlackKing];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];

		int from = lsb(king);

		Bitboard kingAttack = KING_ATTACKS[from] & ~self;
		while (kingAttack)
		{
			int to = lsb(kingAttack);
			clearLSB(kingAttack);

			if (getBit(enemy, to))
			{
				*moveList++ = createMove(from, to, King, None, true, false, false, false);
			}
			else
			{
				*moveList++ = createMove(from, to, King, None, false, false, false, false);
			}
		}
		return moveList;
	}

	// generate king castle moves
	template <bool whiteKingSide, bool whiteQueenSide>
	int* generateWhiteCastles(int* moveList)
	{
		if (whiteKingSide)
			*moveList++ = createMove(E1, G1, King, None, false, false, false, true);
		if (whiteQueenSide)
			*moveList++ = createMove(E1, C1, King, None, false, false, false, true);

		return moveList;
	}

	template <bool blackKingSide, bool blackQueenSide>
	int* generateBlackCastles(int* moveList)
	{
		if (blackKingSide)
			*moveList++ = createMove(E8, G8, King, None, false, false, false, true);
		if (blackQueenSide)
			*moveList++ = createMove(E8, C8, King, None, false, false, false, true);
		return moveList;
	}

	// Bishop

	// retrieve bishop moves using magic 
	inline Bitboard getBishopAttacks(Bitboard occ, int sq)
	{
		occ &= BISHOP_MASKS[sq];
		uint64_t index = ((occ * BISHOP_MAGICS[sq]) >> 55) + BISHOP_OFFSETS[sq];
		return BISHOP_ATTACKS[index];
	}

	// generate pseudo bishop moves
	template <bool sideToMove>
	int* generateBishopMove(const Board& board, int* moveList)
	{
		Bitboard bishops = sideToMove ? board.pieces[WhiteBishop] : board.pieces[BlackBishop];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];

		while (bishops)
		{
			int from = lsb(bishops);
			clearLSB(bishops);

			Bitboard bishopAttack = getBishopAttacks(board.occupancy[Both], from) & ~self;
			while (bishopAttack)
			{

				int to = lsb(bishopAttack);
				clearLSB(bishopAttack);

				if (getBit(enemy, to))
				{
					*moveList++ = createMove(from, to, Bishop, None, true, false, false, false);
				}
				else
				{
					*moveList++ = createMove(from, to, Bishop, None, false, false, false, false);
				}
			}
		}

		return moveList;
	}

	// Rook

	// retrieve rook moves using magic 
	inline Bitboard getRookAttacks(Bitboard occ, int sq)
	{
		occ &= ROOK_MASKS[sq];
		uint64_t index = ((occ * ROOK_MAGICS[sq]) >> 52) + ROOK_OFFSETS[sq];
		return ROOK_ATTACKS[index];
	}

	// generate pseudo bishop moves
	template <bool sideToMove>
	int* generateRookMove(const Board& board, int* moveList)
	{
		Bitboard rooks = sideToMove ? board.pieces[WhiteRook] : board.pieces[BlackRook];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];

		while (rooks)
		{
			int from = lsb(rooks);
			clearLSB(rooks);

			Bitboard rookAttack = getRookAttacks(board.occupancy[Both], from) & ~self;
			while (rookAttack)
			{

				int to = lsb(rookAttack);
				clearLSB(rookAttack);

				if (getBit(enemy, to))
				{
					*moveList++ = createMove(from, to, Rook, None, true, false, false, false);
				}
				else
				{
					*moveList++ = createMove(from, to, Rook, None, false, false, false, false);
				}
			}
		}

		return moveList;
	}

	// retrieve queen moves from rook and bishop 
	inline Bitboard getQueenAttacks(Bitboard occ, int sq)
	{
		return getRookAttacks(occ, sq) | getBishopAttacks(occ, sq);
	}

	// generate pseudo bishop moves
	template <bool sideToMove>
	int* generateQueenkMove(const Board& board, int* moveList)
	{
		Bitboard queens = sideToMove ? board.pieces[WhiteQueen] : board.pieces[BlackQueen];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemy = sideToMove ? board.occupancy[Black] : board.occupancy[White];

		while (queens)
		{
			int from = lsb(queens);
			clearLSB(queens);

			Bitboard queenAttack = getQueenAttacks(board.occupancy[Both], from) & ~self;
			while (queenAttack)
			{

				int to = lsb(queenAttack);
				clearLSB(queenAttack);

				if (getBit(enemy, to))
				{
					*moveList++ = createMove(from, to, Queen, None, true, false, false, false);
				}
				else
				{
					*moveList++ = createMove(from, to, Queen, None, false, false, false, false);
				}
			}
		}

		return moveList;
	}

	// all pseudo moves
	template <bool sideToMove, int ePSq>
	int* generatePseudoMove(const Board& board, int* moveList)
	{

		moveList = generatePawnMoves<sideToMove, ePSq>(board, moveList);
		moveList = generateKnightMove<sideToMove>(board, moveList);
		moveList = generateKingMove<sideToMove>(board, moveList);
		moveList = generateBishopMove<sideToMove>(board, moveList);
		moveList = generateRookMove<sideToMove>(board, moveList);
		moveList = generateQueenkMove<sideToMove>(board, moveList);

		return moveList;
	}

	template <bool sideToMove>
	inline Bitboard squareAttackers(const Board& board, int sq)
	{
		Bitboard attackers = EMPTY_BOARD;

		Bitboard enemyP = sideToMove ? board.pieces[BlackPawn]   : board.pieces[WhitePawn];
		Bitboard enemyN = sideToMove ? board.pieces[BlackKnight] : board.pieces[WhiteKnight];
		Bitboard enemyB = sideToMove ? board.pieces[BlackBishop] : board.pieces[WhiteBishop];
		Bitboard enemyR = sideToMove ? board.pieces[BlackRook]   : board.pieces[WhiteRook];
		Bitboard enemyQ = sideToMove ? board.pieces[BlackQueen]  : board.pieces[WhiteQueen];
		Bitboard enemyK = sideToMove ? board.pieces[BlackKing]   : board.pieces[WhiteKing];
		
		// calculate all attacks from the sq and see if the corresponding enemy piece is in that attack
		attackers |= pawnLeftAttacksFromSquare<sideToMove>(sq, enemyP);
		attackers |= pawnRightAttacksFromSquare<sideToMove>(sq, enemyP);
		attackers |= KNIGHT_ATTACKS[sq] & enemyN;
		attackers |= getBishopAttacks(board.occupancy[Both], sq) & enemyB;
		attackers |= getRookAttacks(board.occupancy[Both], sq) & enemyR;
		attackers |= getQueenAttacks(board.occupancy[Both], sq) & enemyQ;
		attackers |= KING_ATTACKS[sq] & enemyK;


		return attackers;
	}

	//calculate rays between two squares
	inline Bitboard getRaysBetweenSquare(int sq1, int sq2)
	{
		return RAYS[sq1][sq2];
	}

	template <bool sideToMove>
	inline CheckInfo getCheckInfo(const Board& board)
	{
		CheckInfo ci;
		Bitboard occ = board.occupancy[Both];

		int kSq = sideToMove ? lsb(board.pieces[WhiteKing]) : lsb(board.pieces[BlackKing]);
		Bitboard bishop = sideToMove ?  board.pieces[BlackBishop]: board.pieces[WhiteBishop];
		Bitboard rook = sideToMove ? board.pieces[BlackRook] : board.pieces[WhiteRook];
		Bitboard queen  = sideToMove ?  board.pieces[BlackQueen]: board.pieces[WhiteQueen];
		Bitboard self = sideToMove ? board.occupancy[White] : board.occupancy[Black];
		Bitboard enemySliders = bishop | rook | queen;

		// checker are all pieces attacking the king currently
		ci.checkers = squareAttackers<sideToMove>(board, kSq);

		Bitboard rookAttacks = getRookAttacks(EMPTY_BOARD, kSq);
		Bitboard bishopAttacks = getBishopAttacks(EMPTY_BOARD, kSq);

		// pinned pieces by rook and queen
		while (rookAttacks)
		{
			int sq = lsb(rookAttacks);
			clearLSB(rookAttacks);

			Bitboard ray = getRaysBetweenSquare(sq, kSq);
			Bitboard blockers = ray & occ;

			
			// if 2 blockers, it may be the enemy piece and the possibly pinned piece
			if (popCount(blockers) == 1)
			{
				// check if possibly pinned piece is self
				if (blockers & self)
				{
					Bitboard selfBlockers = blockers & self;
					int maybePinned = lsb(selfBlockers); 

					// if ray piece is an enemy sliding piece, the piece is pinned
					if ((1ULL << sq) & (rook | queen))
						ci.pinned |= 1ULL << maybePinned;
				}
			}
		}

		// pinned pieces by bishop and queen
		while (bishopAttacks)
		{
			int sq = lsb(bishopAttacks);
			clearLSB(bishopAttacks);

			Bitboard ray = getRaysBetweenSquare(sq, kSq);
			Bitboard blockers = ray & occ;

			// if 2 blockers, it may be the enemy piece and the possibly pinned piece
			if (popCount(blockers) == 1)
			{
				// check if possibly pinned piece is self
				if (blockers & self)
				{
					Bitboard selfBlockers = blockers & self;
					int maybePinned = lsb(selfBlockers);

					// if ray piece is an enemy sliding piece, the piece is pinned
					if ((1ULL << sq) & (bishop | queen))
						ci.pinned |= 1ULL << maybePinned;
				}
			}
		}

		if (!ci.checkers)
		{
			ci.checkMask = FULL_BOARD;
		}
		// check mask 
		// single check
		else if (popCount(ci.checkers) == 1)
		{
			int checkerSq = lsb(ci.checkers);
			Bitboard mask = (1ULL << checkerSq);

			// to allow blocking moves for slider checks
			if (mask & enemySliders)
				mask |= getRaysBetweenSquare(kSq, checkerSq);

			ci.checkMask = mask;
		}
		// multiple check only king move
		else
		{
			ci.checkMask = EMPTY_BOARD;
		}
		// no check
		
		return ci;
	}

	template <bool sideToMove>
	bool isLegalMove(int move, const Board& board, const CheckInfo& ci, int ePSq)
	{
		int kSq = sideToMove ? lsb(board.pieces[WhiteKing]) : lsb(board.pieces[BlackKing]);

		// King moves: target square must NOT be attacked
		if (getPiece(move) == King)
		{
			// We need to check a board without king to prevent x-ray moves from sneaking in
			Board b = board;
			removeSquareFromBoard(b, kSq);
			updateOccupancy(b);

			if (squareAttackers<sideToMove>(b, getTarget(move)))
			{
				return false;
			}
				
			if (getIsCastling(move))
			{
				// if check no castling
				if(squareAttackers<sideToMove>(board, getCurrent(move)))
					return false;

				switch (getTarget(move))
				{
				case G1: 
					return !(squareAttackers<sideToMove>(board, F1) || getBit(board.occupancy[Both], F1) 
						|| getBit(board.occupancy[Both], G1));
				case C1: return !(squareAttackers<sideToMove>(board, D1) || getBit(board.occupancy[Both], D1)
					|| getBit(board.occupancy[Both], C1) || getBit(board.occupancy[Both], B1));
				case G8: return !(squareAttackers<sideToMove>(board, F8) || getBit(board.occupancy[Both], F8)
					|| getBit(board.occupancy[Both], G8));
				case C8: return !(squareAttackers<sideToMove>(board, D8) || getBit(board.occupancy[Both], D8)
					|| getBit(board.occupancy[Both], C8) || getBit(board.occupancy[Both], B8));
				}
			}
			return true;
		}

		// Non-king moves: if double-check, only king moves are allowed
		if (popCount(ci.checkers) >= 2) return false;

		// If piece is pinned, it must move along the ray between king and target
		if ((1ULL << getCurrent(move)) & ci.pinned)
		{
			if (!((1ULL << getCurrent(move)) & getRaysBetweenSquare(kSq, getTarget(move)) ||
				(1ULL << getTarget(move)) & getRaysBetweenSquare(kSq, getCurrent(move))))
				return false;
		}

		// If single check, move must block or capture the checker
		if (!(ci.checkMask & (1ULL << getTarget(move)))) return false;

		// En-passant: simulate the capture and ensure king is not left in check
		if (getIsEnPassant(move))
		{
			Board b = board;

			if (sideToMove)
			{
				popBit(b.pieces[WhitePawn], getCurrent(move));
				setBit(b.pieces[WhitePawn], ePSq);
				popBit(b.pieces[BlackPawn], ePSq - 8);
			}
			else
			{
				popBit(b.pieces[BlackPawn], getCurrent(move));
				setBit(b.pieces[BlackPawn], ePSq);
				popBit(b.pieces[WhitePawn], ePSq + 8);
			}

			updateOccupancy(b);

			// if king is attacked after en-passant, the move is illegal
			if (squareAttackers<sideToMove>(b, kSq)) return false;
		}
		return true;
	}

	int* generateLegalMoves(const Board& board, int* moveList, const State& state)
	{
		int* start = moveList;
		CheckInfo ci;

		if (state.sideToMove == White)
		{
			ci = getCheckInfo<White>(board);

			switch (state.enPassantSquare)
			{
			case A6: moveList = generatePseudoMove<White, A6>(board, moveList);  break;
			case B6: moveList = generatePseudoMove<White, B6>(board, moveList);  break;
			case C6: moveList = generatePseudoMove<White, C6>(board, moveList);  break;
			case D6: moveList = generatePseudoMove<White, D6>(board, moveList);  break;
			case E6: moveList = generatePseudoMove<White, E6>(board, moveList);  break;
			case F6: moveList = generatePseudoMove<White, F6>(board, moveList);  break;
			case G6: moveList = generatePseudoMove<White, G6>(board, moveList);  break;
			case H6: moveList = generatePseudoMove<White, H6>(board, moveList);  break;
			default: moveList = generatePseudoMove<White, -1>(board, moveList);  break;
			}
			if (state.castlingRights[0])
			{
				moveList = generateWhiteCastles<true, false>(moveList);
			}
			if (state.castlingRights[1])
			{
				moveList = generateWhiteCastles<false, true>(moveList);
			}

			
			for (int* m = start; m != moveList; ++m)
			{
				int move = *m;

				if (isLegalMove<White>(move, board, ci, state.enPassantSquare))
				{
					*start++ = move;
				}
			}
		}
		else
		{
			ci = getCheckInfo<Black>(board);
			switch (state.enPassantSquare)
			{
			case A3: moveList = generatePseudoMove<Black, A3>(board, moveList);  break;
			case B3: moveList = generatePseudoMove<Black, B3>(board, moveList);  break;
			case C3: moveList = generatePseudoMove<Black, C3>(board, moveList);  break;
			case D3: moveList = generatePseudoMove<Black, D3>(board, moveList);  break;
			case E3: moveList = generatePseudoMove<Black, E3>(board, moveList);  break;
			case F3: moveList = generatePseudoMove<Black, F3>(board, moveList);  break;
			case G3: moveList = generatePseudoMove<Black, G3>(board, moveList);  break;
			case H3: moveList = generatePseudoMove<Black, H3>(board, moveList);  break;
			default: moveList = generatePseudoMove<Black, -1>(board, moveList);  break;
			}

			if (state.castlingRights[2])
			{
				moveList = generateBlackCastles<true, false>(moveList);
			}
			if (state.castlingRights[3])
			{
				moveList = generateBlackCastles<false, true>(moveList);
			}


			for (int* m = start; m != moveList; ++m)
			{
				int move = *m;

				if (isLegalMove<Black>(move, board, ci, state.enPassantSquare))
				{
					*start++ = move;
				}
			}
		}
		
		*start++ = 0;
		return start;
	}
}