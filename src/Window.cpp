#include <iostream>

#include "Window.h"

Window::Window() :
	bgColor(1.0f, 1.0f, 1.0f, 0.0f)
{
	mouseLeftPrevState_ = GLFW_RELEASE;
	mousePrevPosition_ = glm::vec2(0.0f, 0.0f);
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

	// disable window resizing
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
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

void Window::set_cursor(int shape)
{
	if (cursor != NULL)
	{
		glfwDestroyCursor(cursor);
	}

	cursor = glfwCreateStandardCursor(shape);
	glfwSetCursor(window, cursor);
}

void Window::setCursorPos(int w, int h)
{
	glfwPollEvents();
	glfwSetCursorPos(window, w, h);
}

void Window::setBgcolor(float r, float g, float b, float a)
{
	bgColor = glm::vec4(r, g, b, a);
}

void Window::clear()
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
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
	if (cursor != NULL)
	{
		glfwDestroyCursor(cursor);
	}
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

void Window::process_events()
{
	if (mouseLeftPrevState_ == GLFW_RELEASE && isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		mousePrevPosition_ = getCursorPos();
		Events::emit_mouse_left_pressed(mousePrevPosition_[0], mousePrevPosition_[1]);
		mouseLeftPrevState_ = GLFW_PRESS;
	}

	if (mouseLeftPrevState_ == GLFW_PRESS && !isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		Events::emit_mouse_left_released(mousePrevPosition_[0], mousePrevPosition_[1]);
		mouseLeftPrevState_ = GLFW_RELEASE;
	}

	if (mouseLeftPrevState_ == GLFW_PRESS && isMousePressed(GLFW_MOUSE_BUTTON_LEFT) &&
		glm::length(getCursorPos() - mousePrevPosition_) >= 1.0f)
	{
		mousePrevPosition_ = getCursorPos();
		Events::emit_mouse_dragged(mousePrevPosition_[0], mousePrevPosition_[1]);
	}
}