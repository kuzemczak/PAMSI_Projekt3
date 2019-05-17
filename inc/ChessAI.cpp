#include "ChessAI.h"

#include "randutils.hpp"

std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) {
	return std::uniform_int_distribution<T>{min, max}(gen);
}

int get_next_move(ChessBoard & board, Team team, int depth)
{
	
	int ret = -100000 * team,
		bestMoveIndex = 0;
	std::vector<int> moves = board.get_possible_moves(team);

	for (int i = 0; i < moves.size(); i++)
	{
		board.do_move(moves[i], true);
		int outcome = 0;
		if (depth > 1)
		{
			if (team == BLACK)
				outcome = minimax(board, WHITE, depth - 1);
			else
				outcome = minimax(board, BLACK, depth - 1);
		}
		else
			outcome = board.get_strength_balance();
		if ((team == BLACK && outcome < ret) ||
			(team == WHITE && outcome > ret))
		{
			ret = outcome;
			bestMoveIndex = i;
		}
		board.undo_moves(1, true);
	}
	board.do_move(moves[bestMoveIndex]);

	return ret;
}

int minimax(ChessBoard & board, Team team, int depth)
{
	int ret = -100000 * team;
	std::vector<int> moves = board.get_possible_moves(team);

	for (int i = 0; i < moves.size(); i++)
	{
		board.do_move(moves[i], true);
		int outcome = 0;
		if (depth > 1)
		{
			if (team == BLACK)
				outcome = minimax(board, WHITE, depth - 1);
			else
				outcome = minimax(board, BLACK, depth - 1);
		}
		else
			outcome = board.get_strength_balance();
		if ((team == BLACK && outcome < ret) ||
			(team == WHITE && outcome > ret))
		{
			ret = outcome;
		}
		board.undo_moves(1, true);
	}

	return ret;
}