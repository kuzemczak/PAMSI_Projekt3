#include "ChessBoard.h"

#include "stb_image.h"

ChessBoard::ChessBoard(GLfloat boardPixWidth, GLfloat boardPixHeight) :
	boardPixHeight_(boardPixHeight),
	boardPixWidth_(boardPixWidth),
	chessBoard(boardPixWidth, boardPixHeight, boardPixHeight / 2, boardPixWidth / 2, 0.0f)
{
	handle_events(this);

	for (int i = 0; i < 64; i++)
	{
		board_.push_back(NULL);
	}
	
	Rect::setLocationMode(CENTER);
	Piece::set_texture_dir("data/");

	white_.push_back(new King(WHITE));
	white_.push_back(new Queen(WHITE));
	white_.push_back(new Rook(WHITE, 0));
	white_.push_back(new Rook(WHITE, 7));
	white_.push_back(new Bishop(WHITE, 2));
	white_.push_back(new Bishop(WHITE, 5));
	white_.push_back(new Knight(WHITE, 1));
	white_.push_back(new Knight(WHITE, 6));
	for (int i = 0; i < 8; i++)
		white_.push_back(new Pawn(WHITE, 8 + i));

	black_.push_back(new King(BLACK));
	black_.push_back(new Queen(BLACK));
	black_.push_back(new Rook(BLACK, 63));
	black_.push_back(new Rook(BLACK, 56));
	black_.push_back(new Bishop(BLACK, 58));
	black_.push_back(new Bishop(BLACK, 61));
	black_.push_back(new Knight(BLACK, 57));
	black_.push_back(new Knight(BLACK, 62));
	for (int i = 0; i < 8; i++)
		black_.push_back(new Pawn(BLACK, 48 + i));

	for (Piece* p : white_)
		pieces_.push_back(p);

	for (Piece* p : black_)
		pieces_.push_back(p);

	update_board_positions();
	update_screen_positions();

	selectedPiece_ = NULL;

	displayMoveMarks_ = false;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("data/chessBoard.png", &width, &height, &nrChannels, 0);
	chessBoard.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);

	currentTeam_ = WHITE;

	ai_turn = false;
}

void ChessBoard::execute()
{
	if (ai_turn)
	{
		int move = ChessAI::get_next_move(BLACK, board_, moveHistory_);
		selectedPiece_ = board_[GET_FROM(move)];
		manage_move(move);
		ai_turn = false;
	}
}

void ChessBoard::update_board_positions()
{
	for (int i = 0; i < 64; i++)
	{
		board_[i]= NULL;
	}

	for (Piece * p : pieces_)
	{
		board_[p->get_board_position()] = p;
	}
}

void ChessBoard::update_screen_positions()
{
	for (Piece * p : pieces_)
	{
		GLfloat x = 0, y = 0;
		board_pos_to_pix(p->get_board_position(), x, y);
		p->move_screen(x, y);
	}
}

void ChessBoard::update_move_marks()
{
	for (Rect * r : possibleMoveMarks_)
	{
		delete r;
	}
	possibleMoveMarks_.clear();
	for (int pos : possibleMoves_)
	{
		GLfloat x = 0, y = 0;
		board_pos_to_pix((pos >> 6) & 63, x, y);
		Rect * ptr = new Rect(boardPixWidth_ / 8, x, y);
		possibleMoveMarks_.push_back(ptr);
		
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load("data/mark.png", &width, &height, &nrChannels, 0);
		ptr->generateTexture(data, width, height, nrChannels);
		stbi_image_free(data);
	}
}

void ChessBoard::draw()
{
	chessBoard.draw();

	if (displayMoveMarks_)
	{
		for (Rect * r : possibleMoveMarks_)
			r->draw();
	}
	for (Piece* p : pieces_)
		p->draw();
}

void ChessBoard::change_team()
{
	if (currentTeam_ == WHITE)
	{
		currentTeam_ = BLACK;
		ai_turn = true;
	}
	else
		currentTeam_ = WHITE;
}

void ChessBoard::manage_move(int move)
{
	if (has_bits_set(move, CAPTURE))
	{
		do_capture(move);
	}
	else if (has_bits_set(move, KING_CASTLE) ||
		has_bits_set(move, QUEEN_CASTLE))
	{
		do_castle(move);
	}
	else if (has_bits_set(move, EP_CAPTURE))
	{
		do_ep_capture(move);
	}

	selectedPiece_->move_board(GET_TO(move));
	update_board_positions();
	update_screen_positions();

	change_team();

	moveHistory_.push_back(move);

	selectedPiece_ = NULL;
}

void ChessBoard::do_capture(int move)
{
	Piece * capturedPtr = board_[GET_TO(move)];
	board_[GET_TO(move)] = NULL;
	if (currentTeam_ == WHITE)
	{
		black_.erase(std::find(black_.begin(), black_.end(), capturedPtr));
	}
	else
	{
		white_.erase(std::find(white_.begin(), white_.end(), capturedPtr));
	}
	pieces_.erase(std::find(pieces_.begin(), pieces_.end(), capturedPtr));
	delete capturedPtr;
}

void ChessBoard::do_castle(int move)
{
	if (has_bits_set(move, QUEEN_CASTLE))
	{
		board_[GET_TO(move) - selectedPiece_->get_team() * 2]->move_board(GET_TO(move) + selectedPiece_->get_team());
	}
	else
	{
		board_[GET_TO(move) + selectedPiece_->get_team() * 1]->move_board(GET_TO(move) - selectedPiece_->get_team());
	}
}

void ChessBoard::do_ep_capture(int move)
{
	do_capture((GET_TO(move) - selectedPiece_->get_team() * 8) << 6);
}

void ChessBoard::board_pos_to_pix(int boardPos, GLfloat & xx, GLfloat	& yy)
{
	int rest = boardPos % 8;
	xx = ((GLfloat)rest + 0.5f) * (boardPixWidth_ / 8);
	yy = boardPixHeight_ * ((GLfloat)(boardPos - rest) + 4.0f) / 64;
}

int ChessBoard::closest_square(GLfloat xx, GLfloat yy)
{
	int w8 = static_cast<int>(boardPixWidth_) / 8,
		h8 = static_cast<int>(boardPixHeight_) / 8,
		x = static_cast<int>(xx),
		y = static_cast<int>(yy);

	return (x - (x % w8)) / w8 + 8 * (y - (y % h8)) / h8;
}

void ChessBoard::print_board()
{
	for (int i = 0; i < board_.size(); i++)
	{
		if (i % 8 == 0)
			std::cout << std::endl;
		if (board_[i] == NULL)
			std::cout << "\t, ";
		else
			std::cout << board_[i]->get_name() << ", ";
	}
	std::cout << std::endl;
}

//////////////////////////// Events
void ChessBoard::mouse_left_pressed(GLfloat xx, GLfloat yy)
{
	if (selectedPiece_ == NULL)
	{
		for (Piece * p : pieces_)
		{
			if (
				p->get_team() == currentTeam_ &&
				p->shape_contains(xx, yy)
				)
			{
				possibleMoves_ = p->get_moves(board_, moveHistory_);

				if (possibleMoves_.size() != 0)
				{
					update_move_marks();
					displayMoveMarks_ = true;
					selectedPiece_ = p;
				}
				break;
			}
		}
	}
	else
	{
		int closestPos = closest_square(xx, yy);

		struct compare
		{
			int key;
			compare(int const &i) : key(i) { }

			bool operator()(int const &i)
			{
				return ((i & (63 << 6)) == key);
			}
		};

		auto comp = std::find_if(possibleMoves_.begin(),
			possibleMoves_.end(), compare(closestPos << 6));

		if (comp != possibleMoves_.end())
		{
			manage_move(*comp);
		}

		possibleMoves_.clear();
		update_move_marks();
	}
}