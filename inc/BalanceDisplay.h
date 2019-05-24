#ifndef BALANCEDISPLAY_H
#define BALANCEDISPLAY_H

#include "Rect.h"
#include "Text.h"

class BalanceDisplay
{
	Rect * left_;
	Rect * right_;
	glm::vec4 leftColor_, rightColor_;
	Text valueText_;
	int value_, max_, min_;
	GLfloat xx_, yy_, width_, height_;

public:
	BalanceDisplay(GLfloat xx,
		GLfloat yy,
		GLfloat width,
		GLfloat height,
		int initialVal, 
		int min, 
		int max, 
		glm::vec4 leftColor = glm::vec4(1.0f,1.0f,1.0f,0.0f),
		glm::vec4 rightColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	void draw();
	void set_value(int val);
	void update_rects();
};

#endif