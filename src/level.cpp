#include "level.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer.h"

level::level() {

}

level::~level() {

}

void level::upload(renderer& r) {
	u_m = glGetUniformLocation(r.program, "m");
    u_mvp = glGetUniformLocation(r.program, "mvp");
    u_minv = glGetUniformLocation(r.program, "minv");
    for (auto object = objects.begin(); object != objects.end(); object++)
    {
    	object->geometry.upload(r);
    }
}

void level::render() {
	glm::mat4 camera_matrix = view.get_view();
	for (auto object = objects.begin(); object != objects.end(); object++)
	{
		glm::mat4 m = object->get_matrix();

		glm::mat3 m_inv = glm::mat3(glm::inverse(m));

		glm::mat4 mvp = camera_matrix * m;

		glUniformMatrix4fv(u_m, 1, GL_FALSE, glm::value_ptr(m));
        glUniformMatrix3fv(u_minv, 1, GL_FALSE, glm::value_ptr(m_inv));
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
        
        object->geometry.render();
	}
}

game_object& level::new_game_object() {
	objects.push_back(game_object());
	return objects.back();
}