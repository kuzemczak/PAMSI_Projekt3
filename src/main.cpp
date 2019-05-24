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

	//window->set_cursor(GLFW_HAND_CURSOR);

	ChessBoard * board = new ChessBoard(600, 600);
	Text text("00:00", glm::vec4(0.643f, 0.267f, 0.067f, 0.0f));
	text.setLocation(glm::vec2(650.0f, 500.0f));
	text.generateShaders();
	text.loadFont("fonts/tt0351m_.ttf", 100);
	//
	//Rect::setLocationMode(TOP_LEFT);
	//Rect rect(360.0f, 240.0f, 620.0f, 300.0f, 0.0f);
	//int width, height, nrChannels;
	//
	//stbi_set_flip_vertically_on_load(true);
	//unsigned char *data = stbi_load("data/button.png", &width, &height, &nrChannels, 0);
	//rect.generateTexture(data, width, height, nrChannels);
	//stbi_image_free(data);

	Button button("", 800.0f, 150.0f, 90.0f, 60.0f, window);
	button.load_icon("data/new_game_icon.png");

	BalanceDisplay balanceDisplay(800.0f, 400.0f, 360.0f, 50.0f,
		0, -210, 210);
		
	int clockTimeMillis = 0, clockTimeSec = 0, clockTimeMin = 0;
	auto prevTime = Clock::now();

	bool prevButton = false;
	int prevVal = 0;
	do {
		if (board->current_team() == BLACK)
		{
			auto t0 = Clock::now();
			get_next_move(*board, BLACK, 3);
			auto t1 = Clock::now();
			std::cout << "time: " 
				<< static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count())
				<< std::endl;
		}
		auto t2 = Clock::now();
		clockTimeMillis += static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - prevTime).count());
		prevTime = t2;
		if (clockTimeMillis > 999)
		{
			clockTimeMillis = clockTimeMillis % 1000;
			clockTimeSec++;
		}
		if (clockTimeSec > 59)
		{
			clockTimeSec = clockTimeSec % 60;
			clockTimeMin++;
		}
		std::string s = "";
		if (clockTimeMin < 10)
			s.append("0");
		s.append(std::to_string(clockTimeMin));
		s.append(":");
		if (clockTimeSec < 10)
			s.append("0");
		s.append(std::to_string(clockTimeSec));
		text.setText(s);
		
		if (button.is_pressed())
		{
			if (!prevButton)
			{
				prevButton = true;
				delete board;
				board = new ChessBoard(600, 600);
			}
		}
		else
		{
			prevButton = false;
		}
		
		if (board->get_strength_balance() != prevVal)
		{
			prevVal = board->get_strength_balance();
			balanceDisplay.set_value(prevVal);
		}

		window->clear();
		window->process_events();

		board->draw();
		button.draw();
		balanceDisplay.draw();
		text.draw();
		//rect.draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));
	window->close();

	return 0;
}