/* Move encryption:
** (0 to 64) | (0 to 63) << 6 | (0 to 31) << 12 | (powers of 2) << 17
** `---v---'   `------v------'  `-------v------'  `---------v--------'
**   from            to              captured            special
**/
#define GET_FROM(i) ((i) & 63)							// decodes destination part of encoded move
#define GET_TO(i) (((i) >> 6) & 63)					// decodes origin part of encoded move
#define GET_FROM_X(i) (((i) & 7)
#define GET_FROM_Y(i) (((i) >> 3) & 7)
#define GET_TO_X(i) (((i) >> 6) & 7)
#define GET_TO_Y(i) (((i) >> 9) & 7)
#define GET_CAPTURED(i) (((i) >> 12) & 31)	// decodes a number of captured piece part of encoded move
#define MAKE_CAPTURED(i) ((i) << 12)				// encodes a number of captured piece part of a move
#define ENCODE(from,to,capt,spec) ((from) | ((to) << 6) | ((capt) << 12) | (spec))	// encodes given values in an integer

// Special bits:
#define QUIET_MOVE (0 << 17)						// quiet move
#define KING_CASTLE (1 << 17)						// king castle
#define QUEEN_CASTLE (1 << 18)					// queen castle
#define CAPTURE (1 << 19)								// capture
#define EP_CAPTURE (1 << 20)						// ep-capture
#define N_PROMO (1 << 21)							// knight-promotion
#define B_PROMO (1 << 22)							// bishop-promotion
#define R_PROMO (1 << 23)							// rook-promotion
#define Q_PROMO (1 << 24)							// queen-promotion
#define RAY_MOVE (1 << 25)						// ray move
#define NEGATIVE_MOVE (1 << 26)				// negative (opposite direction)
#define PROMOTION (1 << 27)					// general Pawn promotion
#define DOUBLE_PAWN_PUSH  (1 << 28)	// double pawn push
#define PAWN_CAPTURE (1 << 29)				// pawn diagonal capturing move
#define PAWN_PUSH (1 << 30)					// pawn push
// jeszcze jeden wolny bit



#ifndef PIECE_H
#define PIECE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Rect.h"
#include "Events.h"
#include "Team.h"


enum PieceType
{
	NONE = 0,
	PAWN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6
};

class Piece : Events
{
protected:
	static std::string textureDir;

	std::vector<int> moves_;
	int boardPosition_;
	std::string name_;
	Rect shape_;
	Team team_;
	PieceType type_;
	int strength_;
	GLuint moveCntr_;
	bool captured_;

	int gen_move(int from, int to, int specialBits);
	
public:
	Piece(PieceType type, Team team, std::string name = "");
	~Piece() {}

	static void set_texture_dir(const std::string & name);

	void set_init_position(int pos);

	void move_screen(glm::vec2 screenPosition);
	void move_screen(GLfloat xx, GLfloat yy);
	void move_board(glm::i8vec2 boardPosition);
	void move_board(GLuint xx, GLuint yy);
	void move_board(int pos);
	void undo_move(int pos);
	void set_captured(bool captured);

	bool is_captured();
	virtual std::vector<int>  get_moves(
		const std::vector<Piece*> & board, 
		const std::vector<int> & moveHistory);
	bool shape_contains(GLfloat xx, GLfloat yy);
	int get_board_position();
	int get_strength();
	GLuint get_move_count();
	Team get_team();
	PieceType get_type();
	std::string get_name();

	void set_shape_texture(
		unsigned char * textureData, 
		GLuint width, 
		GLuint height, 
		GLuint channels
	);

	void load_shape_texture(const std::string & path);
	
	void draw();


	///////////////////////// Events
	//void mouse_dragged(GLfloat xx, GLfloat yy);
};

class King : public Piece
{
public:
	King(Team team, int initialPosition);
	~King() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Queen : public Piece
{
public:
	Queen(Team team, int initialPosition);
	~Queen() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Rook : public Piece
{
public:
	Rook(Team team, int initialPosition);
	~Rook() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Bishop : public Piece
{
public:
	Bishop(Team team, int initialPosition);
	~Bishop() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Knight : public Piece
{
public:
	Knight(Team team, int initialPosition);
	~Knight() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Pawn : public Piece
{
public:
	Pawn(Team team, int initialPosition);
	~Pawn() {}

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};
#endif 

