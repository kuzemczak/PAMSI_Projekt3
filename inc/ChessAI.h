#ifndef CHESSAI_H
#define CHESSAI_H

#include "Piece.h"

class ChessAI
{
public:
	static int get_next_move(Team team, const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

#endif // !CHESSAI_H

