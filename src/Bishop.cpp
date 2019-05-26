#include "Piece.h"

Bishop::Bishop(Team team, int initialPosition) :
	Piece(BISHOP, team, "Bishop")
{
	strength_ = team * 30;

	// available moves with 'ray move' bit set
	moves_.push_back(gen_move(0, 9, RAY_MOVE | NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 7, RAY_MOVE | NEGATIVE_MOVE));
	moves_.push_back(gen_move(0, 7, RAY_MOVE));
	moves_.push_back(gen_move(0, 9, RAY_MOVE));

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) != '/')
		s.append("/");
	s.append("Goniec");
	if (team == WHITE)
		s.append("_bialy");
	s.append(".png");
	load_shape_texture(s);

	set_init_position(initialPosition);
}

std::vector<int> Bishop::get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
{
	std::vector<int> ret;
	for (int m : moves_)
	{
		int pos = 0, change = 0;
		if (has_bits_set(m, NEGATIVE_MOVE))
			change = -GET_TO(m);
		else
			change = GET_TO(m);
		pos = boardPosition_ + change;
		int prevPos = boardPosition_;

		while (	pos >= 0 &&
						pos < 64 &&
						!((prevPos % 8) == 0 && (pos % 8) == 7) &&
						!((prevPos % 8) == 7 && (pos % 8) == 0))
		{
			if (board[pos] == NULL)
				ret.push_back(gen_move(boardPosition_, pos, QUIET_MOVE | RAY_MOVE));
			else
			{
				ret.push_back(gen_move(boardPosition_, pos, CAPTURE | RAY_MOVE));
				break;
			}
			if (!has_bits_set(m, RAY_MOVE))
				break;
			prevPos = pos;
			pos += change;
		}
	}

	return ret;
}