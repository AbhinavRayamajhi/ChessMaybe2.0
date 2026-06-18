#include "Board.h"

Board::Board() :
	pieces{}, 
	occupancy{},
	sideToMove{ WHITE },
	castlingRights{ NO_CASTLING },
	enPassantSq{ SQ_NONE },
	halfMoveClock{0},
	fullMoveClock{ 0 } {
}

Board::Board(std::string FEN) : Board() {

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
		case 'p': setBit(pieces[BLACK][PAWN], sq); file++; break;
		case 'n': setBit(pieces[BLACK][KNIGHT], sq); file++; break;
		case 'b': setBit(pieces[BLACK][BISHOP], sq); file++; break;
		case 'r': setBit(pieces[BLACK][ROOK], sq); file++; break;
		case 'q': setBit(pieces[BLACK][QUEEN], sq); file++; break;
		case 'k': setBit(pieces[BLACK][KING], sq); file++; break;
		case 'P': setBit(pieces[WHITE][PAWN], sq); file++; break;
		case 'N': setBit(pieces[WHITE][KNIGHT], sq); file++; break;
		case 'B': setBit(pieces[WHITE][BISHOP], sq); file++; break;
		case 'R': setBit(pieces[WHITE][ROOK], sq); file++; break;
		case 'Q': setBit(pieces[WHITE][QUEEN], sq); file++; break;
		case 'K': setBit(pieces[WHITE][KING], sq); file++; break;

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
		sideToMove = WHITE;
	else
		sideToMove = BLACK;

	// get Castling rights
	for (char c : tokens[2]) {
		
		if (c == '-') {
			break;
		}
		else if (c == 'K') {
		    castlingRights |= WHITE_OO;
		}
		else if (c == 'k') {
			castlingRights |= BLACK_OO;
		}
		else if (c == 'Q') {
			castlingRights |= WHITE_OOO;
		}
		else if (c == 'q') {
			castlingRights |= BLACK_OOO;
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
		enPassantSq = getSquareFromRF(rank, file);
	}
	else
		enPassantSq = SQ_NONE;

	// get clocks
	halfMoveClock = tokens[4][0] - '0';
	fullMoveClock = tokens[5][0] - '0';

	updateAllOccupancy();
}

void Board::printBoard() {

    for (int rank = 7; rank >= 0; --rank) {

		std::cout << rank + 1 << "  ";
		for (int file = 0; file < 8; ++file) {

			Square sq = getSquareFromRF(rank, file);
			if (getBit(occupancy[BOTH], sq)) {

				if (getBit(pieces[WHITE][PAWN], sq))
					std::cout << "P ";
				else if (getBit(pieces[WHITE][KNIGHT], sq))
					std::cout << "N ";
				else if (getBit(pieces[WHITE][BISHOP], sq))
					std::cout << "B ";
				else if (getBit(pieces[WHITE][ROOK], sq))
					std::cout << "R ";
				else if (getBit(pieces[WHITE][QUEEN], sq))
					std::cout << "Q ";
				else if (getBit(pieces[WHITE][KING], sq))
					std::cout << "K ";
				else if (getBit(pieces[BLACK][PAWN], sq))
					std::cout << "p ";
				else if (getBit(pieces[BLACK][KNIGHT], sq))
					std::cout << "n ";
				else if (getBit(pieces[BLACK][BISHOP], sq))
					std::cout << "b ";
				else if (getBit(pieces[BLACK][ROOK], sq))
					std::cout << "r ";
				else if (getBit(pieces[BLACK][QUEEN], sq))
					std::cout << "q ";
				else if (getBit(pieces[BLACK][KING], sq))
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