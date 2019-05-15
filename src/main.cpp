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

Window * window;

using namespace glm;

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
		window->clear();
		window->process_events();

		board.draw();

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));
	window->close();

	return 0;
}