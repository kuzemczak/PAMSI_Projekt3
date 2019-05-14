#include "Rect.h"

LocMode Rect::locationMode = TOP_LEFT;



Rect::Rect(GLfloat w, GLfloat h, GLfloat locx, GLfloat locy, GLfloat rot, glm::vec4 clr)
{
	switch (locationMode)
	{
	case TOP_LEFT:
	{
		float tab[12] = {
			0.0f, 0.0f, 0.0f,
			0.0f, -h, 0.0f,
			w, -h, 0.0f,
			w, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case TOP_RIGHT:
	{
		float tab[12] = {
			-w, 0.0f, 0.0f,
			-w, -h, 0.0f,
			0.0f, -h, 0.0f,
			0.0f, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_LEFT:
	{
		float tab[12] = {
			0.0f, h, 0.0f,
			0.0f, 0.0f, 0.0f,
			w, 0.0f, 0.0f,
			w, h, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_RIGHT:
	{
		float tab[12] = {
			-w, h, 0.0f,
			-w, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, h, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case CENTER:
	{
		float tab[12] = {
			-w / 2, h / 2, 0.0f,
			-w / 2, -h / 2, 0.0f,
			w / 2, -h / 2, 0.0f,
			w / 2, h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case TOP_CENTER:
	{
		float tab[12] = {
			-w / 2, 0.0f, 0.0f,
			-w / 2, -h, 0.0f,
			w / 2, -h, 0.0f,
			w / 2, 0.0f, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case BOTTOM_CENTER:
	{
		float tab[12] = {
			-w / 2, h, 0.0f,
			-w / 2, 0.0f, 0.0f,
			w / 2, 0.0f, 0.0f,
			w / 2, h, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case LEFT_CENTER:
	{
		float tab[12] = {
			0.0f, h / 2, 0.0f,
			0.0f, -h / 2, 0.0f,
			w, -h / 2, 0.0f,
			w, h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	case RIGHT_CENTER:
	{
		float tab[12] = {
			-w, h / 2, 0.0f,
			-w, -h / 2, 0.0f,
			0.0f, -h / 2, 0.0f,
			0.0f, h / 2, 0.0f
		};
		for (int i = 0; i < 12; i++)
			vertices.push_back(tab[i]);
	}
		break;
	}

	
	color[0] = clr[0];
	color[1] = clr[1];
	color[2] = clr[2];
	color[3] = clr[3];
	for (int i = 0; i < 4; i++)
	{
		colors.push_back(color[0]);
		colors.push_back(color[1]);
		colors.push_back(color[2]);
		colors.push_back(color[3]);
	}

	textureCoords.push_back(0.0f);
	textureCoords.push_back(1.0f);
	textureCoords.push_back(0.0f);
	textureCoords.push_back(0.0f);
	textureCoords.push_back(1.0f);
	textureCoords.push_back(0.0f);
	textureCoords.push_back(1.0f);
	textureCoords.push_back(1.0f);

	width = w;
	height = h;
	setLocation(glm::vec2(locx, locy));

	rotate(rot);

	generateShaders();

	setDrawingMode(GL_TRIANGLE_FAN);
}

Rect::~Rect()
{
}

void Rect::setLocationMode(LocMode mode)
{
	locationMode = mode;
}

bool Rect::contains(GLfloat xx, GLfloat yy)
{
	if (xx >= vertices[3] + transformationMatrix[3][0] &&
		xx <= vertices[9] + transformationMatrix[3][0] &&
		yy >= vertices[4] + transformationMatrix[3][1] &&
		yy <= vertices[10] + transformationMatrix[3][1])
		return true;
	return false;
}