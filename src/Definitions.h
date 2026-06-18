#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using Bitboard 		= uint64_t;
using Key         	= uint64_t;
using Color 		= bool;
using Piece         = uint8_t;
using Square        = uint8_t;
using CastlingRight = uint8_t;
using MoveType      = uint8_t;
using Rank          = uint8_t;
using File 			= uint8_t;

// for side to move and occupancy bitboards 
enum Colors : uint8_t {
	
	BLACK, WHITE,
	BOTH, OCC_COUNT
};

// for piece in bitboard and move(including promotions)
enum Pieces : uint8_t {

	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
	PIECE_NONE, PIECE_COUNT = PIECE_NONE,
	NO_PROMOTION = PAWN
};

// little endian rank-file Mapping
enum Squares : uint8_t {

	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	SQ_NONE, SQ_COUNT = SQ_NONE
};

// for castling array
enum CastlingRights : uint8_t {

	NO_CASTLING,

	WHITE_OO,
	WHITE_OOO = WHITE_OO << 1,
	BLACK_OO  = WHITE_OO << 2,
	BLACK_OOO = WHITE_OO << 3,

	WHITE_CASTLE = WHITE_OO | WHITE_OOO,
	BLACK_CASTLE = BLACK_OO | BLACK_OOO
};

// bishop and rook directions
enum Direction : uint8_t {

	NORTH, SOUTH, EAST, WEST,
	NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST
};

enum MoveTypes : uint8_t{
	NORMAL,
	ENPASSANT,
	CASTLING,
	PROMOTION
};

// int to piece string array for printing promotions for move
inline const std::array<std::string, 8> intToPieceString { "", "P", "N", "B", "R", "Q", "K", "F"};

// char to int rank mapping
inline const std::map<char, File> fileToInt {
	{'a', 0},
	{'b', 1},
	{'c', 2},
	{'d', 3},
	{'e', 4},
	{'f', 5},
	{'g', 6},
	{'h', 7}
};

// board file definitions
inline constexpr Bitboard FILE_A = 0x0101010101010101;
inline constexpr Bitboard FILE_B = 0x0202020202020202;
inline constexpr Bitboard FILE_C = 0x0404040404040404;
inline constexpr Bitboard FILE_D = 0x0808080808080808;
inline constexpr Bitboard FILE_E = 0x1010101010101010;
inline constexpr Bitboard FILE_F = 0x2020202020202020;
inline constexpr Bitboard FILE_G = 0x4040404040404040;
inline constexpr Bitboard FILE_H = 0x8080808080808080;

// board rank definitions
inline constexpr Bitboard RANK_1 = 0x00000000000000FF;
inline constexpr Bitboard RANK_2 = 0x000000000000FF00;
inline constexpr Bitboard RANK_3 = 0x0000000000FF0000;
inline constexpr Bitboard RANK_4 = 0x00000000FF000000;
inline constexpr Bitboard RANK_5 = 0x000000FF00000000;
inline constexpr Bitboard RANK_6 = 0x0000FF0000000000;
inline constexpr Bitboard RANK_7 = 0x00FF000000000000;
inline constexpr Bitboard RANK_8 = 0xFF00000000000000;

// full and empty boards
inline constexpr Bitboard FULL_BOARD =  0xFFFFFFFFFFFFFFFF;
inline constexpr Bitboard EMPTY_BOARD = 0x0000000000000000;




