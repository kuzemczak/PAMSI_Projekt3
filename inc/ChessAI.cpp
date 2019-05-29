#include "ChessAI.h"

#include "randutils.hpp"

int cntr = 0;

int get_next_move(ChessBoard & board, Team team, int depth)
{
	
	int ret = -100000 * team,
		bestMoveIndex = 0;
	std::vector<int> moves = board.get_possible_moves(team, true, true);
	std::vector<int> bestMoveIndices;

	for (int i = 0; i < moves.size(); i++)
	{
		int outcome = 0;
		if (depth > 1)
		{
			board.do_move(moves[i], false);
			outcome = minimax(board, other_team(team), depth - 1, ret);
			board.undo_moves(1, false);
		}
		else
		{
			board.do_move(moves[i], false, false);
			outcome = board.get_strength_balance();
			board.undo_moves(1, false, false);
		}

		if (outcome == ret)
			bestMoveIndices.push_back(i);
		else if ((team == BLACK && outcome < ret) ||
			(team == WHITE && outcome > ret))
		{
			ret = outcome;
			bestMoveIndices.clear();
			bestMoveIndices.push_back(i);
		}
	}
	ret = moves[bestMoveIndices[random(0, (int)bestMoveIndices.size() - 1)]];
	//board.do_move(moves[bestMoveIndex]);
	//std::cout << "moving time: " << cntr << std::endl;

	return ret;
}

int minimax(ChessBoard & board, Team team, int depth, int pruningParameter)
{
	//board.print_board();
	int ret = -100000 * team;
	
	std::vector<int> moves = board.get_possible_moves(team, true, true);

	bool pruningCondition = false;


	for (int i = 0; i < moves.size() && !pruningCondition; i++)
	{
		int outcome = 0;
		
		if (depth > 1)
		{
			board.do_move(moves[i], false);
			outcome = minimax(board, other_team(team), depth - 1, ret);
			board.undo_moves(1, false);
		}
		else
		{
			board.do_move(moves[i], false, false, false);
			outcome = board.get_strength_balance();
			board.undo_moves(1, false, false);
		}
		

		int a = random(0, 1);
		if (team == BLACK)
		{
			pruningCondition = outcome < pruningParameter;
			if (outcome < ret + a)
				ret = outcome;
		}
		else
		{
			pruningCondition = outcome > pruningParameter;
			if (outcome > ret - a)
				ret = outcome;
		}

	}

	return ret;
}