#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include "Window.h"
#include "shader.hpp"
#include "texture.hpp"
#include "Text.h"
#include "Rect.h"
#include "Piece.h"
#include "ChessBoard.h"
#include "ChessAI.h"

Window * window;

using namespace glm;

typedef std::chrono::high_resolution_clock Clock;

int main()
{
	window = new Window();
	window->init(600, 600, "Okno");

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	window->enableKeyboard();
	window->setBgcolor(0, 0, 0.4f, 0);
	Rect::setLocationMode(CENTER);
	DrawableObject::setAspectRatio(
		static_cast<GLuint>(window->width()),
		static_cast<GLuint>(window->height())
	);

	ChessBoard board(window->width(), window->height());

	do {
		if (board.current_team() == BLACK)
		{
			auto t0 = Clock::now();
			get_next_move(board, BLACK, 3);
			auto t1 = Clock::now();
			std::cout << "time: " 
				<< static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count())
				<< std::endl;
		}

		window->clear();
		window->process_events();

		board.draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));
	window->close();

	return 0;
}