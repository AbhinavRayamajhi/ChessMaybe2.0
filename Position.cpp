#include "Position.h"

void makeMove(int move, Board& board, State& state, std::vector<Position>& history)
{
	Position p(board, state);
	history.push_back(p);

	int current = getCurrent(move);
	int target = getTarget(move);
	int piece = getPiece(move);
	int promotionPiece = getPromotion(move);
	bool isCapture = getIsCapture(move);
	bool isDoublePush = getIsDoublePush(move);
	bool isEnPassant = getIsEnPassant(move);
	bool isCastling = getIsCastling(move);


	Bitboard* pawns = state.sideToMove ? &board.pieces[WhitePawn] : &board.pieces[BlackPawn];
	Bitboard* knights = state.sideToMove ? &board.pieces[WhiteKnight] : &board.pieces[BlackKnight];
	Bitboard* bishops = state.sideToMove ? &board.pieces[WhiteBishop] : &board.pieces[BlackBishop];
	Bitboard* rooks = state.sideToMove ? &board.pieces[WhiteRook] : &board.pieces[BlackRook];
	Bitboard* queens = state.sideToMove ? &board.pieces[WhiteQueen] : &board.pieces[BlackQueen];
	Bitboard* king = state.sideToMove ? &board.pieces[WhiteKing] : &board.pieces[BlackKing];

	Bitboard* self = state.sideToMove ? &board.occupancy[White] : &board.occupancy[Black];
	Bitboard* enemy = state.sideToMove ? &board.occupancy[Black] : &board.occupancy[White];
	Bitboard* occ = &board.occupancy[Both];

	// set castling right to false if not moved rook is target
	switch (target)
	{
	case H1:
		state.castlingRights[WhiteKingSide] = false; break;
	case A1:
		state.castlingRights[WhiteQueenSide] = false; break;
	case H8:
		state.castlingRights[BlackKingSide] = false; break;
	case A8:
		state.castlingRights[BlackQueenSide] = false; break;
	}

	// update board using move
	removeSquareFromBoard(board, current);
	removeSquareFromBoard(board, target);

	if (piece == Pawn)
	{
		// if enPassant capture remove the captured piece
		if (isEnPassant)
		{
			removeSquareFromBoard(board, state.sideToMove ? target - 8 : target + 8);
		}

		// if promotion piece set the relevant bit to promoted piece
		switch (promotionPiece)
		{
		case None: setBit(*pawns, target); break;
		case Knight: setBit(*knights, target); break;
		case Bishop: setBit(*bishops, target); break;
		case Rook: setBit(*rooks, target); break;
		case Queen: setBit(*queens, target); break;
		}
	}

	// reset enPassant sq
	state.enPassantSquare = -1;

	// set enPassant sq if double push
	if (isDoublePush)
	{
		state.enPassantSquare = state.sideToMove ? target - 8 : target + 8;
	}
	
	if (piece == King)
	{
		if (isCastling)
		{
			switch (target)
			{
			case G1:
				removeSquareFromBoard(board, H1);
				setBit(*rooks, F1);
				noWhiteCastlingRights(state);
				break;
			case C1:
				removeSquareFromBoard(board, A1);
				setBit(*rooks, D1);
				noWhiteCastlingRights(state);
				break;
			case G8:
				removeSquareFromBoard(board, H8);
				setBit(*rooks, F8);
				noBlackCastlingRights(state);
				break;
			case C8:
				removeSquareFromBoard(board, A8);
				setBit(*rooks, D8);
				noBlackCastlingRights(state);
				break;
			}
		}
		else
		{
			if (state.sideToMove == White) noWhiteCastlingRights(state);
			else noBlackCastlingRights(state);
		}
		setBit(*king, target);
	}
	else if (piece == Rook)
	{
		setBit(*rooks, target);

		// if rook moved from orig square, cancel castle rights
		switch (current)
		{
		case H1:
			state.castlingRights[0] = false; break;
		case A1:
			state.castlingRights[1] = false; break;
		case H8:
			state.castlingRights[2] = false; break;
		case A8:
			state.castlingRights[3] = false; break;
		}

	}
	else if (piece == Knight) { setBit(*knights, target); }
	else if (piece == Bishop) { setBit(*bishops, target); }
	else if (piece == Queen) { setBit(*queens, target); }

	updateOccupancy(board);

	state.sideToMove ^= 1;
}

void unmakeMove(Board& board, State& state, std::vector<Position>& history)
{
	Position prev = history.back();

	board = prev.board;
	state = prev.state;

	history.pop_back();
}