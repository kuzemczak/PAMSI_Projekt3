#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <algorithm>

#include "vectorUtils.h"
#include "Piece.h"
#include "Events.h"
#include "ChessAI.h"

class ChessBoard : Events
{
	std::vector<Piece*> board_;
	std::vector<Piece*> white_;
	std::vector<Piece*> black_;
	std::vector<Piece*> pieces_;
	Piece * selectedPiece_;

	std::vector<Rect*> possibleMoveMarks_;
	std::vector<int> possibleMoves_;
	bool displayMoveMarks_;

	std::vector<int> moveHistory_;

	GLfloat boardPixWidth_,
		boardPixHeight_;

	Rect chessBoard;
	
	Team currentTeam_;

	bool ai_turn;

public:
	ChessBoard(GLfloat boardPixWidth, GLfloat boardPixHeight);

	void execute();

	void update_board_positions();
	void update_screen_positions();
	void update_move_marks();
	void draw();

	void change_team();
	void manage_move(int move);
	void do_capture(int move);
	void do_castle(int move);
	void do_ep_capture(int move);

	void board_pos_to_pix(int boardPos, GLfloat & xx, GLfloat & yy);
	int closest_square(GLfloat xx, GLfloat yy);

	void print_board();

	/////////////////////////////// Events
	void mouse_left_pressed(GLfloat xx, GLfloat yy);
	//void mouse_left_released(GLfloat xx, GLfloat yy);
};

#endif
