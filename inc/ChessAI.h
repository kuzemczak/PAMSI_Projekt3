#ifndef CHESSAI_H
#define CHESSAI_H

#include "Piece.h"
#include "ChessBoard.h"

extern int cntr;

int get_next_move(ChessBoard & board, Team team, int depth);
int minimax(ChessBoard & board, Team team, int depth, int pruningParameter);


#endif // !CHESSAI_H