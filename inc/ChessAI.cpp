#include "ChessAI.h"

#include "randutils.hpp"

int cntr = 0;

/* Funkcja zwracajaca najlepszy ruch. Reprezentuje wierzcholek
* startowy drzewa.
* Dane wejsciowe:
* board - obiekt planszy, na ktorej ma byc wykonany ruch
* team - enum wskazujacy druzyne, ktora ma wykonac ruch
* depth - glebokosc przeszukiwania
* 
* Dane wyjsciowe:
* int zawierajacy informacje o ruchu
*/
int get_next_move(ChessBoard & board, Team team, int depth)
{
	// ustawienie najgorszej wartosci planszy dla danej druzyny
	int ret = -100000 * team;

	// index najlepszego ruchu na liscie mozliwych ruchow
	int bestMoveIndex = 0; 

	// uzyskanie z planszy listy mozliwych ruchow dla danej druzyny
	std::vector<int> moves = board.get_possible_moves(team, true, true);

	// lista indeksow najlepszych ruchow.
	// Moze sie zdarzyc, ze kilka bedzie dawalo
	// taka sama wartosc planszy.
	std::vector<int> bestMoveIndices;

	// petla algorytmu, wykonuje kazdy ruch
	for (int i = 0; i < moves.size(); i++)
	{
		// zmienna przechowujaca wartosc planszy zwrocona przez potomka
		int outcome = 0;

		// sprawdzenie glebokosci
		if (depth > 1)
		{
			// jesli to nie lisc, to wywolanie rekurencyjne
			board.do_move(moves[i], false);
			outcome = minimax(board, other_team(team), depth - 1, ret);
			board.undo_moves(1, false);
		}
		else
		{
			// jesli to lisc, to uzyskanie wartosci planszy
			board.do_move(moves[i], false, false);
			outcome = board.get_strength_balance();
			board.undo_moves(1, false, false);
		}

		// sprawdzenie wartosci zwroconej wyzej
		if (outcome == ret)
		{// jesli jest taka sama jak poprzednia najlepsza,
			// to wepchniecie na liste najlepszych
			bestMoveIndices.push_back(i);
		}
		else if ((team == BLACK && outcome < ret) ||
			(team == WHITE && outcome > ret))
		{
			// jesli jest lepsza od ostatnio najlepszej
			// to wyczyszczenie listy najlepszych i wwepchniecie
			// na nia nowego najlepszego ruchu
			ret = outcome;
			bestMoveIndices.clear();
			bestMoveIndices.push_back(i);
		}
	}

	// wylosowanie ruchu z najlepszych
	ret = moves[bestMoveIndices[random(0, (int)bestMoveIndices.size() - 1)]];

	return ret;
}

/* Funkcja wewnetrzna algorytmu minimax. Reprezentuje
* wierzcholki inne niz startowy.
* Dane wejsciowe:
* board - obiekt planszy, na ktorej ma byc wykonany ruch
* team - enum wskazujacy druzyne, ktorej ruchy sa przeszukiwane
* depth - glebokosc przeszukiwania
* pruningParameter - parametr alfa lub beta przycinania.
*		Wskazuje, jaka byla ostatnio najlepsza wartosc dla rodzica.
* 
* Dane wyjsciowe:
* int zawierajacy najlepszy dla danej druzyny wynik planszy.
*/
int minimax(ChessBoard & board, Team team, int depth, int pruningParameter)
{
	// ustawienie najgorszej wartosci planszy dla danej druzyny
	int ret = -100000 * team;
	
	// uzyskanie z planszy listy mozliwych ruchow dla danej druzyny
	std::vector<int> moves = board.get_possible_moves(team, true, true);

	// warunek przycinania, decyduje o przerwaniu petli
	bool pruningCondition = false;

	// petla przeszukujaca ruchy. Konczy sie gdy wszystkie ruchy zsotana przeszukane,
	// lub gdy warunek przycinania bedzie spelniony
	for (int i = 0; i < moves.size() && !pruningCondition; i++)
	{
		// zmienna przechowujaca wartosc planszy zwrocona przez potomka
		int outcome = 0;
		
		// sprawdzenie glebokosci
		if (depth > 1)
		{
			// jesli to nie lisc, to wywolanie rekurencyjne
			board.do_move(moves[i], false);
			outcome = minimax(board, other_team(team), depth - 1, ret);
			board.undo_moves(1, false);
		}
		else
		{
			// jesli to lisc, to uzyskanie wartosci planszy
			board.do_move(moves[i], false, false, false);
			outcome = board.get_strength_balance();
			board.undo_moves(1, false, false);
		}
		
		// parametr zapewniajacy wieksze zroznicowanie
		// w przypadku wielu ruchow o tym samym wyniku
		int a = random(0, 1);

		if (team == BLACK)
		{
			// aktualizacja warunku przycinania
			pruningCondition = outcome < pruningParameter;

			// aktualizacja najlepszej wartosci planszy
			if (outcome < ret + a)
				ret = outcome;
		}
		else
		{
			// aktualizacja warunku przycinania
			pruningCondition = outcome > pruningParameter;

			// aktualizacja najlepszej wartosci planszy
			if (outcome > ret - a)
				ret = outcome;
		}

	}

	return ret;
}