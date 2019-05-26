#include "Clock.h"

Clock::Clock(GLfloat x, GLfloat y, 
	GLfloat width, GLfloat height,
	glm::vec4 color) :

	text_("00:00", color),
	millis_(0),
	background_(width, height, x + width/2, y + height/2, 0.0f)
{
	text_.setLocation(glm::vec2(x + 0.04 * width, y + 0.25 * height));
	text_.generateShaders();
	text_.loadFont("fonts/tt0351m_.ttf", 0.8*height);

	prevTimeStamp_ = Clock_t::now();
	load_shape_texture(background_, "data/clock_bg.png");
}

void Clock::update()
{
	auto stamp = Clock_t::now();
	millis_ += static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(stamp - prevTimeStamp_).count());
	prevTimeStamp_ = stamp;

	seconds_ = ((millis_ - (millis_ % 1000))/1000)%60;
	minutes_ = (millis_ - (millis_ % 60000))/60000;
	std::string s = "";
	if (minutes_ < 10)
		s.append("0");
	s.append(std::to_string(minutes_));
	s.append(":");
	if (seconds_ < 10)
		s.append("0");
	s.append(std::to_string(seconds_));
	text_.setText(s);
}

void Clock::draw()
{
	background_.draw();
	text_.draw();
}

void Clock::reset()
{
	millis_ = 0;
}