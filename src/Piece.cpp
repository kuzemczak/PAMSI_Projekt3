#include "Piece.h"


void Piece::move_screen(glm::vec2 boardCoords)
{
	shape_.setLocation(glm::vec3(boardCoords, 0.0f));
}

void Piece::move_screen(GLfloat xx, GLfloat yy)
{
	shape_.setLocation(glm::vec3(xx, yy, 0.0f));
}

void Piece::move_board(glm::i8vec2 boardPosition)
{
	boardPosition_ = boardPosition;
	moveCntr_++;
}

void Piece::move_board(GLuint xx, GLuint yy)
{
	boardPosition_ = glm::i8vec2(xx, yy);
	moveCntr_++;
}

glm::i8vec2 & Piece::get_board_position()
{
	return boardPosition_;
}

std::vector<glm::i8vec2> & Piece::get_moves()
{
	return moves_;
}

bool Piece::contains(GLfloat xx, GLfloat yy)
{
	return shape_.contains(xx, yy);
}

int Piece::get_strength()
{
	return strength_;
}

GLuint Piece::get_move_count()
{
	return moveCntr_;
}

void Piece::set_shape_texture(
	unsigned char * textureData,
	GLuint width,
	GLuint height,
	GLuint channels
)
{
	shape_.generateTexture(textureData, width, height, channels);
}