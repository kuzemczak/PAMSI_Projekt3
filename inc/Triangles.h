#ifndef TRIANGLES_H
#define TRIANGLES_H
#include "DrawableObject.h"
class Triangles : public DrawableObject
{
protected:
	static GLuint isTextureUniformID;

	int newVertices, newLastPoint, newColors;
	std::vector<GLfloat> textureCoords;
	GLuint textureCoordsBuffer;
	GLuint isTexture;
	GLuint texture;

	bool newTextureCoords;

public:
	Triangles();
	~Triangles();

	void generateShaders();
	void draw();
	void generateTexture(
		unsigned char * textureData, 
		GLuint width, 
		GLuint height, 
		GLuint channels
	);
};

void load_shape_texture(Triangles & shape, const std::string & path);
#endif
