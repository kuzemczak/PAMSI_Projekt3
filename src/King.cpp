#include "Piece.h"

King::King(Team team, int initialPosition) :
	Piece(KING, team, "King")
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
	moves_.push_back(gen_move(0, 2, QUEEN_CASTLE | NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 2, KING_CASTLE));

	set_init_position(initialPosition);

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) != '/')
		s.append("/");
	s.append("Krol");
	if (team == WHITE)
	{
		s.append("_bialy");
	}

	s.append(".png");
	load_shape_texture(s);

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

		int prevPos = boardPosition_;

		if (has_bits_set(m, QUEEN_CASTLE))
		{
			if (moveCntr_ == 0 &&
					board[boardPosition_ - 4] != NULL &&
					board[boardPosition_ - 4]->get_move_count() == 0 &&
					board[boardPosition_ - 3] == NULL &&
					board[boardPosition_ - 2] == NULL &&
					board[boardPosition_ - 1] == NULL)
			{
				ret.push_back(gen_move(boardPosition_, pos, QUEEN_CASTLE));
			}
		}
		else if (	has_bits_set(m, KING_CASTLE))
		{
			if (moveCntr_ == 0 &&
				board[boardPosition_ + 3] != NULL &&
				board[boardPosition_ + 3]->get_move_count() == 0 &&
				board[boardPosition_ + 2] == NULL &&
				board[boardPosition_ + 1] == NULL)
			{
				ret.push_back(gen_move(boardPosition_, pos, KING_CASTLE));
			}
		}
		else if (pos >= 0 && pos < 64 &&
			!((prevPos % 8) == 0 && (pos % 8) == 7) &&
			!((prevPos % 8) == 7 && (pos % 8) == 0))
		{
			if (board[pos] == NULL)
					ret.push_back(gen_move(boardPosition_, pos, QUIET_MOVE));
			else
			{
				ret.push_back(gen_move(boardPosition_, pos, CAPTURE));
			}
		}
	}

	return ret;
}