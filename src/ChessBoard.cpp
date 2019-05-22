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

	whiteTotalStrength_ = 0;
	blackTotalStrength_ = 0;
	for (Piece* p : white_)
		whiteTotalStrength_ += p->get_strength();

	for (Piece* p : black_)
		blackTotalStrength_ += p->get_strength();

	strengthBalance_ = whiteTotalStrength_ + blackTotalStrength_;

	for (Piece* p : white_)
		pieces_.push_back(p);

	for (Piece* p : black_)
		pieces_.push_back(p);

	update_board_positions();
	update_screen_positions();

	movingPiece_ = NULL;

	displayMoveMarks_ = false;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("data/chessBoard.png", &width, &height, &nrChannels, 0);
	chessBoard.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);

	currentTeam_ = WHITE;
}

void ChessBoard::execute()
{
	/*if (aiTurn_)
	{
		int move = ChessAI::get_next_move(BLACK, board_, moveHistory_);
		do_move(move);
		aiTurn_ = false;
	}*/
}

void ChessBoard::update_board_positions()
{
	for (int i = 0; i < 64; i++)
	{
		board_[i]= NULL;
	}

	for (Piece * p : pieces_)
	{
		if (p != NULL && !(p->is_captured()))
			board_[p->get_board_position()] = p;
	}
}

void ChessBoard::update_board_positions(int oldPos, int newPos)
{
	board_[newPos] = board_[oldPos];
	board_[oldPos] = NULL;
}

void ChessBoard::update_screen_positions()
{
	for (Piece * p : pieces_)
	{
		if (p != NULL && !(p->is_captured()))
		{
			GLfloat x = 0, y = 0;
			board_pos_to_pix(p->get_board_position(), x, y);
			p->move_screen(x, y);
		}
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
	{
		if (!(p->is_captured()))
			p->draw();
	}
}

void ChessBoard::change_team()
{
	if (currentTeam_ == WHITE)
	{
		currentTeam_ = BLACK;
	}
	else
		currentTeam_ = WHITE;
}

Team ChessBoard::current_team()
{
	return currentTeam_;
}

void ChessBoard::do_move(int move, bool noDisplay)
{
	movingPiece_ = board_[GET_FROM(move)];

	if (has_bits_set(move, CAPTURE))
	{
		int capturedIndex = -1;
		if (has_bits_set(move, EP_CAPTURE))
			capturedIndex = do_ep_capture(move);
		else
			capturedIndex = do_capture(move);

		if (capturedIndex > -1)
			move = move | MAKE_CAPTURED(capturedIndex);
	}
	else if (has_bits_set(move, KING_CASTLE) ||
		has_bits_set(move, QUEEN_CASTLE))
	{
		do_castle(move);
	}

	movingPiece_->move_board(GET_TO(move));
	update_board_positions(GET_FROM(move), GET_TO(move));

	if (!noDisplay)
		update_screen_positions();

	change_team();

	moveHistory_.push_back(move);

	movingPiece_ = NULL;

	if (!noDisplay && is_check(currentTeam_))
	{
		if (is_checkmate(currentTeam_))
			std::cout << "checkmate!\n";
		else
			emit_check_event(currentTeam_);
	}
}

int ChessBoard::do_capture(int move)
{
	Piece * capturedPtr = board_[GET_TO(move)];
	board_[GET_TO(move)] = NULL;
	
	capturedPtr->set_captured(true);
	strengthBalance_ -= capturedPtr->get_strength();

	return find_i(pieces_, capturedPtr);
}

void ChessBoard::do_castle(int move)
{
	int from = 0, to = 0;
	if (has_bits_set(move, QUEEN_CASTLE))
	{
		from = GET_TO(move) - movingPiece_->get_team() * 2;
		to = GET_TO(move) + movingPiece_->get_team();
	}
	else
	{
		from = GET_TO(move) + movingPiece_->get_team();
		to = GET_TO(move) - movingPiece_->get_team();
	}
	board_[from]->move_board(to);
	update_board_positions(from, to);
}

int ChessBoard::do_ep_capture(int move)
{
	return do_capture((GET_TO(move) - movingPiece_->get_team() * 8) << 6);
}

void ChessBoard::undo_moves(int number, bool noDisplay)
{
	for (int i = 0; i < number; i++)
	{
		if (moveHistory_.size() == 0)
			return;

		int m = *(moveHistory_.end() - 1);
		moveHistory_.pop_back();
		int from = GET_FROM(m),
			to = GET_TO(m);
		Piece * undonePiece = board_[to];
		Team team = undonePiece->get_team();
		
		undo_piece_move(to, from);

		if (has_bits_set(m, CAPTURE))
		{
			Piece * ptr = pieces_[GET_CAPTURED(m)];
			undo_capture(ptr);
		}
		else if (has_bits_set(m, KING_CASTLE))
		{
			int oldPos = from + team, newPos = from + team * 3;
			undo_piece_move(oldPos, newPos);
		}
		else if (has_bits_set(m, QUEEN_CASTLE))
		{
			int oldPos = from - team, newPos = from + team * 3;
			undo_piece_move(oldPos, newPos);
		}
		change_team();

		if (!noDisplay)
			update_screen_positions();
	}
}

void ChessBoard::undo_piece_move(int positionBeforeUndoing, int positionAfterUndoing)
{
	board_[positionBeforeUndoing]->undo_move(positionAfterUndoing);
	update_board_positions(positionBeforeUndoing, positionAfterUndoing);
}

void ChessBoard::undo_capture(Piece * capturedPiecePtr)
{
	capturedPiecePtr->set_captured(false);
	board_[capturedPiecePtr->get_board_position()] = capturedPiecePtr;
	strengthBalance_ += capturedPiecePtr->get_strength();
}

std::vector<int> ChessBoard::get_possible_moves(Team team, bool verifyCaptures, bool checkSafety)
{
	std::vector<int> ret;
	std::vector<Piece*>* iteratedTeam = NULL;
	if (team == WHITE)
		iteratedTeam = &white_;
	else
		iteratedTeam = &black_;

	for (Piece *p : *iteratedTeam)
	{
		if (!(p->is_captured()))
		{
			//auto t0 = Clock::now();
			std::vector<int> moves = p->get_moves(board_, moveHistory_);
			//auto t1 = Clock::now();
			//std::cout << p->get_name() << " get_moves time: "
			//	<< static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count())
			//	<< std::endl;
			
			if (p->get_type() == KING && checkSafety)
			{
				verify_king_moves_safety(p->get_board_position(), team, moves);
			}

			ret.insert(ret.end(), moves.begin(), moves.end());
		}
	}

	if (verifyCaptures)
	{
		verify_piece_captures(team, ret);
		order_moves(ret);
	}
	return ret;
}

void ChessBoard::order_moves(std::vector<int> & moves)
{
	std::vector<int> ret, kingCaps, queenCaps, pawnCaps, otherCaps, otherMoves;

	for (int i = moves.size() - 1; i > -1; i--)
	{
		int m = moves[i];
		if (has_bits_set(m, CAPTURE) && board_[GET_TO(m)] != NULL)
		{
			PieceType type = board_[GET_TO(m)]->get_type();
			switch (type)
			{
			case KING:
				kingCaps.push_back(m);
				break;
			case QUEEN:
				queenCaps.push_back(m);
				break;
			case PAWN:
				pawnCaps.push_back(m);
				break;
			default:
				otherCaps.push_back(m);
			}
		}
		else
		{
			otherMoves.push_back(m);
		}
	}

	ret.insert(ret.end(), kingCaps.begin(), kingCaps.end());
	ret.insert(ret.end(), queenCaps.begin(), queenCaps.end());
	ret.insert(ret.end(), otherCaps.begin(), otherCaps.end());
	ret.insert(ret.end(), pawnCaps.begin(), pawnCaps.end());
	ret.insert(ret.end(), otherMoves.begin(), otherMoves.end());

	moves = ret;
}

int ChessBoard::get_strength_balance()
{
	return strengthBalance_;
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

bool ChessBoard::is_square_safe(int square, Team teamCheckingSafety, const std::vector<int> & opponentMoves)
{
	bool ret = true;
	if (board_[square] == NULL ||
		(board_[square] != NULL && 
			(board_[square]->get_team() != teamCheckingSafety && board_[square]->get_type() != KING
			|| board_[square]->get_team() == teamCheckingSafety && board_[square]->get_type() == KING)))
	{
		for (int m : opponentMoves)
		{
			if (GET_TO(m) == square && 
				(board_[GET_FROM(m)]->get_type() != PAWN ||
				(board_[GET_FROM(m)]->get_type() == PAWN && has_bits_set(m, PAWN_CAPTURE))))
			{
				ret = false;
				break;
			}
		}
	}
	return ret;
}

void ChessBoard::verify_king_moves_safety(int kingPos, Team kingTeam, std::vector<int> & moves)
{
	std::vector<int> indicesToErase;
	Piece * ptr = board_[kingPos];
	board_[kingPos] = NULL;
	std::vector<int> opponentMoves = get_possible_moves(other_team(kingTeam), false);
	for (int i = 0; i < moves.size(); i++)
	{
		if (!is_square_safe(GET_TO(moves[i]), kingTeam, opponentMoves))
		{
			indicesToErase.push_back(i);
		}
	}
	for (int i = indicesToErase.size() - 1; i > -1; i--)
	{
		moves.erase(moves.begin() + indicesToErase[i]);
	}
	board_[kingPos] = ptr;
}

void ChessBoard::verify_piece_captures(Team pieceTeam, std::vector<int> & moves)
{
	std::vector<int> indicesToErase;
	for (int i = moves.size() - 1; i > -1; i--)
	{
		int to = GET_TO(moves[i]);
		if ((has_bits_set(moves[i], PAWN_CAPTURE) &&
			(board_[to] == NULL ||
			(board_[to] != NULL && board_[to]->get_team() == pieceTeam))) ||
				(has_bits_set(moves[i], CAPTURE) &&
					board_[to] != NULL &&
					board_[to]->get_team() == pieceTeam))
		{
			moves.erase(moves.begin() + i);
		}
	}
}

bool ChessBoard::is_check(Team checkedTeam)
{
	std::vector<Piece*> * checkedTeamSet;
	if (checkedTeam == BLACK)
		checkedTeamSet = &black_;
	else
		checkedTeamSet = &white_;

	Piece * kingPtr = NULL;

	for (Piece * p : *checkedTeamSet)
		if (p->get_type() == KING)
			kingPtr = p;

	int position = kingPtr->get_board_position();
	board_[position] = NULL;

	std::vector<int> opponentMoves = get_possible_moves(other_team(checkedTeam), false);
	board_[position] = kingPtr;
	return !is_square_safe(position, checkedTeam, opponentMoves);
}

bool ChessBoard::is_checkmate(Team checkedTeam)
{
	std::vector<int> moves = get_possible_moves(checkedTeam, true, true);

	for (int m : moves)
	{
		do_move(m);
		if (!is_check(checkedTeam))
		{
			undo_moves(1);
			return false;
		}
		undo_moves(1);
	}

	return true;
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
	static bool pieceSelected = false;
	if (!pieceSelected)
	{
		for (Piece * p : pieces_)
		{
			if (!(p->is_captured()) &&
				p->get_team() == currentTeam_ &&
				p->shape_contains(xx, yy))
			{
				possibleMoves_ = p->get_moves(board_, moveHistory_);

				if (possibleMoves_.size() != 0)
				{
					if (p->get_type() == KING)
					{
						verify_king_moves_safety(p->get_board_position(), 
							currentTeam_,
							possibleMoves_);
					}
					verify_piece_captures(currentTeam_, possibleMoves_);
					update_move_marks();
					displayMoveMarks_ = true;
					pieceSelected = true;
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
			do_move(*comp);
		}
		else
		{
			movingPiece_ = NULL;
		}

		possibleMoves_.clear();
		update_move_marks();
		pieceSelected = NULL;
	}
}