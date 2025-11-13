#pragma once

#include "Search.h"
#include "Move.h"
#include "MoveGen.h"
#include "Position.h"

class Player
{
private:
	Position p;
	std::vector<Position> history;
	int moveList[256]{ 0 };
	int bestMove;
	bool side;

public:
	Player(std::string FEN, bool side) :
		p(Position(FEN)), side(side)
	{
	};

	inline void play(int depth)
	{
		int oppMove = 0;
		printBoard(p.board);

		if (this->p.state.sideToMove == this->side)
		{
			this->bestMove = Engine::getBestMove(p, depth);
			makeMove(this->bestMove, p.board, p.state, this->history);
			printMove(this->bestMove);
			printBoard(p.board);
		}

		while (true)
		{
			// clear movelist
			for (int i = 0; i < 256; ++i) moveList[i] = 0;

			MoveGen::generateLegalMoves(p.board, moveList, p.state);

			// if no move, checkmate or draw todo::draw
			if (moveList[0] == 0)
			{
				std::cout << "Checkmate\n";
				break;
			}

			// user move ask for
			std::cout << "Enter Move: ";
			std::string userInput;
			std::cin >> userInput;
			std::cout << "\n";

			// -1 to exit
			std::string current = userInput.substr(0, 2);
			if (current == "-1") break;

			// get squares from user input
			int currentSq = stringToSquare(current);
			int targetSq = stringToSquare(userInput.substr(2, 2));

			bool validMove = false;
			// check all legal moves and see if user move matches
			for (int i = 0; moveList[i] != 0; ++i)
			{
				if (getCurrent(moveList[i]) == currentSq && getTarget(moveList[i]) == targetSq)
				{
					validMove = true;
					makeMove(moveList[i], p.board, p.state, this->history);
					printMove(moveList[i]);
					printBoard(p.board);
					break;
				}
			}

			if (!validMove)
			{
				std::cout << "Invalid Move. Try Again\n";
			}
			else
			{
				// clear movelist
				for (int i = 0; i < 256; ++i) moveList[i] = 0;

				this->bestMove = Engine::getBestMove(p, depth);

				if (this->bestMove == 0)
				{
					std::cout << "Checkmate\n";
					break;
				}

				makeMove(this->bestMove, p.board, p.state, this->history);
				printMove(this->bestMove);
				printBoard(p.board);
			}
		}
	}
	
};