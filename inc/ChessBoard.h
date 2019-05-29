#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <algorithm>

#include "vectorUtils.h"
#include "Piece.h"
#include "Events.h"

template <typename T1, typename T2>
struct Pair
{
	T1 first;
	T2 second;

	Pair() {}
	Pair(T1 first_, T2 second_) :
		first(first_),
		second(second_) {}
};

class ChessBoard : Events, public GraphicalGameObjectInterface
{
	std::vector<Piece*> white_;
	std::vector<Piece*> black_;
	std::vector<Piece*> pieces_;
	Piece * movingPiece_;

	std::vector<Rect*> possibleMoveMarks_;
	std::vector<Rect*> hintMarks_;
	std::vector<int> selectedPiecePossibleMoves_;
	std::vector<std::vector<int>> whitePossibleMovesNotChecked_,
		blackPossibleMovesNotChecked_,
		possibleMovesNotChecked_,
		possibleMoves_;
	std::vector<int> safeMovesIfCheck_;

	std::vector<std::vector<Pair<int, int>>>
		possibleMovesByDestination_;

	bool displayMoveMarks_;
	bool displayHint_;
	bool enabled_;
	
	std::vector<Rect*> latestMoveMarks_;

	std::vector<int> moveHistory_;
	std::vector<std::vector<std::vector<int>>> possibleMovesHistory_;
	std::vector < std::vector<std::vector<Pair<int, int>>>> possibleMovesByDestinationHistory_;
	std::vector<Pair<bool, bool>> kingCheckHistory_;

	GLfloat boardPixWidth_,
		boardPixHeight_,
		xPos_,
		yPos_;

	Rect chessBoard;
	
	Team currentTeam_;

	int whiteTotalStrength_,
		blackTotalStrength_,
		strengthBalance_;

	bool whiteKingChecked_,
		blackKingChecked_;

	void fill_PossibleMovesNotChecked_();
	void fill_possibleMovesByDestination_();
public:
	ChessBoard(GLfloat boardPixWidth, GLfloat boardPixHeight, GLfloat xPos = 0.0f, GLfloat yPos = 0.0f);
	~ChessBoard();

	std::vector<Piece*> board_;


	void execute();
	void enable();
	void disable();

	void update_board_positions();
	void update_board_positions(int oldPos, int newPos);
	void update_screen_positions();
	void update_move_marks();
	void update_latest_move_marks();
	void display_hint(int hintMove);
	void draw();

	void change_team();
	Team current_team();
	void do_move(int move, bool display = true, bool updatePossibleMoves = true, bool evaluateCheck = true);
	int do_capture(int move);
	void do_castle(int move);
	int do_ep_capture(int  move);
	void undo_moves(int number, bool display = true, bool redoPossibleMoves = true);
	void undo_piece_move(int positionBeforeUndoing, int positionAfterUndoing);
	void undo_capture(Piece * capturedPiecePtr);

	std::vector<int> generate_possible_moves(Team team, bool verifyCaptures = true, bool checkSafety = false);
	void update_possible_moves(Team movingTeam, int move);
	std::vector<int> get_possible_moves(Team team, bool verifyCaptures = true, bool checkSafety = false, bool allowChecksafeMoves = true);
	void order_moves(std::vector<int> & moves);
	int get_strength_balance();

	void board_pos_to_pix(int boardPos, GLfloat & xx, GLfloat & yy);
	int closest_square(GLfloat xx, GLfloat yy);
	bool is_square_safe(int square, Team teamCheckingSafety/*, const std::vector<int> & opponentMoves*/);
	void verify_king_moves_safety(int kingPos, Team kingTeam, std::vector<int> & moves);
	void verify_piece_captures(Team pawnTeam, std::vector<int> & moves);
	bool evaluate_check(Team checkedTeam);
	bool evaluate_checkmate(Team checkedTeam, std::vector<int> *safeMovesContainer = NULL);
	bool is_king_checked(Team checkedTeam);

	void print_board();

	/////////////////////////////// Events
	void mouse_left_pressed(GLfloat xx, GLfloat yy);
	//void mouse_left_released(GLfloat xx, GLfloat yy);
};

#endif
