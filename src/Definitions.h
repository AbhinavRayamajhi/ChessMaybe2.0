#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>

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
	NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST,
	NORTH_NORTH_EAST, NORTH_NORTH_WEST, SOUTH_SOUTH_EAST, SOUTH_SOUTH_WEST,
	NORTH_EAST_EAST, NORTH_WEST_WEST, SOUTH_EAST_EAST, SOUTH_WEST_WEST
};

enum MoveTypes : uint8_t{
	NORMAL,
	ENPASSANT,
	CASTLING,
	PROMOTION
};

enum MoveGenFlags : bool {
	SCORE_GEN_TRUE = true,
	SCORE_GEN_FALSE = false,
	CAPTURE_TRUE = true,
	CAPTURE_FALSE = false
};

// int to piece string array for printing promotions for move
inline const std::array<std::string, 8> intToPieceString { "", "P", "N", "B", "R", "Q", "K", "F"};

// char to int rank mapping
inline const std::map<char, File> fileToInt{
	
	{'a', 0},
	{'b', 1},
	{'c', 2},
	{'d', 3},
	{'e', 4},
	{'f', 5},
	{'g', 6},
	{'h', 7}
};

const std::map<char, Piece> promotionMap {
    {'q', QUEEN}, {'Q', QUEEN},
    {'r', ROOK},  {'R', ROOK},
    {'b', BISHOP},{'B', BISHOP},
    {'n', KNIGHT},{'N', KNIGHT},
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

constexpr std::array<Bitboard, 8> FILES = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };

// board rank definitions
inline constexpr Bitboard RANK_1 = 0x00000000000000FF;
inline constexpr Bitboard RANK_2 = 0x000000000000FF00;
inline constexpr Bitboard RANK_3 = 0x0000000000FF0000;
inline constexpr Bitboard RANK_4 = 0x00000000FF000000;
inline constexpr Bitboard RANK_5 = 0x000000FF00000000;
inline constexpr Bitboard RANK_6 = 0x0000FF0000000000;
inline constexpr Bitboard RANK_7 = 0x00FF000000000000;
inline constexpr Bitboard RANK_8 = 0xFF00000000000000;

constexpr std::array<Bitboard, 8> RANKS = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

// diagonals
inline constexpr Bitboard DIAG_1 = 0x0000000000000080;
inline constexpr Bitboard DIAG_2 = 0x0000000000008040;
inline constexpr Bitboard DIAG_3 = 0x0000000000804020;
inline constexpr Bitboard DIAG_4 = 0x0000000080402010;
inline constexpr Bitboard DIAG_5 = 0x0000008040201008;
inline constexpr Bitboard DIAG_6 = 0x0000804020100804;
inline constexpr Bitboard DIAG_7 = 0x0080402010080402;
inline constexpr Bitboard DIAG_8 = 0x8040201008040201;
inline constexpr Bitboard DIAG_9 = 0x4020100804020100;
inline constexpr Bitboard DIAG_10 = 0x2010080402010000;
inline constexpr Bitboard DIAG_11 = 0x1008040201000000;
inline constexpr Bitboard DIAG_12 = 0x0804020100000000;
inline constexpr Bitboard DIAG_13 = 0x0402010000000000;
inline constexpr Bitboard DIAG_14 = 0x0201000000000000;
inline constexpr Bitboard DIAG_15 = 0x0100000000000000;

constexpr std::array<Bitboard, 15> DIAGS = { DIAG_1, DIAG_2, DIAG_3, DIAG_4, DIAG_5, DIAG_6, DIAG_7, DIAG_8,
	DIAG_9, DIAG_10, DIAG_11, DIAG_12, DIAG_13, DIAG_14, DIAG_15};

// anti-diagonals
inline constexpr Bitboard ANTI_DIAG_1 = 0x0000000000000001;
inline constexpr Bitboard ANTI_DIAG_2 = 0x0000000000000102;
inline constexpr Bitboard ANTI_DIAG_3 = 0x0000000000010204;
inline constexpr Bitboard ANTI_DIAG_4 = 0x0000000001020408;
inline constexpr Bitboard ANTI_DIAG_5 = 0x0000000102040810;
inline constexpr Bitboard ANTI_DIAG_6 = 0x0000010204081020;
inline constexpr Bitboard ANTI_DIAG_7 = 0x0001020408102040;
inline constexpr Bitboard ANTI_DIAG_8 = 0x0102040810204080;
inline constexpr Bitboard ANTI_DIAG_9 = 0x0204081020408000;
inline constexpr Bitboard ANTI_DIAG_10 = 0x0408102040800000;
inline constexpr Bitboard ANTI_DIAG_11 = 0x0810204080000000;
inline constexpr Bitboard ANTI_DIAG_12 = 0x1020408000000000;
inline constexpr Bitboard ANTI_DIAG_13 = 0x2040800000000000;
inline constexpr Bitboard ANTI_DIAG_14 = 0x4080000000000000;
inline constexpr Bitboard ANTI_DIAG_15 = 0x8000000000000000;

constexpr std::array<Bitboard, 15> ANTI_DIAGS = {ANTI_DIAG_1, ANTI_DIAG_2, ANTI_DIAG_3, ANTI_DIAG_4, ANTI_DIAG_5,
	ANTI_DIAG_6, ANTI_DIAG_7, ANTI_DIAG_8, ANTI_DIAG_9, ANTI_DIAG_10, ANTI_DIAG_11, ANTI_DIAG_12, 
	ANTI_DIAG_13, ANTI_DIAG_14, ANTI_DIAG_15};

// full and empty boards
inline constexpr Bitboard FULL_BOARD =  0xFFFFFFFFFFFFFFFF;
inline constexpr Bitboard EMPTY_BOARD = 0x0000000000000000;

inline const std::array<int, PIECE_COUNT> materialVal{ 100, 350, 330, 525, 1000, INT32_MAX};

