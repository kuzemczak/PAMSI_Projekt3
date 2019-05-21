#include "Team.h"

Team other_team(Team team)
{
	if (team == WHITE)
		return BLACK;
	return WHITE;
}

std::ostream& operator<< (std::ostream& out, Team team)
{
	if (team == WHITE)
		out << "white";
	else 
		out << "black";

	return out;
}