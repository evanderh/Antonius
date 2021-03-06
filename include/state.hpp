#ifndef ANTONIUS_STATE_H
#define ANTONIUS_STATE_H

#include "types.hpp"

class Board;

struct State {

    State() = default;
    State(const State&) = default;

    State(const State& state, Move mv)
        : zkey(state.zkey)
        , pawnzkey(state.pawnzkey)
        , move(mv)
        , castle(state.castle)
        , hmClock(state.hmClock + 1)
    { }

    // Check info bitboards
    BB checkingPieces = 0;
    BB pinnedPieces = 0;
    BB discoveredCheckers = 0;
    BB checkingSquares[6] = { 0, 0, 0, 0, 0, 0 };

    // Hash keys
    U64 zkey = 0;
	U64 pawnzkey = 0;

    // Board state variables
    Move move;
    PieceType captured = NONE;
    CastleRights castle = ALL_CASTLE;
    Square enPassantSq = INVALID;
    U8 hmClock = 0;
};

#endif