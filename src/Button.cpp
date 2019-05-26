#include "Button.h"

#include "stb_image.h"

Button::Button(const std::string & text, 
	GLfloat xx, 
	GLfloat yy, 
	GLfloat width, 
	GLfloat height,
	Window * parentWindow,
	bool * flagToSetOnRelease) :
	shape_(width, height, xx, yy, 0.0f),
	shape_pressed_(width, height, xx, yy, 0.0f),
	icon_(0.6 * height, xx, yy),
	pressed_(false),
	isIcon_(false),
	prevMouseOver_(false),
	parentWindow_(parentWindow),
	flagToSetOnRelease_(flagToSetOnRelease)
{
	handle_events(this);

	if (text.size() > 0)
	{
		text_.push_back(new Text(text));
		text_[0]->setLocation(glm::vec2(xx - 0.4*width, yy));
		text_[0]->generateShaders();
		text_[0]->loadFont("fonts/tt0351m_.ttf", 0.2*height);
	}

	load_shape_texture(shape_, "data/button.png");
	load_shape_texture(shape_pressed_, "data/pressed_button.png");
}

void Button::load_icon(const std::string & path)
{
	load_shape_texture(icon_, path);
	isIcon_ = true;
}

void Button::draw()
{
	if (parentWindow_ != NULL)
	{
		glm::vec2 pos = parentWindow_->getCursorPos();
		if (shape_.contains(pos.x, pos.y))
		{
			if (!prevMouseOver_)
			{
				prevMouseOver_ = true;
				parentWindow_->set_cursor(GLFW_HAND_CURSOR);
			}
		}
		else
		{
			if (prevMouseOver_)
				parentWindow_->set_cursor(NULL);
			prevMouseOver_ = false;
		}
	}
	if (pressed_)
		shape_pressed_.draw();
	else
		shape_.draw();

	for (Text * t : text_)
		t->draw();

	if (isIcon_)
		icon_.draw();
}

bool Button::is_pressed()
{
	return pressed_;
}

/////////////////////////////////////////////// Events
void Button::mouse_left_pressed(GLfloat xx, GLfloat yy)
{
	if (shape_.contains(xx, yy))
		if (!pressed_)
		{
			pressed_ = true;
		}
}

void Button::mouse_dragged(GLfloat xx, GLfloat yy)
{
	pressed_ = shape_.contains(xx, yy);
}

void Button::mouse_left_released(GLfloat xx, GLfloat yy)
{
	if (shape_.contains(xx, yy) && pressed_)
		*flagToSetOnRelease_ = true;
	pressed_ = false;

}