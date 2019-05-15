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