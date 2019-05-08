#include <iostream>

#include "Window.h"

Window::Window()
{
	
}


Window::~Window()
{
}

int Window::init(int w, int h, const char * name)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		getchar();
		return -1;
	}

	
	//glfwWindowHint(GLFW_DECORATED, 0); // ramka i przyciski
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(w, h, name, NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version." << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}
	glewInit();
	glfwMakeContextCurrent(window);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

int Window::height()
{
	int h, w;
	glfwGetWindowSize(window, &w, &h);
	return h;
}

int Window::width()
{
	int h, w;
	glfwGetWindowSize(window, &w, &h);
	return w;
}

void Window::enableKeyboard()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Window::disableKeyboard()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
}

void Window::enableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::hideCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Window::disableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::enableMouseKeys()
{
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
}

void Window::disableMouseKeys()
{
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);
}

void Window::setCursorPos(int w, int h)
{
	glfwPollEvents();
	glfwSetCursorPos(window, w, h);
}

void Window::setBgcolor(float r, float g, float b, float a)
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::isPressed(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Window::isMousePressed(int key)
{
	if (glfwGetMouseButton(window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(window) != 0;
}

void Window::close()
{
	glfwTerminate();
}

GLFWwindow * Window::getGLFWWindowPtr()
{
	return window;
}

glm::vec2 Window::getCursorPos()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// old version
	/*xpos = ((2 * xpos / width()) - 1)*width() / height();
	ypos = (2 * (height() - ypos) / height()) - 1;
	return glm::vec2((float)xpos, (float)ypos);*/

	// new version
	return glm::vec2((float)xpos, (float)height()-ypos);
}