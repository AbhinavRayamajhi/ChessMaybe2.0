#pragma once

#include "Board.h"
#include "Move.h"
#include "Position.h"
#include "Search.h"
#include <string>

const std::string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

namespace Engine {

    void uciLoop();
}