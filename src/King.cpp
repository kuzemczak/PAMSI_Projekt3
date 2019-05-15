#include "Piece.h"

King::King(Team team) :
	Piece(team, "King")
{
	strength_ = team * 900;

	moves_.push_back(gen_move(0, 9, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 8, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 7, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 1, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 1, 0));
	moves_.push_back(gen_move(0, 7, 0));
	moves_.push_back(gen_move(0, 8, 0));
	moves_.push_back(gen_move(0, 9, 0));
	
	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) != '/')
		s.append("/");
	s.append("Krol");
	if (team == WHITE)
	{
		set_init_position(4);
		moves_.push_back(gen_move(0, 2, QUEEN_CASTLE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 2, KING_CASTLE));
		s.append("_bialy");
	}
	else
	{
		set_init_position(59);
		moves_.push_back(gen_move(0, 2, QUEEN_CASTLE));
		moves_.push_back(gen_move(0, 2, KING_CASTLE | NEGATIVE_MOVE));
	}

	s.append(".png");
	load_shape_texture(s);
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

std::vector<int> King::get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
{
	std::vector<int> ret;

	for (int m : moves_)
	{
		int pos = 0;
		if (has_bits_set(m, NEGATIVE_MOVE))
			pos = - GET_TO(m) + boardPosition_;
		else
			pos = GET_TO(m) + boardPosition_;

		if (moveCntr_ == 0 &&
				has_bits_set(m, QUEEN_CASTLE))
		{
			if (board[boardPosition_ - team_ * 4] != NULL &&
					board[boardPosition_ - team_ * 4]->get_move_count() == 0 &&
					board[boardPosition_ - team_ * 3] == NULL &&
					board[boardPosition_ - team_ * 2] == NULL &&
					board[boardPosition_ - team_ * 1] == NULL)
			{
				ret.push_back(gen_move(boardPosition_, pos, QUEEN_CASTLE));
			}
		}
		else if (	moveCntr_ == 0 && 
							has_bits_set(m, KING_CASTLE) &&
							board[boardPosition_ + team_ * 3] != NULL &&
							board[boardPosition_ + team_ * 3]->get_move_count() == 0 &&
							board[boardPosition_ + team_ * 2] == NULL && 
							board[boardPosition_ + team_ * 1] == NULL)
		{
			ret.push_back(gen_move(boardPosition_, pos, KING_CASTLE));
		}
		else if (pos >= 0 && pos < 64)
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