#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Window.h"
#include "shader.hpp"
#include "texture.hpp"
#include "Text.h"
#include "Rect.h"
#include "Piece.h"

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
	DrawableObject::setAspectRatio(window->width(), window->height());

	//Piece p;

	std::vector<Triangles*> chess;

	Rect pion(75, vec2(400.0f, 300.0f)),
		wieza(75, vec2(110.0f, 110.0f)),
		hetman(75, vec2(220.0f, 110.0f)),
		goniec(75, vec2(330.0f, 110.0f)),
		skoczek(75, vec2(440.0f, 110.0f)),
		krol(75, vec2(550.0f, 110.0f)),
		chessBoard(600, vec2(300.0f, 300.0f));

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("data/Pion.png", &width, &height, &nrChannels, 0);
	pion.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&pion);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/Wieza.png", &width, &height, &nrChannels, 0);
	wieza.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&wieza);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/Hetman.png", &width, &height, &nrChannels, 0);
	hetman.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&hetman);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/Goniec.png", &width, &height, &nrChannels, 0);
	goniec.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&goniec);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/Skoczek.png", &width, &height, &nrChannels, 0);
	skoczek.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&skoczek);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/Krol.png", &width, &height, &nrChannels, 0);
	krol.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
	chess.push_back(&krol);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("data/chessBoard.png", &width, &height, &nrChannels, 0);
	chessBoard.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);

	do {
		window->clear();
		chessBoard.draw();
		for (Triangles * t : chess)
		{
			t->draw();
		}

		window->swapBuffers();
	} while (!(window->isPressed(GLFW_KEY_ESCAPE)) && !(window->shouldClose()));
	window->close();

	return 0;
}