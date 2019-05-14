#include "Piece.h"

King::King(Team team) :
	Piece(team, "King")
{
	strength_ = team * 900;

	for (signed char i = -1; i < 2; i++)
		for (signed char j = -1; j < 2; j++)
			if (i == 0 && j == 0)
				moves_.push_back(glm::i8vec2(i, j));

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) == '/')
		s.append("Krol.png");
	else
		s.append("/Krol.png");
	load_shape_texture(s);

	shape_.setLocation(glm::vec2(100.0f, 100.0f));
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
