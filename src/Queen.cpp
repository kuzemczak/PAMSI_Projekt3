#include "Piece.h"

Queen::Queen(Team team) :
	Piece(team, "Queen")
{
	strength_ = team * 90;

	// available moves with 'ray move' bit set
	moves_.push_back(9 | RAY_MOVE | NEGATIVE_MOVE);
	moves_.push_back(8 | RAY_MOVE | NEGATIVE_MOVE);
	moves_.push_back(7 | RAY_MOVE | NEGATIVE_MOVE);
	moves_.push_back(1 | RAY_MOVE | NEGATIVE_MOVE);
	moves_.push_back(1 | RAY_MOVE);
	moves_.push_back(7 | RAY_MOVE);
	moves_.push_back(8 | RAY_MOVE);
	moves_.push_back(9 | RAY_MOVE);

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) == '/')
		s.append("Hetman.png");
	else
		s.append("/Hetman.png");
	load_shape_texture(s);

	if (team == WHITE)
	{
		move_board(3);
	}
	else
	{
		move_board(60);
	}
}

std::vector<int> Queen::get_moves(std::vector<Piece*> board)
{
	std::vector<int> ret;
	for (int m : moves_)
	{
		if (has_bits_set(m, RAY_MOVE))
		{
			int pos = 0, change = 0;
			if (has_bits_set(m, NEGATIVE_MOVE))
				change = -(m & 63);
			else
				change = (m & 63);
			pos = boardPosition_ + change;

			// TODO: zastanowic sie jak rozwiazac problem z przechodzeniem do nowej linii podczas ray move
			while (pos >= 0 && pos < 64)
			{

			}
		}
	}

	return ret;
}