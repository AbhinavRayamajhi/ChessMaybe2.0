#include "UCI.h"

#include <fstream>
#include <sstream>

namespace Engine {

    void uciLoop() {

        Position position;
        std::string input, token;

        while (std::getline(std::cin, input)) {

            std::ofstream log("uci_log.txt", std::ios::app);
            log << input << "\n";
            log.flush();
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

                position.setBoard(startpos);
            }
            else if (token == "position") {

                iss >> token;

                if (token == "startpos") {

                    position.setBoard(startpos);
                    iss >> token; // remove moves from stream
                }
                else if (token == "fen") {

                    std::string fen;
                    while (iss >> token && token != "moves") {
                        fen += token + " ";
                    }

                    position.setBoard(fen);
                }
 
                while (iss >> token) {

                    History h;
                    Board board = position.getBoard();
                    Move move = convertMoveFromString(board, token);
                    makeMove(move, position, h);
                }
            }
            else if (token == "go") {

                int whiteTime = -1;
                int blackTime = -1;
                int whiteInc = 0;
                int blackInc = 0;
                int depth = 10;
                int movetime = -1;

                while (iss >> token) {

                    if (token == "depth")
                        iss >> depth;
                    else if (token == "movetime") {
                        iss >> movetime;
                        movetime = 3000;
                    }
                    else if (token == "wtime")
                        iss >> whiteTime;
                    else if (token == "btime")
                        iss >> blackTime;
                    else if (token == "winc")
                        iss >> whiteInc;
                    else if (token == "binc")
                        iss >> blackInc;
                }

                int myTime;

                if (movetime != -1)
                    myTime = movetime;
                else if (position.side() == WHITE)
                    myTime = whiteTime / 100 + whiteInc;
                else
                    myTime = blackTime / 100 + blackInc;
                
                uint64_t nodes = 0;
                Move bestMove = getBestMove(position, depth, nodes, myTime, false);
                std::cout << "bestmove " << convertMoveToString(bestMove) << std::endl;
            }
            else if (token == "play") {
                iss >> token;
                History h;
                Board board = position.getBoard();
                Move move = convertMoveFromString(board, token);
                makeMove(move, position, h);
            }
            else if (token == "d") {
                position.printBoard();
            }
            else if (token == "quit") {

                break;
            }
        }
    }
}