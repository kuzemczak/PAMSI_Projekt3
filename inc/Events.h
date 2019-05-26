#ifndef EVENTS_H
#define EVENTS_H

#include <GL/glew.h>

#include <vector>

#include "vectorUtils.h"
#include "Team.h"

class Events
{
	static std::vector<Events*> objects;
public:
	void handle_events(Events * itself);
	void unhandle_events(Events * itself);

	////////////////////////////// AVAILABLE EVENTS
	virtual void mouse_left_pressed(GLfloat xx, GLfloat yy);
	static void emit_mouse_left_pressed(GLfloat xx, GLfloat yy);

	virtual void mouse_left_released(GLfloat xx, GLfloat yy);
	static void emit_mouse_left_released(GLfloat xx, GLfloat yy);

	virtual void mouse_dragged(GLfloat xx, GLfloat yy);
	static void emit_mouse_dragged(GLfloat xx, GLfloat yy);

	virtual void mouse_left_clicked(GLfloat xx, GLfloat yy);
	static void emit_mouse_left_clicked(GLfloat xx, GLfloat yy);

	virtual void check_event(Team checkedTeam);
	static void emit_check_event(Team checkedTeam);

	virtual void checkmate_event(Team checkedTeam);
	static void emit_checkmate_event(Team checkedTeam);

	//virtual void new_game();
	//static void emit_new_game();
	//
	//virtual void hint_event(int hintMove);
	//static void emit_hint_event(int hintMove);


};

#endif
