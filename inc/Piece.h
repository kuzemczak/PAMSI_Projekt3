/* Move encryption:
** (0 to 64) | (0 to 63) << 6 | (0 to 16 w/o 6 & 7) << 12
** `---v---'   `------v------'  `-----------v------------'
**   from            to                 special
**/
#define GET_FROM(i) ((i) & 63)			// decodes destination part of encoded move
#define GET_TO(i) (((i) >> 6) & 63)	// decodes origin part of encoded move
// Special bits:
#define QUIET_MOVE (0 << 12)						// quiet move
#define KING_CASTLE (1 << 12)						// king castle
#define QUEEN_CASTLE (2 << 12)					// queen castle
#define CAPTURE (4 << 12)								// capture
#define EP_CAPTURE (8 << 12)						// ep-capture
#define N_PROMO (16 << 12)							// knight-promotion
#define B_PROMO (32 << 12)							// bishop-promotion
#define R_PROMO (64 << 12)							// rook-promotion
#define Q_PROMO (128 << 12)							// queen-promotion
#define N_PROMO_CAPTURE (256 << 12)			// knight-promo capture
#define B_PROMO_CAPTURE (512 << 12)			// bishop-promo capture
#define R_PROMO_CAPTURE (1024 << 12)		// rook-promo capture
#define Q_PROMO_CAPTURE (2048 << 12)		// queen-promo capture
#define RAY_MOVE (4096 << 12)						// ray move
#define NEGATIVE_MOVE (8192 << 12)			// negative (opposite direction)
#define PAWN_CAPTURE (16384 << 12)			// diagonal move of a Pawn
#define PAWN_PUSH (32768 << 12)					// straight move of a Pawn
#define DOUBLE_PAWN_PUSH (65536 << 12)	// double pawn push
#define PROMOTION (131072 << 12)				// general Pawn promotion



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

	std::vector<int> moves_;
	int boardPosition_;
	std::string name_;
	Rect shape_;
	Team team_;
	int strength_;
	GLuint moveCntr_;

	int gen_move(int from, int to, int specialBits);
	
public:
	Piece(Team team, std::string name = "");
	~Piece() {}

	static void set_texture_dir(const std::string & name);

	void set_init_position(int pos);

	void move_screen(glm::vec2 screenPosition);
	void move_screen(GLfloat xx, GLfloat yy);
	virtual void move_board(glm::i8vec2 boardPosition);
	virtual void move_board(GLuint xx, GLuint yy);
	virtual void move_board(int pos);

	virtual std::vector<int>  get_moves(
		const std::vector<Piece*> & board, 
		const std::vector<int> & moveHistory);
	bool shape_contains(GLfloat xx, GLfloat yy);
	int get_board_position();
	int get_strength();
	GLuint get_move_count();
	Team get_team();
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
	King(Team team);
	~King() {}

	void move_board(glm::i8vec2 boardPosition);
	void move_board(GLuint xx, GLuint yy);
	void move_board(int pos);

	std::vector<int> get_moves(const std::vector<Piece*> & board, const std::vector<int> & moveHistory);
};

class Queen : public Piece
{
public:
	Queen(Team team);
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

