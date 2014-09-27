#ifndef LEVEL_H
#define LEVEL_H

#include <list>
#include <GL/glew.h>
#include "game_object.h"
#include "camera.h"

class renderer;

class level
{
	std::list<game_object> objects;

	//openGL stuff
	GLuint u_m, u_minv, u_mvp;
public:
	camera view;
	level();
	~level();
	

	void upload(renderer& r);
	void render();
	game_object& new_game_object();
};
#endif