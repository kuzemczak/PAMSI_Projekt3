#ifndef PIECE_H
#define PIECE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Rect.h"
#include "Events.h"

enum Team { BLACK = -1, WHITE = 1};

class Piece : Events
{
protected:
	static std::string textureDir;

	std::vector<glm::i8vec2> moves_;
	glm::i8vec2 boardPosition_;
	std::string name_;
	Rect shape_;
	Team team_;
	int strength_;
	GLuint moveCntr_;


public:
	Piece(Team team, std::string name = "");
	~Piece() {}

	static void set_texture_dir(const std::string & name);

	void move_screen(glm::vec2 screenPosition);
	void move_screen(GLfloat xx, GLfloat yy);
	virtual void move_board(glm::i8vec2 boardPosition);
	virtual void move_board(GLuint xx, GLuint yy);

	std::vector<glm::i8vec2> & get_moves();
	bool contains(GLfloat xx, GLfloat yy);
	glm::i8vec2 & get_board_position();
	int get_strength();
	GLuint get_move_count();

	void set_shape_texture(
		unsigned char * textureData, 
		GLuint width, 
		GLuint height, 
		GLuint channels
	);

	void load_shape_texture(const std::string & path);
	
	void draw();


	///////////////////////// Events
	void mouse_dragged(GLfloat xx, GLfloat yy);
};

class King : public Piece
{
public:
	King(Team team);
	~King() {}

	void move_board(glm::i8vec2 boardPosition);
	void move_board(GLuint xx, GLuint yy);
};

class Queen : public Piece
{
public:
	Queen(Team team);
	~Queen() {}
};
#endif 
