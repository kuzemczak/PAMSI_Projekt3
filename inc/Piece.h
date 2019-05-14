/* Move encryption:
** (0 to 64) | (0 to 63) << 6 | (0 to 16 w/o 6 & 7) << 12
** `---v---'   `------v------'  `-----------v------------'
**   from            to                 special
**/
// Special bits:
#define QUIET_MOVE (0 << 12)				// quiet move
#define DOUBLE_PAWN_PUSH (1 << 12)	// double pawn push
#define KING_CASTLE (2 << 12)				// king castle
#define QUEEN_CASTLE (3 << 12)			// queen castle
#define CAPTURE (4 << 12)						// capture
#define EP_CAPTURE (5 << 12)				// ep-capture
#define N_PROMO (8 << 12)						// knight-promotion
#define B_PROMO (9 << 12)						// bishop-promotion
#define R_PROMO (10 << 12)					// rook-promotion
#define Q_PROMO (11 << 12)					// queen-promotion
#define N_PROMO_CAPTURE (12 << 12)	// knight-promo capture
#define B_PROMO_CAPTURE (13 << 12)	// bishop-promo capture
#define R_PROMO_CAPTURE (14 << 12)	// rook-promo capture
#define Q_PROMO_CAPTURE (15 << 12)	// queen-promo capture
#define RAY_MOVE (16 << 12)					// ray move
#define NEGATIVE_MOVE (32 << 12)		// negative (opposite direction)



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
	bool has_bits_set(int move, int bits);
public:
	Piece(Team team, std::string name = "");
	~Piece() {}

	static void set_texture_dir(const std::string & name);

	void move_screen(glm::vec2 screenPosition);
	void move_screen(GLfloat xx, GLfloat yy);
	virtual void move_board(glm::i8vec2 boardPosition);
	virtual void move_board(GLuint xx, GLuint yy);
	virtual void move_board(int pos);

	virtual std::vector<int>  get_moves(std::vector<Piece*> board);
	bool shape_contains(GLfloat xx, GLfloat yy);
	int get_board_position();
	int get_strength();
	GLuint get_move_count();
	Team get_team();

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
	void move_board(int pos);

	std::vector<int> get_moves(std::vector<Piece*> board);
};

class Queen : public Piece
{
public:
	Queen(Team team);
	~Queen() {}

	std::vector<int> get_moves(std::vector<Piece*> board);
};
#endif 
