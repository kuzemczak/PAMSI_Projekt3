#include "Piece.h"

Knight::Knight(Team team, int initialPosition) :
	Piece(team, "Knight")
{
	strength_ = team * 30;

	// available moves with 'ray move' bit set
	moves_.push_back(gen_move(0, 6 , NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 10, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 15, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 17, NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 6 , 0));
	moves_.push_back(gen_move(0, 10, 0));
	moves_.push_back(gen_move(0, 15, 0));
	moves_.push_back(gen_move(0, 17, 0));

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) != '/')
		s.append("/");
	s.append("Skoczek");
	if (team == WHITE)
		s.append("_bialy");
	s.append(".png");
	load_shape_texture(s);

	set_init_position(initialPosition);
}

std::vector<int> Knight::get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
{
	std::vector<int> ret;

	for (int m : moves_)
	{
		int pos = 0;
		if (has_bits_set(m, NEGATIVE_MOVE))
			pos = -GET_TO(m) + boardPosition_;
		else
			pos = GET_TO(m) + boardPosition_;
		int prevPos = boardPosition_;

		if (pos >= 0 && pos < 64 &&
				!((prevPos % 8) < 2 && (pos % 8) > 5) &&
				!((prevPos % 8) > 5 && (pos % 8) < 2))
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