#ifndef BUTTON_H
#define BUTTON_H

#include "Events.h"
#include "Rect.h"
#include "Text.h"
#include "Window.h"

class Button : Events, public GraphicalGameObjectInterface
{
	Rect shape_;
	Rect shape_pressed_;
	Rect icon_;
	std::vector<Text*> text_;
	bool pressed_,
		isIcon_,
		prevMouseOver_;
	Window * parentWindow_;

	bool* flagToSetOnRelease_;

public:
	Button(const std::string & text, 
		GLfloat xx, 
		GLfloat yy, 
		GLfloat width, 
		GLfloat height, 
		Window * parentWindow = NULL,
		bool * flagToSetOnRelease = NULL);

	void load_icon(const std::string & path);
	void draw();
	bool is_pressed();

	////////////////////////////////////////////// Events
	void mouse_left_pressed(GLfloat xx, GLfloat yy);
	void mouse_dragged(GLfloat xx, GLfloat yy);
	void mouse_left_released(GLfloat xx, GLfloat yy);
};

#endif