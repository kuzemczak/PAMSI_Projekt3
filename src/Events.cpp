#include "Events.h"

std::vector<Events*> Events::objects;

void Events::handle_events(Events * itself)
{
	objects.push_back(itself);
}

void Events::unhandle_events(Events * itself)
{
	objects.erase(std::find(objects.begin(), objects.end(), itself));
}

void Events::mouse_left_pressed(GLfloat xx, GLfloat yy)
{
	
}

void Events::emit_mouse_left_pressed(GLfloat xx, GLfloat yy)
{
	for (Events * e : objects)
	{
		e->mouse_left_pressed(xx, yy);
	}
}

void Events::mouse_left_released(GLfloat xx, GLfloat yy)
{

}

void Events::emit_mouse_left_released(GLfloat xx, GLfloat yy)
{
	for (Events * e : objects)
	{
		e->mouse_left_released(xx, yy);
	}
}

void Events::mouse_dragged(GLfloat xx, GLfloat yy)
{
	//std::cout << "Events::mouse_dragged(...)\n";
}

void Events::emit_mouse_dragged(GLfloat xx, GLfloat yy)
{
	//std::cout << "Events::emit_mouse_dragged(" << xx << ", " << yy << ")\n"
	//	"objects.size() : " << objects.size() << "\n";
	for (Events * e : objects)
	{
		e->mouse_dragged(xx, yy);
	}
}

void Events::mouse_left_clicked(GLfloat xx, GLfloat yy)
{

}

void Events::emit_mouse_left_clicked(GLfloat xx, GLfloat yy)
{
	for (Events * e : objects)
	{
		e->mouse_left_clicked(xx, yy);
	}
}

void Events::check_event(Team checkedTeam)
{
	//std::cout << checkedTeam << " King is in check!\n";
}

void Events::emit_check_event(Team checkedTeam)
{
	for (Events * e : objects)
	{
		e->check_event(checkedTeam);
	}
}

void Events::checkmate_event(Team checkedTeam)
{
}

void Events::emit_checkmate_event(Team checkedTeam)
{
	for (Events * e : objects)
	{
		e->checkmate_event(checkedTeam);
	}
}

void Events::promo_event(int square)
{

}

void Events::emit_promo_event(int square)
{
	for (Events * e : objects)
	{
		e->promo_event(square);
	}
}

//void Events::new_game()
//{
//
//}
//
//void Events::emit_new_game()
//{
//	for (Events * e : objects)
//	{
//		e->new_game();
//	}
//}

//void Events::hint_event(int hintMove)
//{
//
//}
//
//void Events::emit_hint_event(int hintMove)
//{
//	for (Events * e : objects)
//	{
//		e->hint_event(hintMove);
//	}
//}