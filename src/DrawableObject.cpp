#include "DrawableObject.h"

unsigned int DrawableObject::objectCntr = 0;
unsigned int DrawableObject::everCreatedObjectCntr = 0;
GLuint DrawableObject::VertexArrayID;
GLuint DrawableObject::programID = 0;
GLuint DrawableObject::transformationMatrixID = 0;
glm::mat4 DrawableObject::orthoMatrix = glm::mat4(1.0f);

DrawableObject::DrawableObject()
{
	if (objectCntr == 0)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
	}

	objectCntr++;
	everCreatedObjectCntr++;
	objectNumber = everCreatedObjectCntr;
	visible = true;
	drawingMode = GL_POINTS;
}


DrawableObject::~DrawableObject()
{
	if (objectCntr == 1)
	{
		glDeleteVertexArrays(1, &VertexArrayID);
	}
	objectCntr--;
}

void DrawableObject::addIndex(unsigned int i)
{
	indices.push_back(i);
}

void DrawableObject::addVertix(GLfloat v)
{
	vertices.push_back(v);
}

void DrawableObject::addColor(GLfloat c)
{
	colors.push_back(c);
}

std::vector<unsigned int> * DrawableObject::indicesVectorPtr()
{
	return &indices;
}

std::vector<GLfloat> * DrawableObject::verticesVectorPtr()
{
	return &vertices;
}

std::vector<GLfloat> * DrawableObject::colorsVectorPtr()
{
	return &colors;
}

void DrawableObject::setDrawingMode(int mode)
{
	drawingMode = mode;
}

void DrawableObject::setAspectRatio(GLfloat w, GLfloat h)
{
	orthoMatrix = glm::ortho(0.0f, w, 0.0f, h); // new version

	// old version
	/*GLfloat aspect = w / h;
	orthoMatrix = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
	orthoMatrix = glm::ortho(0.0f, w, 0.0f, h);*/
}

void DrawableObject::show()
{
	visible = true;
}

void DrawableObject::hide()
{
	visible = false;
}

void DrawableObject::rotate(float angle)
{
	transformationMatrix[0][0] = cos(angle);
	transformationMatrix[1][0] = -sin(angle);
	transformationMatrix[0][1] = sin(angle);
	transformationMatrix[1][1] = transformationMatrix[0][0];
}

void DrawableObject::move(glm::vec2 vec)
{
	transformationMatrix[3][0] += vec[0];
	transformationMatrix[3][1] += vec[1];
}

void DrawableObject::move(glm::vec3 vec)
{
	transformationMatrix[3][0] += vec[0];
	transformationMatrix[3][1] += vec[1];
	transformationMatrix[3][2] += vec[2];
}

void DrawableObject::setLocation(glm::vec2 point)
{
	transformationMatrix[3][0] = point[0];
	transformationMatrix[3][1] = point[1];
}

void DrawableObject::setLocation(glm::vec3 point)
{
	transformationMatrix[3][0] = point[0];
	transformationMatrix[3][1] = point[1];
	transformationMatrix[3][2] = point[2];
}