#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "stb_image.h"

#include "Window.h"
#include "shader.hpp"
#include "texture.hpp"
#include "Text.h"
#include "Rect.h"
#include "Piece.h"
#include "ChessBoard.h"
#include "ChessAI.h""
#include "Button.h"
#include "BalanceDisplay.h"
#include "Clock.h"
#include "Game.h"

Window * window;

using namespace glm;

int main()
{
#ifdef _WIN32
	//FreeConsole();
#endif

	window = new Window();
	window->init(1000, 600, "Okno");

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	window->enableKeyboard();
	window->setBgcolor(0.957f, 0.827f, 0.659f, 0);
	Rect::setLocationMode(CENTER);
	DrawableObject::setAspectRatio(
		static_cast<GLuint>(window->width()),
		static_cast<GLuint>(window->height())
	);

	//ChessBoard * board = new ChessBoard(600, 600);
	//
	//Button button("", 730.0f, 150.0f, 90.0f, 60.0f, window);
	//button.load_icon("data/new_game_icon.png");
	//Button hintButton("", 870.0f, 150.0f, 90.0f, 60.0f, window);
	//hintButton.load_icon("data/bulb_icon.png");
	//
	//BalanceDisplay balanceDisplay(800.0f, 400.0f, 360.0f, 50.0f,
	//	0, -210, 210);
	//	
	//Clock clock(610, 440, 380, 150, glm::vec4(0.643f, 0.267f, 0.067f, 0.0f));
	//
	//int clockTimeMillis = 0, clockTimeSec = 0, clockTimeMin = 0;
	//auto prevTime = Clock_t::now();
	//
	//bool prevNewGameButton = false,
	//	prevHintButton = false;

	Game game(window);
	do {
		//if (board->current_team() == BLACK)
		//{
		//	//auto t0 = Clock_t::now();
		//	board->do_move(get_next_move(*board, BLACK, 3));
		//	//auto t1 = Clock_t::now();
			//std::cout << "time: " 
			//	<< static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count())
			//	<< std::endl;
		//}
		//
		//if (button.is_pressed())
		//{
		//	if (!prevNewGameButton)
		//	{
		//		prevNewGameButton = true;
		//		delete board;
		//		board = new ChessBoard(600, 600);
		//		clock.reset();
		//	}
		//}
		//else
		//{
		//	prevNewGameButton = false;
		//}
		//
		//if (hintButton.is_pressed())
		//{
		//	if (!prevHintButton)
		//	{
		//		prevHintButton = true;
		//		int hint = get_next_move(*board, board->current_team(), 4);
		//		board->display_hint(hint);
		//	}
		//}
		//else
		//{
		//	prevHintButton = false;
		//}
		//
		//balanceDisplay.set_value(board->get_strength_balance());
		//
		//clock.update();

		window->clear();

		game.update();
		window->process_events();
		game.draw();
		//board->draw();
		//button.draw();
		//hintButton.draw();
		//balanceDisplay.draw();
		//clock.draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));
	window->close();

	return 0;
}