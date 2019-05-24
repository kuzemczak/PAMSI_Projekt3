#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <algorithm>

#include "vectorUtils.h"
#include "Piece.h"
#include "Events.h"

class ChessBoard : Events
{
	std::vector<Piece*> board_;
	std::vector<Piece*> white_;
	std::vector<Piece*> black_;
	std::vector<Piece*> pieces_;
	Piece * movingPiece_;

	std::vector<Rect*> possibleMoveMarks_;
	std::vector<int> possibleMoves_;
	bool displayMoveMarks_;
	
	std::vector<Rect*> latestMoveMarks_;

	std::vector<int> moveHistory_;

	GLfloat boardPixWidth_,
		boardPixHeight_,
		xPos_,
		yPos_;

	Rect chessBoard;
	
	Team currentTeam_;

	int whiteTotalStrength_,
		blackTotalStrength_,
		strengthBalance_;

public:
	ChessBoard(GLfloat boardPixWidth, GLfloat boardPixHeight, GLfloat xPos = 0.0f, GLfloat yPos = 0.0f);
	~ChessBoard();

	void execute();

	void update_board_positions();
	void update_board_positions(int oldPos, int newPos);
	void update_screen_positions();
	void update_move_marks();
	void update_latest_move_marks();
	void draw();

	void change_team();
	Team current_team();
	void do_move(int move, bool noDisplay = false);
	int do_capture(int move);
	void do_castle(int move);
	int do_ep_capture(int  move);
	void undo_moves(int number, bool noDisplay = false);
	void undo_piece_move(int positionBeforeUndoing, int positionAfterUndoing);
	void undo_capture(Piece * capturedPiecePtr);

	std::vector<int> get_possible_moves(Team team, bool verifyCaptures = true, bool checkSafety = false);
	void order_moves(std::vector<int> & moves);
	int get_strength_balance();

	void board_pos_to_pix(int boardPos, GLfloat & xx, GLfloat & yy);
	int closest_square(GLfloat xx, GLfloat yy);
	bool is_square_safe(int square, Team teamCheckingSafety, const std::vector<int> & opponentMoves);
	void verify_king_moves_safety(int kingPos, Team kingTeam, std::vector<int> & moves);
	void verify_piece_captures(Team pawnTeam, std::vector<int> & moves);
	bool is_check(Team checkedTeam);
	bool is_checkmate(Team checkedTeam);

	void print_board();

	/////////////////////////////// Events
	void mouse_left_pressed(GLfloat xx, GLfloat yy);
	//void mouse_left_released(GLfloat xx, GLfloat yy);
};

#endif
