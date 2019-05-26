#include "Piece.h"

Pawn::Pawn(Team team, int initialPosition) :
	Piece(PAWN, team, "Pawn")
{
	strength_ = team * 10;

	// available moves with 'ray move' bit set
	if (team == WHITE)
	{
		moves_.push_back(gen_move(0, 8, QUIET_MOVE));
		moves_.push_back(gen_move(0, 8, PROMOTION));
		moves_.push_back(gen_move(0, 7, CAPTURE));
		moves_.push_back(gen_move(0, 9, CAPTURE));
		moves_.push_back(gen_move(0, 7, CAPTURE | EP_CAPTURE));
		moves_.push_back(gen_move(0, 9, CAPTURE | EP_CAPTURE));
		moves_.push_back(gen_move(0, 16, DOUBLE_PAWN_PUSH));
	}
	else
	{
		moves_.push_back(gen_move(0, 8, QUIET_MOVE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 8, PROMOTION | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 7, CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 9, CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 7, CAPTURE | EP_CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 9, CAPTURE | EP_CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 16, DOUBLE_PAWN_PUSH | NEGATIVE_MOVE));
	}

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) != '/')
		s.append("/");
	s.append("Pion");
	if (team == WHITE)
		s.append("_bialy");
	s.append(".png");
	load_shape_texture(s);

	set_init_position(initialPosition);
}

std::vector<int> Pawn::get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
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


		if (has_bits_set(m, DOUBLE_PAWN_PUSH))
		{
			if (moveCntr_ == 0 &&
				board[boardPosition_ + team_ * 8] == NULL &&
				board[pos] == NULL)
			{
				ret.push_back(gen_move(boardPosition_, pos, DOUBLE_PAWN_PUSH));
			}
		}
		else if (has_bits_set(m, EP_CAPTURE))
		{
			if (pos - team_ * 8 > 0 &&
				pos - team_ * 8 < 63 && 
				board[pos - team_ * 8] != NULL &&
				board[pos - team_ * 8]->get_type() == PAWN &&
				board[pos - team_ * 8]->get_team() != team_ &&
				moveHistory.size() > 0 &&
				has_bits_set(*(moveHistory.end() - 1), DOUBLE_PAWN_PUSH) &&
				GET_TO(*(moveHistory.end() - 1)) == (pos - team_ * 8) &&
				!((prevPos % 8) == 0 && (pos % 8) == 7) &&
				!((prevPos % 8) == 7 && (pos % 8) == 0))
			{
				ret.push_back(gen_move(boardPosition_, pos, CAPTURE | EP_CAPTURE));
			}
		}
		else if (has_bits_set(m, CAPTURE))
		{
			if (pos >= 0 && pos < 64 &&
				!((prevPos % 8) == 0 && (pos % 8) == 7) &&
				!((prevPos % 8) == 7 && (pos % 8) == 0))
			{
				ret.push_back(gen_move(boardPosition_, pos, CAPTURE | PAWN_CAPTURE));
			}
		}
		else if (!has_bits_set(m, PROMOTION) &&
			pos >= 0 && pos < 64 &&
			board[pos] == NULL)
		{
			ret.push_back(gen_move(boardPosition_, pos, QUIET_MOVE | PAWN_PUSH));
		}
	}

	return ret;
}