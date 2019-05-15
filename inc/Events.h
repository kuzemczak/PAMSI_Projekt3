#ifndef EVENTS_H
#define EVENTS_H

#include <GL/glew.h>

#include <vector>

#include "vectorUtils.h"

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

};

#endif
