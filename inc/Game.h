#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "ChessBoard.h"
#include "Button.h"
#include "Clock.h"
#include "BalanceDisplay.h"
#include "Popup.h"
#include "ChessAI.h"

class Game : Events
{
	Window * parentWindow_;

	ChessBoard * board_;
	Button *newGameButton_,
		*hintButton_;
	std::vector<Button*> buttons_;
	bool * buttonFlags_;
	BalanceDisplay * balanceDisplay_;
	Clock *clock_;
	Popup *gameEndPopup_;

	std::vector<GraphicalGameObjectInterface*> objects_;

public:
	Game(Window * parentWindow);
	~Game();

	void update();
	void draw();

	//////////////////////////////////// Events
	void new_game();
	void hint_event();
	void checkmate_event(Team checkedTeam);
	void check_event(Team checkedTeam);

};

#endif