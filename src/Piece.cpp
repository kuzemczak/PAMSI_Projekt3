#include "Piece.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string Piece::textureDir = "";

Piece::Piece(PieceType type, Team team, std::string name) :
	shape_(75, glm::vec2(0.0f, 0.0f)),
	name_(name),
	boardPosition_(0),
	team_(team),
	type_(type),
	moveCntr_(0),
	captured_(false)
{
}

void Piece::set_init_position(int pos)
{
	boardPosition_ = pos;
}

int Piece::gen_move(int from, int to, int specialBits)
{
	return (from << 0) | (to << 6) | specialBits;
}

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
	boardPosition_ = boardPosition[1] * 8 + boardPosition[0];
	moveCntr_++;
}

void Piece::move_board(GLuint xx, GLuint yy)
{
	boardPosition_ = yy * 8 + xx;
	moveCntr_++;
}

void Piece::move_board(int pos)
{
	boardPosition_ = pos;
	moveCntr_++;
}

void Piece::undo_move(int pos)
{
	boardPosition_ = pos;
	moveCntr_--;
}

void Piece::set_captured(bool captured)
{
	captured_ = captured;
}

bool Piece::is_captured()
{
	return captured_;
}

int Piece::get_board_position()
{
	return boardPosition_;
}

std::vector<int> Piece::get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory)
{
	return moves_;
}

bool Piece::shape_contains(GLfloat xx, GLfloat yy)
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

Team Piece::get_team()
{
	return team_;
}

PieceType Piece::get_type()
{
	return type_;
}

std::string Piece::get_name()
{
	return name_;
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

void Piece::set_texture_dir(const std::string & name)
{
	textureDir = name;
}

void Piece::load_shape_texture(const std::string & path)
{
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	shape_.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);
}

void Piece::draw()
{
	shape_.draw();
}
//////////////////////////////////// Events
//void Piece::mouse_dragged(GLfloat xx, GLfloat yy)
//{
//	if (shape_.contains(xx, yy))
//	{
//		shape_.setLocation(glm::vec2(xx, yy));
//	}
//}