#include "Text.h"

GLuint Text::programID = 0;
GLuint Text::transformationMatrixID = 0;
GLuint Text::textColorUniformID = 0;

Text::Text(std::string text, glm::vec4 color)
{
	this->text = text;
	this->color = color;
}


Text::~Text()
{
}

void Text::loadFont(std::string name, GLuint size)
{
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, name.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		GLuint advance = 0;
		if (c == ' ')
			advance = (face->glyph->advance.x) >> 6;
		else
			advance = face->glyph->bitmap_left + face->glyph->bitmap.width;
		Character character = {
			texture,
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			advance
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text::generateShaders()
{
	if (programID == 0)
	{
		std::ofstream vertexShader, fragmentShader;
		vertexShaderFileName.append("VertexShader.vertexshader");
		fragmentShaderFileName.append("FragmentShader.fragmentshader");

		vertexShader.open(vertexShaderFileName);
		vertexShader
			<< "#version 330 core\n\n"
			<< "layout(location = 0) in vec4 vertex;\n"
			<< "out vec2 TexCoords;\n\n"
			<< "uniform mat4 transformationMatrix;\n\n"
			<< "void main() {\n"
			<< "\tgl_Position =  transformationMatrix * vec4(vertex.xy, 0.0, 1.0);\n"
			<< "\tTexCoords = vertex.zw;\n"
			<< "}";
		vertexShader.close();

		fragmentShader.open(fragmentShaderFileName);
		fragmentShader
			<< "#version 330 core\n\n"
			<< "in vec2 TexCoords;\n"
			<< "out vec4 color;\n\n"
			<< "uniform sampler2D text;\n"
			<< "uniform vec3 textColor;\n\n"
			<< "void main() {\n"
			<< "\tvec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
			<< "\tcolor = vec4(textColor, 1.0) * sampled;\n"
			<< "}";
		fragmentShader.close();


		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

		remove(vertexShaderFileName.c_str());
		remove(fragmentShaderFileName.c_str());

		transformationMatrixID = glGetUniformLocation(programID, "transformationMatrix");
		textColorUniformID = glGetUniformLocation(programID, "textColor");
	}

	//glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);
	//glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void Text::draw()
{
	// Activate corresponding render state	
	glUseProgram(programID);
	glUniform3f(textColorUniformID, color.x, color.y, color.z);
	glm::mat4 mvp = orthoMatrix * transformationMatrix;
	glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, &mvp[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//glBindVertexArray(vertexArray);

	GLfloat xpos = 0;
	GLfloat ypos = 0;

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		xpos += /*x + */(float)ch.Bearing.x;
		ypos = /*y - */-(float)((float)ch.Size.y - (float)ch.Bearing.y);

		GLfloat w = (float)ch.Size.x;
		GLfloat h = (float)ch.Size.y;

		// Update VBO for each character
		std::vector<GLfloat> vertices = {
			xpos,     ypos + h,   0.0, 0.0,
			xpos,     ypos,       0.0, 1.0,
			xpos + w, ypos,       1.0, 1.0,
			xpos,     ypos + h,   0.0, 0.0,
			xpos + w, ypos,       1.0, 1.0,
			xpos + w, ypos + h,   1.0, 0.0
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		xpos += (ch.Advance); // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	//glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::setText(std::string text)
{
	this->text = text;
}
