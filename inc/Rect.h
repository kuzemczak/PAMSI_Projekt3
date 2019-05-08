#ifndef RECT_H
#define RECT_H
#include "Triangles.h"

enum LocMode
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER,
	TOP_CENTER,
	BOTTOM_CENTER,
	LEFT_CENTER,
	RIGHT_CENTER
};

class Rect : public Triangles
{
	static LocMode locationMode;

	GLfloat width, height, rotation;
	glm::vec4 color;

public:
	Rect(GLfloat sL, glm::vec2 loc, GLfloat rot = 0, glm::vec4 clr = glm::vec4(1.0f,0.0f,0.0f,1.0f)) :
		Rect(sL, sL, loc[0], loc[1], rot, clr) {}
	Rect(GLfloat sL, GLfloat locx, GLfloat locy, GLfloat rot = 0, glm::vec4 clr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) :
		Rect(sL, sL, locx, locy, rot, clr) {}
	Rect(GLfloat w, GLfloat h, glm::vec2 loc, GLfloat rot = 0, glm::vec4 clr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) :
		Rect(w, h, loc[0], loc[1], rot, clr) {}
	Rect(GLfloat w, GLfloat h, GLfloat locx, GLfloat locy, GLfloat rot = 0, glm::vec4 clr = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	static void setLocationMode(LocMode mode);
	~Rect();

	bool contains(GLfloat xx, GLfloat yy);
};

#endif