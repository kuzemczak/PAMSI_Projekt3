#include "ChessAI.h"

#include "randutils.hpp"

std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) {
	return std::uniform_int_distribution<T>{min, max}(gen);
}

int cntr = 0;

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
			outcome = minimax(board, other_team(team), depth - 1, ret);
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
	//std::cout << "moving time: " << cntr << std::endl;

	return ret;
}

int minimax(ChessBoard & board, Team team, int depth, int pruningParameter)
{
	int ret = -100000 * team;
	
	std::vector<int> moves = board.get_possible_moves(team);

	bool pruningCondition = false;


	for (int i = 0; i < moves.size() && !pruningCondition; i++)
	{
		board.do_move(moves[i], true);
		//std::cout << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count())
		//	<< std::endl;
		int outcome = 0;
		if (depth > 1)
		{
			outcome = minimax(board, other_team(team), depth - 1, ret);
		}
		else
			outcome = board.get_strength_balance();

		if (team == BLACK)
		{
			pruningCondition = outcome < pruningParameter;
			if (outcome < ret)
				ret = outcome;
		}
		else
		{
			pruningCondition = outcome > pruningParameter;
			if (outcome > ret)
				ret = outcome;
		}
		//auto t0 = Clock::now();
		board.undo_moves(1, true);
		//auto t1 = Clock::now();
		//cntr += static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count());
	}

	return ret;
}