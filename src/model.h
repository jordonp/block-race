#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class renderer;

struct triangle_group
{
	std::vector<GLuint> triangles;
	glm::vec3 colour;

	GLuint ibo_triangles;
	triangle_group() : ibo_triangles(0), colour(1.0, 1.0, 1.0) {};
};

class model
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<triangle_group> triangle_groups;

	//opengl stuff
	GLuint vbo_vertices, vbo_normals, a_position, a_normal, u_colour;

	void compute_normals();
	void delete_model();
public:
	model();
	~model();

	void set_colour(long tri_group, glm::vec3 colour);
	void upload(renderer& r);
	void render();
};
#endif