#ifndef CLOCK_H
#define CLOCK_H

#include "vectorUtils.h"
#include "Text.h"
#include "Rect.h"

class Clock : public GraphicalGameObjectInterface
{
	Text text_;
	Rect background_;
	unsigned int millis_,
		seconds_,
		minutes_;

	std::chrono::time_point<std::chrono::steady_clock> prevTimeStamp_;
public:
	Clock(GLfloat x, GLfloat y,
		GLfloat width, GLfloat height,
		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	void update();
	void draw();
	void reset();
};


#endif