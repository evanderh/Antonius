#ifndef ANTONIUS_TYPES_H
#define ANTONIUS_TYPES_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

using VecStr = std::vector<std::string>;
using U64 = uint64_t;
using U32 = uint32_t;
using U16 = uint16_t;
using U8  = uint8_t;
using I64 = int64_t;
using I32 = int32_t;
using I16 = int16_t;
using I8  = int8_t;

enum Color : U8 {
    BLACK, WHITE,
    NCOLORS = 2
};

enum Square : U8 {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    INVALID = 64,
    NSQUARES = 64
};

enum File : I8 {
    FILE1, FILE2, FILE3, FILE4, FILE5, FILE6, FILE7, FILE8
};

enum Rank : I8 {
    RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};

enum PieceType : U8 {
    NONE = 0, ALL = 0,
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    NPIECETYPES = 6
};

enum Piece : U8 {
    EMPTY =  0,
    WPAWN = 1, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,
    BPAWN = 9, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

enum CastleRights : U8 {
    NO_CASTLE = 0x0,
    BLACK_OOO = 0x1,
    BLACK_OO =  0x2,
    WHITE_OOO = 0x4,
    WHITE_OO =  0x8,
    BLACK_CASTLE = BLACK_OO | BLACK_OOO,
    WHITE_CASTLE = WHITE_OO | WHITE_OOO,
    ALL_CASTLE = BLACK_CASTLE | WHITE_CASTLE
};

enum CastleDirection {
    KINGSIDE, QUEENSIDE
};

enum MoveType : U8 {
    NORMAL,
    PROMOTION,
    ENPASSANT,
    CASTLE,
    NULLMOVE
};

enum MoveGenType {
    EVASIONS,
    CAPTURES,
    QUIETS
};

enum class PawnMove {
    LEFT = 7,
    PUSH = 8,
    RIGHT = 9,
    DOUBLE = 16
};

enum Score {
    DRAWSCORE = 0,

    PAWNSCORE = 100,
    KNIGHTSCORE = 320,
    BISHOPSCORE = 330,
    ROOKSCORE = 500,
    QUEENSCORE = 900,

    TOTALPHASE = 2*QUEENSCORE + 4*ROOKSCORE + 4*BISHOPSCORE + 4*KNIGHTSCORE + 16*PAWNSCORE,
    
    KINGSCORE = 20000,
    MATESCORE = 32000,
};

enum Phase {
    OPENING = 0,
    ENDGAME = 1,
};

enum NodeType : U8 {
    TT_NONE,
    TT_EXACT,
    TT_ALPHA,
    TT_BETA,
};

// Define type helper functions
namespace Types {

    inline Square getSquare(const File file, const Rank rank)
    {
        return Square(rank * 8 + file);
    }
    
    inline Square getSquare(const std::string& square)
    {
        auto file = File((int) square[0] - 'a');
        auto rank = Rank(((int)square[1] - '0' - 1));

        return getSquare(file, rank);
    }

    inline Rank getRank(const Square square)
    {
        return Rank(square >> 3);
    }

    inline File getFile(const Square square)
    {
        return File(square & 7);
    }

    inline bool validRank(const Rank rank)
    {
        return (RANK1 <= rank) && (rank <= RANK8);
    }

    inline bool validFile(const File file)
    {
        return (FILE1 <= file) && (file <= FILE8);
    }

    inline Piece makePiece(const Color c, const PieceType p)
    {
        return Piece((c << 3) | p);
    }

    inline PieceType getPieceType(const Piece p)
    {
        return PieceType(p & 0x7);
    }

    inline Color getPieceColor(const Piece p)
    {
        return Color(p >> 3);
    }

    template<Color c, PawnMove p, bool forward>
    inline Square move(const Square sq)
    {
        if (forward)
        {
            if (c == WHITE)
                return Square(sq + static_cast<int>(p));
            else // if c == BLACK
                return Square(sq - static_cast<int>(p));
        }
        else
        {
            if (c == WHITE)
                return Square(sq - static_cast<int>(p));
            else // c == BLACK
                return Square(sq + static_cast<int>(p));
        }
    }

    template<PawnMove p, bool forward>
    inline Square move(const Square sq, const Color c)
    {
        if (c == WHITE)
            return move<WHITE, p, forward>(sq);
        else
            return move<BLACK, p, forward>(sq);
    }

    const char PieceChar[16] = {
        ' ', 'p', 'n', 'b', 'r', 'q', 'k', ' ',
        ' ', 'P', 'N', 'B', 'R', 'Q', 'K', ' '
    };

}


// Define common type operators

inline Color operator~(Color c)
{
    return Color(c ^ WHITE);
}

inline std::ostream& operator<<(std::ostream& os, File file)
{
    os << static_cast<char>('a' + file);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, Rank rank)
{
    os << static_cast<char>('1' + rank);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, Square sq)
{
    os << Types::getFile(sq) << Types::getRank(sq);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, Piece p)
{
    os << Types::PieceChar[p];
    return os;
}

// Enable arithmetic and bitwise operators
#define ENABLE_OPERATORS(T)                             \
inline T operator+(T d1, T d2) { return T(int(d1) + int(d2)); } \
inline T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
inline T operator-(T d1, T d2) { return T(int(d1) - int(d2)); } \
inline T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
inline T operator*(T d, int i) { return T(int(d) * i); }        \
inline T operator*(int i, T d) { return T(i * int(d)); }        \
inline T operator/(T d1, T d2) { return T(int(d1) / int(d2)); } \
inline T operator/(T d, int i) { return T(int(d) / i); }        \
inline T& operator+=(T& d1, T d2) { return d1 = d1 + d2; }      \
inline T& operator-=(T& d1, T d2) { return d1 = d1 - d2; }      \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); } \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); } \
inline T& operator++(T& d, int) { return d = T(int(d) + 1); }   \
inline T& operator--(T& d, int) { return d = T(int(d) - 1); }   \
inline T& operator&=(T& d1, T d2) { return d1 = T(d1 & d2); }   \
inline T& operator|=(T& d1, T d2) { return d1 = T(d1 | d2); }   \

ENABLE_OPERATORS(Square)
ENABLE_OPERATORS(File)
ENABLE_OPERATORS(Rank)
ENABLE_OPERATORS(CastleRights)
ENABLE_OPERATORS(Score)

#undef ENABLE_OPERATORS

#endif