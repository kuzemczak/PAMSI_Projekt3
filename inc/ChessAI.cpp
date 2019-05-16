#include "ChessAI.h"

#include "randutils.hpp"

std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) {
	return std::uniform_int_distribution<T>{min, max}(gen);
}

int ChessAI::get_next_move(Team team, const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
{
	int ret = 0;

	std::vector<int> moves;

	for (Piece * p : board)
	{
		if (p != NULL && p->get_team() == team)
		{
			std::vector<int> m = p->get_moves(board, moveHistory);
			moves.insert(moves.end(), m.begin(), m.end());
		}
	}

	ret = moves[random(0, (int)moves.size() - 1)];

	return ret;
}