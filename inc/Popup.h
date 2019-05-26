#ifndef POPUP_H
#define POPUP_H

#include "Text.h"
#include "Rect.h"

class Popup : public GraphicalGameObjectInterface
{
	bool visible_,
		show_icon_;
	Rect * background_,
		*icon_;
	Text * text_;

public:
	Popup(GLfloat xx, GLfloat yy,
		GLfloat width, GLfloat height,
		const std::string & text = "");

	void show();
	void hide();
	void draw();

	void set_text(const std::string & text);
	void load_icon(const std::string & path);
};

#endif