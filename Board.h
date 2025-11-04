#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "Definitions.h"
#include "Helpers.h"

struct Board
{
	// bitboards for each piece type
	std::array<Bitboard, 12> pieces{ 0 };

	// bitboards for occupancy of each type and both colors
	std::array<Bitboard, 3> occupancy{ 0 };
};

// update the occupancy bitboards after the pieces are changed
inline void updateOccupancy(Board& board)
{
	board.occupancy[White] = board.pieces[WhitePawn] | board.pieces[WhiteKnight] |
		board.pieces[WhiteBishop] | board.pieces[WhiteRook] |
		board.pieces[WhiteQueen] | board.pieces[WhiteKing];

	board.occupancy[Black] = board.pieces[BlackPawn] | board.pieces[BlackKnight] |
		board.pieces[BlackBishop] | board.pieces[BlackRook] |
		board.pieces[BlackQueen] | board.pieces[BlackKing];

	board.occupancy[Both] = board.occupancy[White] | board.occupancy[Black];
}

inline Board createBoard(std::string FEN)
{
	Board board = Board();

	// get all pieces
	int file{ 0 }, rank{ 7 };
	for (char c : FEN)
	{
		int sq = getSquareFromRF(rank, file);

		switch (c)
		{
		case 'p': setBit(board.pieces[BlackPawn], sq); file++; break;
		case 'n': setBit(board.pieces[BlackKnight], sq); file++; break;
		case 'b': setBit(board.pieces[BlackBishop], sq); file++; break;
		case 'r': setBit(board.pieces[BlackRook], sq); file++; break;
		case 'q': setBit(board.pieces[BlackQueen], sq); file++; break;
		case 'k': setBit(board.pieces[BlackKing], sq); file++; break;
		case 'P': setBit(board.pieces[WhitePawn], sq); file++; break;
		case 'N': setBit(board.pieces[WhiteKnight], sq); file++; break;
		case 'B': setBit(board.pieces[WhiteBishop], sq); file++; break;
		case 'R': setBit(board.pieces[WhiteRook], sq); file++; break;
		case 'Q': setBit(board.pieces[WhiteQueen], sq); file++; break;
		case 'K': setBit(board.pieces[WhiteKing], sq); file++; break;

		case '7': file++; [[fallthrough]];
		case '6': file++; [[fallthrough]];
		case '5': file++; [[fallthrough]];
		case '4': file++; [[fallthrough]];
		case '3': file++; [[fallthrough]];
		case '2': file++; [[fallthrough]];
		case '1': file++; break;

		case '/': file = 0; rank--; break;
		}

		if (c == ' ') break;
	}

	updateOccupancy(board);
	return board;
}

inline void printBoard(const Board& board)
{
	for (int rank = 7; rank >= 0; --rank)
	{
		std::cout << rank + 1 << "  ";

		for (int file = 0; file < 8; ++file)
		{
			int sq = getSquareFromRF(rank, file);

			if (getBit(board.occupancy[Both], sq))
			{
				if (getBit(board.pieces[WhitePawn], sq))
					std::cout << "P ";
				else if (getBit(board.pieces[WhiteKnight], sq))
					std::cout << "N ";
				else if (getBit(board.pieces[WhiteBishop], sq))
					std::cout << "B ";
				else if (getBit(board.pieces[WhiteRook], sq))
					std::cout << "R ";
				else if (getBit(board.pieces[WhiteQueen], sq))
					std::cout << "Q ";
				else if (getBit(board.pieces[WhiteKing], sq))
					std::cout << "K ";
				else if (getBit(board.pieces[BlackPawn], sq))
					std::cout << "p ";
				else if (getBit(board.pieces[BlackKnight], sq))
					std::cout << "n ";
				else if (getBit(board.pieces[BlackBishop], sq))
					std::cout << "b ";
				else if (getBit(board.pieces[BlackRook], sq))
					std::cout << "r ";
				else if (getBit(board.pieces[BlackQueen], sq))
					std::cout << "q ";
				else if (getBit(board.pieces[BlackKing], sq))
					std::cout << "k ";
			}
			else
			{
				std::cout << ". ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n   a b c d e f g h\n\n";
}

inline void removeSquareFromBoard(Board& board, int sq)
{
	for (int i = 0; i < 12; ++i)
	{
		popBit(board.pieces[i], sq);
	}
}



