#include "Piece.h"

King::King(Team team) :
	Piece(team, "King")
{
	for (signed char i = -1; i < 2; i++)
		for (signed char j = -1; j < 2; j++)
			if (i == 0 && j == 0)
				moves_.push_back(glm::i8vec2(i, j));
}