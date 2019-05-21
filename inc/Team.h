#ifndef TEAM_H
#define TEAM_H

#include "vectorUtils.h"

enum Team { BLACK = -1, WHITE = 1 };
Team other_team(Team team);
std::ostream& operator<< (std::ostream& out, Team team);


#endif