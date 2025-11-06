#pragma once

#include <array>
#include <string>
#include <sstream>
#include <vector>
#pragma once

#include "Definitions.h"
#include "Helpers.h"

struct State 
{
	bool sideToMove{ true };

	std::array<bool, 4> castlingRights{ 0 };

	int enPassantSquare{ 0 };

	int halfMoveClock{ 0 };
	int fullMoveClock{ 0 };

};

inline State stateFromFEN(std::string FEN) 
{
	State state;
	std::istringstream iss(FEN);
	std::vector<std::string> tokens;
	std::string token;

	while (std::getline(iss, token, ' '))
	{
		if (!token.empty()) tokens.push_back(token);
	}

	// get side to move
	if (tokens[1] == "w")
		state.sideToMove = White;
	else
		state.sideToMove = Black;

	// get Castling rights
	for (char c : tokens[2])
	{
		if (c == '-') { break; }
		else if (c == 'K') { state.castlingRights[WhiteKingSide] = true; }
		else if (c == 'k') { state.castlingRights[BlackKingSide] = true;  }
		else if (c == 'Q') { state.castlingRights[WhiteQueenSide] = true; }
		else if (c == 'q') { state.castlingRights[BlackQueenSide] = true; }
	}
	
	// get enPassant
	if (tokens[3] != "-")
	{
		int rank{ 0 }, file{ 0 };
		for (char c : tokens[3])
		{
			if (std::isalpha(c)) file = getFileFromChar(c);
			else rank = getRankFromChar(c);
		}
		state.enPassantSquare = rank * 8 + file;
	}
	else
		state.enPassantSquare = -1;

	// get clocks
	state.halfMoveClock = tokens[4][0] - '0';
	state.fullMoveClock = tokens[5][0] - '0';

	return state;
}

inline void noWhiteCastlingRights(State& state)
{
	state.castlingRights[WhiteKingSide] = false;
	state.castlingRights[WhiteQueenSide] = false;
}

inline void noBlackCastlingRights(State& state)
{
	state.castlingRights[BlackKingSide] = false;
	state.castlingRights[BlackQueenSide] = false;
}
