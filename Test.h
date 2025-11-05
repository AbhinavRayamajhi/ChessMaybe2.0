#pragma once

#include <iostream>
#include "Move.h"
#include "MoveGen.h"
#include "Position.h"

namespace Test
{
	inline uint64_t perfTest(Position& p, std::vector<Position> history, int depth, int rootDepth)
	{
		if (depth == 0) return 1ULL;

		uint64_t nodes = 0;
		int moveList[256]{ 0 };

		MoveGen::generateLegalMoves(p.board, moveList, p.state);

		for (int i = 0; moveList[i] != 0; ++i)
		{
			makeMove(moveList[i], p.board, p.state, history);
			uint64_t node = perfTest(p, history, depth - 1, rootDepth);
			unmakeMove(p.board, p.state, history);

			if (rootDepth == depth)
			{
				printMove(moveList[i]);
				std::cout << " : " << node << '\n';
			}

			nodes += node;
		}

		return nodes;
	}
}