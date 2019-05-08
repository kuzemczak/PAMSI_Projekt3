#ifndef TEXT_H
#define TEXT_H

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H  

#include "DrawableObject.h"

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::vec2 Size;       // Size of glyph
	glm::vec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class Text : public DrawableObject
{
	static GLuint programID;
	static GLuint transformationMatrixID;
	static GLuint textColorUniformID;

	GLuint vertexArray;

	std::string text;
	glm::vec4 color;

	bool fontLoaded;
	GLuint size;

	FT_Library ft;
	FT_Face face;

	std::map<char, Character> Characters;

public:
	Text(std::string text = "", glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	~Text();

	void loadFont(std::string name, GLuint size);
	void generateShaders();
	void draw();
	void setText(std::string text);
};

#endif