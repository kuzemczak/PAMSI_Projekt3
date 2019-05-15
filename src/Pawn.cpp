#include "Piece.h"

Pawn::Pawn(Team team, int initialPosition) :
	Piece(team, "Pawn")
{
	strength_ = team * 10;

	// available moves with 'ray move' bit set
	if (team == WHITE)
	{
		moves_.push_back(gen_move(0, 8, PAWN_PUSH));
		moves_.push_back(gen_move(0, 8, PROMOTION));
		moves_.push_back(gen_move(0, 7, PAWN_CAPTURE));
		moves_.push_back(gen_move(0, 9, PAWN_CAPTURE));
		moves_.push_back(gen_move(0, 7, PAWN_CAPTURE | EP_CAPTURE));
		moves_.push_back(gen_move(0, 9, PAWN_CAPTURE | EP_CAPTURE));
		moves_.push_back(gen_move(0, 16, DOUBLE_PAWN_PUSH));
	}
	else
	{
		moves_.push_back(gen_move(0, 8, PAWN_PUSH | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 8, PROMOTION | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 7, PAWN_CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 9, PAWN_CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 7, PAWN_CAPTURE | EP_CAPTURE | NEGATIVE_MOVE));
		moves_.push_back(gen_move(0, 9, PAWN_CAPTURE | EP_CAPTURE | NEGATIVE_MOVE));
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

		if (moveCntr_ == 0 &&
			has_bits_set(m, DOUBLE_PAWN_PUSH) &&
			board[boardPosition_ + team_ * 8] == NULL &&
			board[pos] == NULL)
		{
			ret.push_back(gen_move(boardPosition_, pos, DOUBLE_PAWN_PUSH));
		}
		else if (has_bits_set(m, EP_CAPTURE) &&
			board[pos - team_ * 8] != NULL &&
			board[pos - team_ * 8]->get_name().compare("Pawn") == 0 && 
			moveHistory.size() != 0 &&
			has_bits_set(*(moveHistory.end() - 1), DOUBLE_PAWN_PUSH) &&
			GET_TO(*(moveHistory.end() - 1)) == (pos - team_ * 8) &&
			!((prevPos % 8) == 0 && (pos % 8) == 7) &&
			!((prevPos % 8) == 7 && (pos % 8) == 0))
		{
			ret.push_back(gen_move(boardPosition_, pos, EP_CAPTURE));
		}
		else if (has_bits_set(m, PAWN_PUSH) &&
			pos >= 0 && pos < 64 &&
			board[pos] == NULL)
		{
			ret.push_back(gen_move(boardPosition_, pos, QUIET_MOVE | PAWN_PUSH));
		}
		else if (has_bits_set(m, PAWN_CAPTURE) &&
			pos >= 0 && pos < 64 &&
			board[pos] != NULL &&
			!((prevPos % 8) == 0 && (pos % 8) == 7) &&
			!((prevPos % 8) == 7 && (pos % 8) == 0))
		{
			if (board[pos]->get_team() != team_)
				ret.push_back(gen_move(boardPosition_, pos, CAPTURE | PAWN_CAPTURE));
		}
	}

	return ret;
}