#include "Game.h"

Game::Game(Window * parentWindow) :
	parentWindow_(parentWindow)
{
	handle_events(this);
	board_ = new ChessBoard(600, 600);

	buttonFlags_ = new bool[2];
	buttonFlags_[0] = false;
	buttonFlags_[1] = false;

	newGameButton_ = new Button("", 730.0f, 150.0f, 90.0f, 60.0f, parentWindow, &(buttonFlags_[0]));
	newGameButton_->load_icon("data/new_game_icon.png");
	hintButton_ = new Button("", 870.0f, 150.0f, 90.0f, 60.0f, parentWindow, &(buttonFlags_[1]));
	hintButton_->load_icon("data/bulb_icon.png");
	
	buttons_.push_back(newGameButton_);
	buttons_.push_back(hintButton_);

	balanceDisplay_ = new BalanceDisplay(800.0f, 400.0f, 360.0f, 50.0f,
		0, -210, 210);

	clock_ = new Clock(610, 440, 380, 150, glm::vec4(0.643f, 0.267f, 0.067f, 0.0f));

	gameEndPopup_ = new Popup(300, 300, 400, 133, "");
	gameEndPopup_->load_icon("data/cup_icon.png");

	objects_.push_back(board_);
	objects_.push_back(clock_);
	objects_.push_back(newGameButton_);
	objects_.push_back(hintButton_);
	objects_.push_back(balanceDisplay_);
	objects_.push_back(gameEndPopup_);
}

Game::~Game()
{
	for (GraphicalGameObjectInterface * o : objects_)
		if (o != NULL)
			delete o;
}

void Game::update()
{
	clock_->update();

	if (buttonFlags_[0])
	{
		new_game();
		buttonFlags_[0] = false;
	}
	if (buttonFlags_[1])
	{
		hint_event();
		buttonFlags_[1] = false;
	}

	if (board_->current_team() == BLACK)
	{
		auto t0 = Clock_t::now();
		int move = get_next_move(*board_, BLACK, 3);
		auto t1 = Clock_t::now();
		std::cout << "time: "
			<< static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
		board_->do_move(move);
	}
}

void Game::draw()
{
	for (GraphicalGameObjectInterface * o : objects_)
		if (o != NULL)
			o->draw();
}

//////////////////////////////////// Events
void Game::new_game()
{
	delete board_;
	board_ = new ChessBoard(600, 600);
	objects_[0] = board_;
	clock_->reset();
	gameEndPopup_->hide();
}

void Game::hint_event()
{
	int hint = get_next_move(*board_, board_->current_team(), 4);
	board_->display_hint(hint);
}

void Game::checkmate_event(Team checkedTeam)
{
	std::string s = "";
	if (checkedTeam == WHITE)
		s.append("Czarne");
	else
		s.append("Biale");

	s.append(" wygraly gre!");

	gameEndPopup_->set_text(s);
	gameEndPopup_->show();
	board_->disable();
}

void Game::check_event(Team checkedTeam)
{

}