#include "Piece.h"

King::King(Team team) :
	Piece(team, "King")
{
	strength_ = team * 900;

	moves_.push_back(9 | NEGATIVE_MOVE);
	moves_.push_back(8 | NEGATIVE_MOVE);
	moves_.push_back(7 | NEGATIVE_MOVE);
	moves_.push_back(1 | NEGATIVE_MOVE);
	moves_.push_back(1);
	moves_.push_back(7);
	moves_.push_back(8);
	moves_.push_back(9);
	moves_.push_back(2 | QUEEN_CASTLE | NEGATIVE_MOVE);
	moves_.push_back(2 | KING_CASTLE);

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) == '/')
		s.append("Krol.png");
	else
		s.append("/Krol.png");
	load_shape_texture(s);

	if (team == WHITE)
	{
		move_board(4);
	}
	else
	{
		move_board(59);
	}
}

void King::move_board(glm::i8vec2 boardPosition)
{
	if (moveCntr_ == 0)
	{
		moves_.pop_back();
		moves_.pop_back();
	}
	Piece::move_board(boardPosition);
}

void King::move_board(GLuint xx, GLuint yy)
{
	if (moveCntr_ == 0)
	{
		moves_.pop_back();
		moves_.pop_back();
	}
	Piece::move_board(xx, yy);
}

void King::move_board(int pos)
{
	if (moveCntr_ == 0)
	{
		moves_.pop_back();
		moves_.pop_back();
	}
	Piece::move_board(pos);
}

std::vector<int> King::get_moves(std::vector<Piece*> board)
{
	std::vector<int> ret;

	for (int m : moves_)
	{
		int pos = 0;
		if (m & NEGATIVE_MOVE == NEGATIVE_MOVE)
			pos = - (m & 63) + boardPosition_;
		else
			pos = (m & 63) + boardPosition_;

		if (moveCntr_ == 0 &&
			((m & QUEEN_CASTLE) == QUEEN_CASTLE) &&
			board[boardPosition_ - 4]->get_move_count() == 0)
		{
			ret.push_back(gen_move(boardPosition_, pos, QUEEN_CASTLE));
		}
		else if (moveCntr_ == 0 && 
			((m & KING_CASTLE) == KING_CASTLE) &&
			board[boardPosition_ + 3]->get_move_count() == 0)
		{
			ret.push_back(gen_move(boardPosition_, pos, KING_CASTLE));
		}
		else if (pos >= 0 &&
			pos < 64)
		{
			if (board[pos] == NULL)
				ret.push_back(gen_move(boardPosition_, pos, QUIET_MOVE));
			else
			{
				if (board[pos]->get_team() != team_)
					ret.push_back(gen_move(boardPosition_, pos, CAPTURE));
			}
		}
	}
	return ret;
}