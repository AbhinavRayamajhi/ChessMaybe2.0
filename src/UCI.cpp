#include "UCI.h"

#include <sstream>

namespace Engine {

    void uciLoop() {

        Board board;
        std::string input, token;

        while (std::getline(std::cin, input)) {

            std::istringstream iss(input);
            iss >> token;

            if (token == "uci") {

                std::cout << "id name ChessMaybe2.0\n";
                std::cout << "id author Abhinav\n";
                std::cout << "uciok" << std::endl;
            }
            else if (token == "isready") {

                std::cout << "readyok" << std::endl;
            }
            else if (token == "ucinewgame") {

                board = Board(startpos);
            }
            else if (token == "position") {

                iss >> token;

                if (token == "startpos") {

                    board = Board(startpos);
                    iss >> token; // remove moves from stream
                }
                else if (token == "fen") {

                    std::string fen;
                    while (iss >> token && token != "moves") {
                        fen += token + " ";
                    }

                    board = Board(fen);
                }
 
                while (iss >> token) {

                    History h;
                    Move move = convertMoveFromString(board, token);
                    makeMove(move, board, h);
                }
            }
            else if (token == "go") {
                
                int depth = 7;

                while (iss >> token) {

                    if (token == "depth") iss >> depth;
                }

                uint64_t nodes = 0;
                Move bestMove = getBestMove(board, depth, nodes, false);
                std::cout << "bestmove " << convertMoveToString(bestMove) << std::endl;
            }
            else if (token == "play") {
                iss >> token;
                History h;
                Move move = convertMoveFromString(board, token);
                makeMove(move, board, h);
            }
            else if (token == "d") {
                board.printBoard();
            }
            else if (token == "quit") {

                break;
            }
        }
    }
}