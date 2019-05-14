#include "Piece.h"

Queen::Queen(Team team) :
	Piece(team, "Queen")
{
	strength_ = team * 90;

	for (signed char i = -1; i < 2; i++)
		for (signed char j = -1; j < 2; j++)
			if (i == 0 && j == 0)
				moves_.push_back(glm::i8vec2(8 * i, 8 * j));

	std::string s = "";
	s.append(textureDir);
	if (*(s.end() - 1) == '/')
		s.append("Hetman.png");
	else
		s.append("/Hetman.png");
	load_shape_texture(s);

	shape_.setLocation(glm::vec2(200.0f, 100.0f));
}