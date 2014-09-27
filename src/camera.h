#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "game_object.h"

class camera : public game_object
{
public:
	camera();
	~camera();
	glm::mat4 get_view();
};
#endif