#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "Definitions.h"
#include "Helpers.h"

struct Board {
	// bitboards for each piece type by color
	std::array<std::array<Bitboard, PIECE_COUNT>, 2> pieces{ 0 };

	// bitboards for occupancy of each type and both colors
	std::array<Bitboard, 3> occupancy{ 0 };

	Color sideToMove = WHITE;
	CastlingRight castlingRights = NO_CASTLING;
	Square enPassantSq = SQ_NONE;

	int halfMoveClock = 0;
	int fullMoveClock = 0;
};

// update the occupancy bitboards after the pieces are changed
inline void updateOccupancy(Board& board) {

	board.occupancy[WHITE] = 0ULL;
	board.occupancy[BLACK] = 0ULL;

	for (Piece p = PAWN; p != PIECE_NONE; ++p) {
		board.occupancy[WHITE] |= board.pieces[WHITE][p];
		board.occupancy[BLACK] |= board.pieces[BLACK][p];
	}

	board.occupancy[BOTH] = board.occupancy[WHITE] | board.occupancy[BLACK];
}

inline Board createBoard(std::string FEN) {

	Board board = Board();

	std::istringstream iss(FEN);
	std::vector<std::string> tokens;
	std::string token;

	while (std::getline(iss, token, ' ')) {

		if (!token.empty()) tokens.push_back(token);
	}

	// get all pieces
	int file = 0, rank = 7;
	for (char c : tokens[0]) {

		Square sq = getSquareFromRF(rank, file);

		switch (c)
		{
		case 'p': setBit(board.pieces[BLACK][PAWN], sq); file++; break;
		case 'n': setBit(board.pieces[BLACK][KNIGHT], sq); file++; break;
		case 'b': setBit(board.pieces[BLACK][BISHOP], sq); file++; break;
		case 'r': setBit(board.pieces[BLACK][ROOK], sq); file++; break;
		case 'q': setBit(board.pieces[BLACK][QUEEN], sq); file++; break;
		case 'k': setBit(board.pieces[BLACK][KING], sq); file++; break;
		case 'P': setBit(board.pieces[WHITE][PAWN], sq); file++; break;
		case 'N': setBit(board.pieces[WHITE][KNIGHT], sq); file++; break;
		case 'B': setBit(board.pieces[WHITE][BISHOP], sq); file++; break;
		case 'R': setBit(board.pieces[WHITE][ROOK], sq); file++; break;
		case 'Q': setBit(board.pieces[WHITE][QUEEN], sq); file++; break;
		case 'K': setBit(board.pieces[WHITE][KING], sq); file++; break;

		case '8': break;
		case '7': file += 7; break;
		case '6': file += 6; break;
		case '5': file += 5; break;
		case '4': file += 4; break;
		case '3': file += 3; break;
		case '2': file += 2; break;
		case '1': file++; break;
		
		case '/': file = 0; rank--; break;
		}
	}

	// get side to move
	if (tokens[1] == "w")
		board.sideToMove = WHITE;
	else
		board.sideToMove = BLACK;

	// get Castling rights
	for (char c : tokens[2]) {
		
		if (c == '-') {
			break;
		}
		else if (c == 'K') {
			board.castlingRights |= WHITE_OO;
		}
		else if (c == 'k') {
			board.castlingRights |= BLACK_OO;
		}
		else if (c == 'Q') {
			board.castlingRights |= WHITE_OOO;
		}
		else if (c == 'q') {
			board.castlingRights |= BLACK_OOO;
		}
	}
	
	// get enPassant
	if (tokens[3] != "-") {

		for (char c : tokens[3]) {

			if (std::isalpha(c))
				file = getFileFromChar(c);
			else
				rank = getRankFromChar(c);
		}
		board.enPassantSq = getSquareFromRF(rank, file);
	}
	else
		board.enPassantSq = SQ_NONE;

	// get clocks
	board.halfMoveClock = tokens[4][0] - '0';
	board.fullMoveClock = tokens[5][0] - '0';

	updateOccupancy(board);
	return board;
}

inline void printBoard(const Board& board) {

	for (int rank = 7; rank >= 0; --rank) {

		std::cout << rank + 1 << "  ";
		for (int file = 0; file < 8; ++file) {

			Square sq = getSquareFromRF(rank, file);
			if (getBit(board.occupancy[BOTH], sq)) {

				if (getBit(board.pieces[WHITE][PAWN], sq))
					std::cout << "P ";
				else if (getBit(board.pieces[WHITE][KNIGHT], sq))
					std::cout << "N ";
				else if (getBit(board.pieces[WHITE][BISHOP], sq))
					std::cout << "B ";
				else if (getBit(board.pieces[WHITE][ROOK], sq))
					std::cout << "R ";
				else if (getBit(board.pieces[WHITE][QUEEN], sq))
					std::cout << "Q ";
				else if (getBit(board.pieces[WHITE][KING], sq))
					std::cout << "K ";
				else if (getBit(board.pieces[BLACK][PAWN], sq))
					std::cout << "p ";
				else if (getBit(board.pieces[BLACK][KNIGHT], sq))
					std::cout << "n ";
				else if (getBit(board.pieces[BLACK][BISHOP], sq))
					std::cout << "b ";
				else if (getBit(board.pieces[BLACK][ROOK], sq))
					std::cout << "r ";
				else if (getBit(board.pieces[BLACK][QUEEN], sq))
					std::cout << "q ";
				else if (getBit(board.pieces[BLACK][KING], sq))
					std::cout << "k ";
			}
			else {
				
				std::cout << ". ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n   a b c d e f g h\n\n";
}

inline void removeSquareFromBoard(Board& board, Square sq) {

	for (Piece p = PAWN; p != PIECE_NONE; ++p) {
		
		resetBit(board.pieces[WHITE][p], sq);
		resetBit(board.pieces[BLACK][p], sq);
	}
}