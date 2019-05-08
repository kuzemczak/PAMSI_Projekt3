#ifndef PIECE_H
#define PIECE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Rect.h"

enum Team { BLACK, WHITE };

class Piece
{
protected:
	std::vector<glm::i8vec2> moves_;
	glm::i8vec2 boardPosition_;
	std::string name_;
	Rect shape_;
	Team team_;

public:
	Piece(Team team, std::string name = "") : 
		shape_(1, glm::vec2(0.0f, 0.0f)),
		name_(name),
		boardPosition_(0, 0),
		team_(team)
	{}
	~Piece() {}

	void move_screen(glm::vec2 screenPosition);
	void move_screen(GLfloat xx, GLfloat yy);
	void move_board(glm::i8vec2 boardPosition);
	void move_board(GLuint xx, GLuint yy);

	std::vector<glm::i8vec2> & get_moves();
	bool contains(GLfloat xx, GLfloat yy);
	glm::i8vec2 & get_board_position();
};

class King : public Piece
{
public:
	King(Team team);
	~King() {}
};

#endif 
