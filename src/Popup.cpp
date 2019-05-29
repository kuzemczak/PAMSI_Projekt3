#include "Popup.h"

Popup::Popup(GLfloat xx, GLfloat yy,
	GLfloat width, GLfloat height,
	const std::string & text) :

	visible_(false),
	show_icon_(false)
{
	background_ = new Rect(width, height, xx, yy, 0.0f);
	load_shape_texture(*background_, "data/popup_bg.png");

	text_ = new Text(text);
	text_->setLocation(glm::vec2(xx - 0.5 * width + height*0.7, yy));
	text_->generateShaders();
	text_->loadFont("fonts/segoeui.ttf", 0.1*height);

	icon_ = new Rect(0.4*height, xx - width/2 + 0.45*height, yy);
}

void Popup::show()
{
	visible_ = true;
}

void Popup::hide()
{
	visible_ = false;
}

void Popup::draw()
{
	if (!visible_)
		return;

	background_->draw();
	text_->draw();
	
	if (show_icon_)
		icon_->draw();
}

void Popup::set_text(const std::string & text)
{
	text_->setText(text);
}

void Popup::load_icon(const std::string & path)
{
	load_shape_texture(*icon_, path);
	show_icon_ = true;
}