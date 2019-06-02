#include "ChessBoard.h"

#include "stb_image.h"

ChessBoard::ChessBoard(GLfloat boardPixWidth, GLfloat boardPixHeight, GLfloat xPos, GLfloat yPos) :
	boardPixHeight_(boardPixHeight),
	boardPixWidth_(boardPixWidth),
	xPos_(xPos),
	yPos_(yPos),
	chessBoard(boardPixWidth, boardPixHeight, xPos + boardPixWidth / 2, yPos + boardPixHeight / 2, 0.0f),
	whiteKingChecked_(false),
	blackKingChecked_(false),
	displayHint_(false),
	enabled_(true)
{
	handle_events(this);

	for (int i = 0; i < 64; i++)
	{
		board_.push_back(NULL);
	}
	
	Rect::setLocationMode(CENTER);
	Piece::set_texture_dir("data/");

	white_.push_back(new King(WHITE, 4));
	white_.push_back(new Queen(WHITE, 3));
	white_.push_back(new Rook(WHITE, 0));
	white_.push_back(new Rook(WHITE, 7));
	white_.push_back(new Bishop(WHITE, 2));
	white_.push_back(new Bishop(WHITE, 5));
	white_.push_back(new Knight(WHITE, 1));
	white_.push_back(new Knight(WHITE, 6));
	for (int i = 0; i < 8; i++)
		white_.push_back(new Pawn(WHITE, 8 + i));

	black_.push_back(new King(BLACK, 60));
	black_.push_back(new Queen(BLACK, 59));
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

	for (int i = 0; i < 2; i++)
	{
		latestMoveMarks_.push_back(new Rect(boardPixHeight_ / 8, -1000, -1000));
		load_shape_texture(*(*(latestMoveMarks_.end() - 1)), "data/full_mark.png");
		hintMarks_.push_back(new Rect(boardPixHeight_ / 8, -1000, -1000));
		load_shape_texture(*(*(hintMarks_.end() - 1)), "data/hint_mark.png");
	}

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("data/chessBoard.png", &width, &height, &nrChannels, 0);
	chessBoard.generateTexture(data, width, height, nrChannels);
	stbi_image_free(data);

	currentTeam_ = WHITE;

	fill_PossibleMovesNotChecked_();
}

ChessBoard::~ChessBoard()
{
	unhandle_events(this);
	for (Piece * p : pieces_)
		delete p;
	for (Rect * r : possibleMoveMarks_)
		delete r;
	for (Rect * r : latestMoveMarks_)
		delete r;
}

// Metoda wypelniajaca liste dostepnych ruchow
// niesprawdzonych pod katem bicia swoich
void ChessBoard::fill_PossibleMovesNotChecked_()
{
	for (Piece *p : pieces_)
	{
		if (!(p->is_captured()))
		{
			possibleMovesNotChecked_.push_back(p->get_moves(board_, moveHistory_));
		}
		else
		{
			possibleMovesNotChecked_.push_back(std::vector<int>(0));
		}
	}
	for (int i = 0; i < 64; i++)
		possibleMovesByDestination_.push_back(std::vector<Pair<int, int>>(0));
	fill_possibleMovesByDestination_();
}

// Metoda wypelniajaca liste dostepnych ruchow
// ulozonych wzgledem pola docelowego
void ChessBoard::fill_possibleMovesByDestination_()
{
	bool visited[64] = { false };
	for (int i = 0; i < possibleMovesNotChecked_.size(); i++)
	{
		for (int m : possibleMovesNotChecked_[i])
		{
			int index = GET_TO(m);
			if (!visited[index])
			{
				possibleMovesByDestination_[index].clear();
				visited[index] = true;
			}
			possibleMovesByDestination_[index].push_back(Pair<int, int>(m, i));
		}
	}
}

// Wlaczenie reagowania planszy na zdarzenia myszy
void ChessBoard::enable()
{
	enabled_ = true;
}

// Wylaczenie reagowania planszy na zdarzenia myszy
void ChessBoard::disable()
{
	enabled_ = false;
}

// Zaktualizowanie listy figur wg ich pozycji
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

// Zaktualizowanie pozycji jednej figury
// na liscie figur wg pozycji
void ChessBoard::update_board_positions(int oldPos, int newPos)
{
	board_[newPos] = board_[oldPos];
	board_[oldPos] = NULL;
}

// Zaktualizwoanie pozycji figur na ekranie
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

// zaktualizowanie znacznikow dostepnych ruchow figury
void ChessBoard::update_move_marks()
{
	for (Rect * r : possibleMoveMarks_)
	{
		delete r;
	}
	possibleMoveMarks_.clear();
	for (int pos : selectedPiecePossibleMoves_)
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

// zaktualizowanie znacznikow ostatniego ruchu
void ChessBoard::update_latest_move_marks()
{
	GLfloat xx = 0, yy = 0;
	int latestMove = *(moveHistory_.end() - 1);
	board_pos_to_pix(GET_FROM(latestMove), xx, yy);
	latestMoveMarks_[0]->setLocation(glm::vec2(xx, yy));
	board_pos_to_pix(GET_TO(latestMove), xx, yy);
	latestMoveMarks_[1]->setLocation(glm::vec2(xx, yy));
}

// Wyswietlenie znacznikow podpowiedzi
void ChessBoard::display_hint(int hintMove)
{
	GLfloat xx = 0, yy = 0;
	board_pos_to_pix(GET_FROM(hintMove), xx, yy);
	hintMarks_[0]->setLocation(glm::vec2(xx, yy));
	board_pos_to_pix(GET_TO(hintMove), xx, yy);
	hintMarks_[1]->setLocation(glm::vec2(xx, yy));

	displayHint_ = true;
}

// Rysowanie wszystkiego
void ChessBoard::draw()
{
	chessBoard.draw();

	if (displayMoveMarks_)
	{
		for (Rect * r : possibleMoveMarks_)
			r->draw();
	}
	if (displayHint_)
	{
		for (Rect * r : hintMarks_)
			r->draw();
	}
	for (Rect * r : latestMoveMarks_)
		r->draw();
	for (Piece* p : pieces_)
	{
		if (!(p->is_captured()))
			p->draw();
	}
}

// Zmiana druzyny wykonujacej ruch
void ChessBoard::change_team()
{
	if (currentTeam_ == WHITE)
	{
		currentTeam_ = BLACK;
	}
	else
		currentTeam_ = WHITE;
}

// Metoda zwracajaca druzyne wykonujaca ruch
Team ChessBoard::current_team()
{
	return currentTeam_;
}

// Metoda wykonujaca wszystkie czynnosci potrzebne,
// aby zrealizowac ruch figury
void ChessBoard::do_move(int move, bool display, bool updatePossibleMoves, bool evaluateCheck)
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
	change_team();

	moveHistory_.push_back(move);
	if (updatePossibleMoves)
	{
		possibleMovesHistory_.push_back(possibleMovesNotChecked_);
		possibleMovesByDestinationHistory_.push_back(possibleMovesByDestination_);
		update_possible_moves(movingPiece_->get_team(), move);
	}
	movingPiece_ = NULL;
	if (display && evaluateCheck)
		std::cout << "hoho";
	if (evaluateCheck)
	{
		if (evaluate_check(currentTeam_))
		{
			std::vector<int> tmp;
			bool b = !evaluate_checkmate(currentTeam_, &tmp);
			safeMovesIfCheck_ = tmp;
			std::cout << b;
			if (b)
			{
				if (currentTeam_ == BLACK)
					blackKingChecked_ = true;
				else
					whiteKingChecked_ = true;
				if (display)
					emit_check_event(currentTeam_);
			}
			else
			{
				if (display)
					emit_checkmate_event(currentTeam_);
			}
			//emit_check_event(currentTeam_);
		}
		else
		{
			if (currentTeam_ == BLACK)
				blackKingChecked_ = false;
			else
				whiteKingChecked_ = false;
		}
	}

	kingCheckHistory_.push_back(Pair<bool, bool>(whiteKingChecked_, blackKingChecked_));
	
	if (display)
	{
		std::cout << "disp ";
		update_screen_positions();
		update_latest_move_marks();
		displayHint_ = false;
	}
}

// Metoda realizujaca przechwycenie figury
int ChessBoard::do_capture(int move)
{
	int to = GET_TO(move);
	Piece * capturedPtr = board_[to];
	board_[to] = NULL;
	
	capturedPtr->set_captured(true);
	strengthBalance_ -= capturedPtr->get_strength();

	return find_i(pieces_, capturedPtr);
}

// Metoda realizujaca roszade
void ChessBoard::do_castle(int move)
{
	int from = 0, to = 0;
	if (has_bits_set(move, QUEEN_CASTLE))
	{
		from = GET_TO(move) - 2;
		to = GET_TO(move) + 1;
	}
	else
	{
		from = GET_TO(move) + 1;
		to = GET_TO(move) - 1;
	}
	board_[from]->move_board(to);
	update_board_positions(from, to);
}

// Metoda wykonujaca bicie w przelocie
int ChessBoard::do_ep_capture(int move)
{
	return do_capture((GET_TO(move) - movingPiece_->get_team() * 8) << 6);
}

// Metoda cofajaca podana ilosc ruchow zapisanych w historii
void ChessBoard::undo_moves(int number, bool display , bool redoPossibleMoves)
{
	for (int i = 0; i < number; i++)
	{
		if (moveHistory_.size() == 0)
			return;

		int m = *(moveHistory_.end() - 1);
		moveHistory_.pop_back();
		if (redoPossibleMoves)
		{
			possibleMovesNotChecked_ = *(possibleMovesHistory_.end() - 1);
			possibleMovesByDestination_ = *(possibleMovesByDestinationHistory_.end() - 1);
			possibleMovesHistory_.pop_back();
			possibleMovesByDestinationHistory_.pop_back();
		}
		Pair<bool, bool> checks = *(kingCheckHistory_.end() - 1);
		whiteKingChecked_ = checks.first;
		blackKingChecked_ = checks.second;
		kingCheckHistory_.pop_back();

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
			int oldPos = from + 1, newPos = from + 3;
			undo_piece_move(oldPos, newPos);
		}
		else if (has_bits_set(m, QUEEN_CASTLE))
		{
			int oldPos = from - 1, newPos = from - 4;
			undo_piece_move(oldPos, newPos);
		}
		change_team();

		if (display)
		{
			update_screen_positions();
			update_latest_move_marks();
		}
	}
}

// Metoda cofajaca ruch figury
void ChessBoard::undo_piece_move(int positionBeforeUndoing, int positionAfterUndoing)
{
	board_[positionBeforeUndoing]->undo_move(positionAfterUndoing);
	update_board_positions(positionBeforeUndoing, positionAfterUndoing);
}

// Metoda cofajaca przechwycenie figury
void ChessBoard::undo_capture(Piece * capturedPiecePtr)
{
	int pos = capturedPiecePtr->get_board_position();
	capturedPiecePtr->set_captured(false);
	board_[pos] = capturedPiecePtr;
	strengthBalance_ += capturedPiecePtr->get_strength();
}

// Metoda generujaca mozliwe ruchy
std::vector<int> ChessBoard::generate_possible_moves(Team team, bool verifyCaptures, bool checkSafety)
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

// Metoda aktualizujaca mozliwe ruchy, uzywana po wykonaniu podanego ruchu
void ChessBoard::update_possible_moves(Team movingTeam, int move)
{
	int a = 0, b = 0;
	if (has_bits_set(move, KING_CASTLE))
		update_possible_moves(movingTeam, (GET_TO(move) + movingTeam) | ((GET_TO(move) - movingTeam) << 6));
	else if (has_bits_set(move, QUEEN_CASTLE))
		update_possible_moves(movingTeam, (GET_TO(move) - movingTeam*2) | ((GET_TO(move) + movingTeam) << 6));
	
	bool toUpdate[32] = { false };
	{
		std::vector<std::vector<Pair<int, int>>*> movesIntersecting = {
			&(possibleMovesByDestination_[GET_TO(move)]),
			&(possibleMovesByDestination_[GET_FROM(move)]) };
		if (has_bits_set(move, EP_CAPTURE))
			movesIntersecting.push_back(&(possibleMovesByDestination_[GET_TO(move) - movingTeam * 8]));
		else if (has_bits_set(move, KING_CASTLE))
		{
			movesIntersecting.push_back(&(possibleMovesByDestination_[(GET_TO(move) + 1)]));
			movesIntersecting.push_back(&(possibleMovesByDestination_[(GET_TO(move) - 1)]));
		}
		else if (has_bits_set(move, QUEEN_CASTLE))
		{
			movesIntersecting.push_back(&(possibleMovesByDestination_[(GET_TO(move) - 2)]));
			movesIntersecting.push_back(&(possibleMovesByDestination_[(GET_TO(move) + 1)]));
		}

		for (std::vector<Pair<int, int>> * moves : movesIntersecting)
		{
			for (int i = 0; i < moves->size(); i++)
			{
				Piece * piecePtr = pieces_[(*moves)[i].second];

				if (!(piecePtr->is_captured()))
				{
					toUpdate[(*moves)[i].second] = true;
				}
			}
		}
	}

	// sprawdzenie, czy ucieczka z en passant
	if (board_[GET_TO(move)] != NULL &&
		board_[GET_TO(move)]->get_type() == PAWN &&
		board_[GET_TO(move)]->get_move_count() == 2)
	{
		for (Pair<int, int> m : possibleMovesByDestination_[GET_FROM(move) - movingTeam * 8])
		{
			if (has_bits_set(m.first, EP_CAPTURE))
				toUpdate[m.second] = true;
		}
	}

	// sprawdzenie czy zbicie piona pod en passant za pomoca innej figury
	if (has_bits_set(move, CAPTURE) &&
		pieces_[GET_CAPTURED(move)]->get_type() == PAWN &&
		pieces_[GET_CAPTURED(move)]->get_move_count() == 1)
	{
		for (Pair<int, int> m : possibleMovesByDestination_[GET_TO(move) + movingTeam * 8])
		{
			if (has_bits_set(m.first, EP_CAPTURE))
				toUpdate[m.second] = true;
		}
	}

	for (int i = 0; i < pieces_.size(); i++)
	{
		Piece * piecePtr = pieces_[i];
		Team team = piecePtr->get_team();
		if (toUpdate[i] ||
			(piecePtr->get_type() == PAWN &&
				!(piecePtr->is_captured()) &&
				(GET_FROM(move) == piecePtr->get_board_position() + team * 8 ||
				(piecePtr->get_move_count() == 0 && GET_FROM(move) == piecePtr->get_board_position() + team * 16) ||
					(has_bits_set(move, DOUBLE_PAWN_PUSH) && 
						((GET_TO(move) % 8 > 0 && piecePtr->get_board_position() == GET_TO(move) - 1) ||
						(GET_TO(move) % 8 < 7 && piecePtr->get_board_position() == GET_TO(move) + 1))))))
		{
			possibleMovesNotChecked_[i] = piecePtr->get_moves(board_, moveHistory_);
		}
	}
	fill_possibleMovesByDestination_();
}

// Metoda zwracajaca mozliwe ruchy
std::vector<int> ChessBoard::get_possible_moves(Team team, bool verifyCaptures, bool checkSafety, bool allowChecksafeMoves)
{
	std::vector<int> ret;

	if (allowChecksafeMoves &&
		checkSafety &&
		(team == WHITE && whiteKingChecked_ ||
			team == BLACK && blackKingChecked_))
	{
		//evaluate_checkmate(team, &ret);
		ret = safeMovesIfCheck_;
	}
	else
	{
		int offset = 0;
		if (team == BLACK)
			offset = white_.size();

		for (int i = 0; i < white_.size(); i++)
		{
			if (!(pieces_[i + offset]->is_captured()))
			{
				std::vector<int> moves = possibleMovesNotChecked_[i + offset];
				if (pieces_[i + offset]->get_type() == KING &&
					checkSafety)
				{
					verify_king_moves_safety(pieces_[i + offset]->get_board_position(), team, moves);
				}
				ret.insert(ret.end(), moves.begin(), moves.end());
			}
		}

		if (verifyCaptures)
		{
			verify_piece_captures(team, ret);
			order_moves(ret);
		}
	}

	return ret;
}

// Metoda porzadkujaca ruchy wzgledem bicia
// Ruchy bijace najbardziej wartosciowe figury ustawiane sa na poczatku
// Ruchy ciche na koncu
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

	if (kingCaps.size() > 0)
		ret.insert(ret.end(), kingCaps.begin(), kingCaps.end());
	if (queenCaps.size() > 0)
		ret.insert(ret.end(), queenCaps.begin(), queenCaps.end());
	if (otherCaps.size() > 0)
		ret.insert(ret.end(), otherCaps.begin(), otherCaps.end());
	if (pawnCaps.size() > 0)
		ret.insert(ret.end(), pawnCaps.begin(), pawnCaps.end());
	if (otherMoves.size() > 0)
		ret.insert(ret.end(), otherMoves.begin(), otherMoves.end());

	moves = ret;
}

// Metoda zwracajaca wartosc planszy
int ChessBoard::get_strength_balance()
{
	return strengthBalance_;
}

// Metoda zmieniajaca pozycje na planszy,
// na pozycje na ekranie w pikselach
void ChessBoard::board_pos_to_pix(int boardPos, GLfloat & xx, GLfloat	& yy)
{
	int rest = boardPos % 8;
	xx = xPos_ + ((GLfloat)rest + 0.5f) * (boardPixWidth_ / 8);
	yy = yPos_ + boardPixHeight_ * ((GLfloat)(boardPos - rest) + 4.0f) / 64;
}

// Metoda zwracajaca pole planszy, ktore jest najblizej
// podanych wspolrzednych w pikselach
int ChessBoard::closest_square(GLfloat xx, GLfloat yy)
{
	int w8 = static_cast<int>(boardPixWidth_) / 8,
		h8 = static_cast<int>(boardPixHeight_) / 8,
		x = static_cast<int>(xx) - xPos_,
		y = static_cast<int>(yy) - yPos_;

	return (x - (x % w8)) / w8 + 8 * (y - (y % h8)) / h8;
}

// Metoda sprawdzajaca, czy podane pole jest bezpieczne (nie bite)
bool ChessBoard::is_square_safe(int square, Team teamCheckingSafety/*, const std::vector<int> & opponentMoves*/)
{
	//bool ret = true;
	//if (board_[square] == NULL ||
	//	(board_[square] != NULL && 
	//		(board_[square]->get_team() != teamCheckingSafety && board_[square]->get_type() != KING
	//		|| board_[square]->get_team() == teamCheckingSafety && board_[square]->get_type() == KING)))
	//{
	//	for (int m : opponentMoves)
	//	{
	//		if (GET_TO(m) == square && 
	//			(board_[GET_FROM(m)]->get_type() != PAWN ||
	//			(board_[GET_FROM(m)]->get_type() == PAWN && has_bits_set(m, PAWN_CAPTURE))))
	//		{
	//			ret = false;
	//			break;
	//		}
	//	}
	//}
	for (Pair<int, int> m : possibleMovesByDestination_[square])
	{
		if (has_bits_set(m.first, CAPTURE) && pieces_[m.second]->get_team() != teamCheckingSafety)
			return false;
	}
	return true;
	//return ret;
}

// Metoda usuwajaca ruchy niebezpieczne w podanej liscie ruchow krola
void ChessBoard::verify_king_moves_safety(int kingPos, Team kingTeam, std::vector<int> & moves)
{
	std::vector<int> indicesToErase;
	Piece * ptr = board_[kingPos];
	board_[kingPos] = NULL;
	//std::vector<int> opponentMoves = generate_possible_moves(other_team(kingTeam), false);
	for (int i = moves.size() - 1; i > -1; i--)
	{
		if (!is_square_safe(GET_TO(moves[i]), kingTeam/*, opponentMoves*/))
		{
			moves.erase(moves.begin() + i);
		}
	}
	board_[kingPos] = ptr;
}

// Metoda usuwajaca z podanej listy ruchow bicia wlasnej druzyny
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

// Metoda sprawdzajaca, czy jest szach
bool ChessBoard::evaluate_check(Team checkedTeam)
{
	std::vector<Piece*> * checkedTeamSet;
	if (checkedTeam == BLACK)
		checkedTeamSet = &black_;
	else
		checkedTeamSet = &white_;

	Piece * kingPtr = NULL;

	for (Piece * p : *checkedTeamSet)
		if (p->get_type() == KING)
		{
			kingPtr = p;
			break;
		}

	int position = kingPtr->get_board_position();
	//board_[position] = NULL;

	//std::vector<int> opponentMoves = get_possible_moves(other_team(checkedTeam), false);
	//board_[position] = kingPtr;
	return !is_square_safe(position, checkedTeam/*, opponentMoves*/);
}

// Metoda sprawdzajaca, czy jest mat
bool ChessBoard::evaluate_checkmate(Team checkedTeam, std::vector<int> *safeMovesContainer)
{
	std::vector<int> moves = get_possible_moves(checkedTeam, true, true, false);

	for (int m : moves)
	{
		do_move(m, false,true,false);
		if (!evaluate_check(checkedTeam))
		{
			if (safeMovesContainer == NULL)
			{
				undo_moves(1, false);
				return false;
			}
			else
			{
				safeMovesContainer->push_back(m);
			}
		}
		undo_moves(1, false);
	}

	if (safeMovesContainer != NULL)
	{
		if (safeMovesContainer->size() > 0)
			return false;
	}
	return true;


}

// Metoda zwracajaca true, jezeli krol podanej druzyny jest w szachu
bool ChessBoard::is_king_checked(Team checkedTeam)
{
	if (checkedTeam == WHITE)
		return blackKingChecked_;
	return whiteKingChecked_;
}

// Metoda wypisujaca plansze na standardowe wyjscie
void ChessBoard::print_board()
{
	for (int i = board_.size() - 1; i > -1; i--)
	{
		int index = (i - (2 * (i % 8))) + 7;
		if (i % 8 == 7)
			std::cout << std::endl;
		if (board_[index] == NULL)
			std::cout << " | ";
		else
			std::cout << board_[index]->get_name()[0] << "| ";
	}
	std::cout << std::endl;
}

//////////////////////////// Events

// Obsluzenie zdarzenia wcisniecia lewego przycisku myszy 
void ChessBoard::mouse_left_pressed(GLfloat xx, GLfloat yy)
{
	static bool pieceSelected = false;
	if (!enabled_)
		return;
	if (!pieceSelected)
	{
		evaluate_checkmate(currentTeam_);
		for (Piece * p : pieces_)
		{
			if (!(p->is_captured()) &&
				p->get_team() == currentTeam_ &&
				p->shape_contains(xx, yy))
			{
				selectedPiecePossibleMoves_ = p->get_moves(board_, moveHistory_);

				if (selectedPiecePossibleMoves_.size() != 0)
				{
					verify_piece_captures(currentTeam_, selectedPiecePossibleMoves_);
					if (p->get_type() == KING)
					{
						verify_king_moves_safety(p->get_board_position(), 
							currentTeam_,
							selectedPiecePossibleMoves_);
					}
					else
					{
						for (int i = selectedPiecePossibleMoves_.size() - 1; i > -1; i--)
						{
							do_move(selectedPiecePossibleMoves_[i], false, true, false);
							if (evaluate_check(p->get_team()))
								selectedPiecePossibleMoves_.erase(selectedPiecePossibleMoves_.begin() + i);
							undo_moves(1, false, true);
						}
					}
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

		auto comp = std::find_if(selectedPiecePossibleMoves_.begin(),
			selectedPiecePossibleMoves_.end(), compare(closestPos << 6));

		if (comp != selectedPiecePossibleMoves_.end())
		{
			do_move(*comp);
		}
		else
		{
			movingPiece_ = NULL;
		}

		selectedPiecePossibleMoves_.clear();
		update_move_marks();
		pieceSelected = NULL;
	}
}