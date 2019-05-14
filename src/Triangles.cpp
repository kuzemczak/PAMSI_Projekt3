#include <string>

#include "Triangles.h"

GLuint Triangles::isTextureUniformID = 0;

Triangles::Triangles()
{
	transformationMatrix = glm::mat4(1.0f);
	vertexBufferSize = 1024;
	newVertices = false;
	newLastPoint = false;
	newColors = false;
	isTexture = 0;
	newTextureCoords = false;
	unsigned char tex11[3] = { 0, 0, 0 };
	generateTexture(tex11, 1, 1, 3);
}


Triangles::~Triangles()
{
	if (vertices.size() == colors.size())
		glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	if(objectCntr == 1)
		glDeleteProgram(programID);
}

void Triangles::generateShaders()
{
	if (programID == 0)
	{
		std::ofstream vertexShader, fragmentShader;
		vertexShaderFileName.append("object");
		vertexShaderFileName.append(std::to_string(objectNumber));
		vertexShaderFileName.append("VertexShader.vertexshader");
		fragmentShaderFileName.append("object");
		fragmentShaderFileName.append(std::to_string(objectNumber));
		fragmentShaderFileName.append("FragmentShader.fragmentshader");

		vertexShader.open(vertexShaderFileName);
		vertexShader
			<< "#version 330 core\n\n"
			"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
			"layout(location = 1) in vec4 vertexColor;\n"
			"layout(location = 2) in vec2 inTexCoord;\n\n"
			"out vec4 fragmentColor;\n"
			"out vec2 texCoord;\n\n"
			"uniform mat4 transformationMatrix;\n\n"
			"void main() {\n"
			"gl_Position =  transformationMatrix * vec4(vertexPosition_modelspace,1);\n"
			"fragmentColor = vertexColor;\n"
			"texCoord = inTexCoord;"
			"}";
		vertexShader.close();

		fragmentShader.open(fragmentShaderFileName);
		fragmentShader
			<< "#version 330 core\n\n"
			"in vec4 fragmentColor;\n"
			"in vec2 texCoord;"
			"out vec4 color;\n"
			"uniform sampler2D uniTexture;\n"
			"uniform uint isTexture;\n"
			"const uint zero = 0;"
			"void main() {\n"
			"if (isTexture == zero) {\n"
			"color = fragmentColor;\n"
			"} else {\n"
			"color = texture(uniTexture, texCoord);\n"
			"}}";
		fragmentShader.close();


		programID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

		remove(vertexShaderFileName.c_str());
		remove(fragmentShaderFileName.c_str());

		transformationMatrixID = glGetUniformLocation(programID, "transformationMatrix");
		isTextureUniformID = glGetUniformLocation(programID, "isTexture");
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &textureCoordsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), textureCoords.data(), GL_STATIC_DRAW);
}

void Triangles::draw()
{
	if (visible)
	{
		// Use our shader
		glUseProgram(programID);

		glm::mat4 mvp = orthoMatrix * transformationMatrix;

		glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniform1ui(isTextureUniformID, isTexture);

		if (newColors)
		{
			GLuint offset = 0,
				size = static_cast<GLuint>(colors.size()) * sizeof(colors[0]);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, colors.data());
		}

		if (newTextureCoords)
		{
			GLuint offset = 0,
				size = static_cast<GLuint>(textureCoords.size()) * sizeof(textureCoords[0]);
			glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, textureCoords.data());
		}

		glEnableVertexAttribArray(0);
		// 1rst attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : texture
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(drawingMode, 0, static_cast<GLuint>(vertices.size()) / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

void Triangles::generateTexture(unsigned char * textureData, GLuint width, GLuint height, GLuint channels)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (textureData)
	{
		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		else
			std::cout << "Error: bad channels value: " << channels << std::endl;
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Bad texture data" << std::endl;
	}
	isTexture = 1;
}