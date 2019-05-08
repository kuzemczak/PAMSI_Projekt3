#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
	GLFWwindow * window;
public:
	Window();

	int init(int w, int h, const char * name);
	void enableKeyboard();
	void disableKeyboard();
	void enableCursor();
	void hideCursor();
	void disableCursor();
	void enableMouseKeys();
	void disableMouseKeys();

	int height();
	int width();
	GLFWwindow * getGLFWWindowPtr();

	glm::vec2 getCursorPos();

	void setCursorPos(int w, int h);
	void setBgcolor(float r, float g, float b, float a);
	void clear();
	void swapBuffers();
	bool isPressed(int key);
	bool isMousePressed(int key);
	bool shouldClose();

	void close();
	

	~Window();
};

