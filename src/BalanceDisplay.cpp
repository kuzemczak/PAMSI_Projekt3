#include "BalanceDisplay.h"

#include <string>

#include "stb_image.h"

BalanceDisplay::BalanceDisplay(GLfloat xx,
	GLfloat yy,
	GLfloat width,
	GLfloat height,
	int initialVal,
	int min,
	int max,
	glm::vec4 leftColor, 
	glm::vec4 rightColor) :

	valueText_(std::to_string(initialVal)),
	xx_(xx),
	yy_(yy),
	width_(width),
	height_(height),
	value_(initialVal),
	min_(min),
	max_(max),
	leftColor_(leftColor),
	rightColor_(rightColor)
{
	valueText_.setLocation(glm::vec2(xx - width/2, yy - height/2 - 22));
	valueText_.generateShaders();
	valueText_.loadFont("fonts/arial.ttf", 20);

	left_ = NULL;
	right_ = NULL;

	update_rects();
}

void BalanceDisplay::draw()
{
	left_->draw();
	right_->draw();
	valueText_.draw();
}

void BalanceDisplay::set_value(int val)
{
	value_ = val;
	valueText_.setText(std::to_string(val));
	update_rects();
}

void BalanceDisplay::update_rects()
{
	if (left_ != NULL)
		delete left_;
	if (right_ != NULL)
		delete right_;

	double mianownik = max_ - min_,
		licznik = value_ - min_;
	double lw = width_ * licznik / mianownik,
		rw = width_ * (mianownik - licznik) / mianownik;

	double lxx = xx_ - width_ / 2 + lw / 2;
	double rxx = lxx + lw / 2 + rw / 2;

	left_ = new Rect(lw, height_, lxx, yy_, 0.0f);
	unsigned char ch0[3] = { leftColor_[0] * 255, leftColor_[1] * 255 , leftColor_[2] * 255 };
	left_->generateTexture(ch0, 1, 1, 3);

	right_= new Rect(rw, height_, rxx, yy_, 0.0f, rightColor_);
	unsigned char ch1[3] = { rightColor_[0] * 255, rightColor_[1] * 255 , rightColor_[2] * 255 };
	right_->generateTexture(ch1, 1, 1, 3);
}
