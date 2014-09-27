#include "model.h"
#include "renderer.h"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

model::model() : vbo_vertices(0), vbo_normals(0) {
	vertices = {glm::vec3( 0.5,-0.5,-0.5), glm::vec3( 0.5,-0.5, 0.5), glm::vec3(-0.5,-0.5, 0.5),
				glm::vec3( 0.5,-0.5,-0.5), glm::vec3(-0.5,-0.5, 0.5), glm::vec3(-0.5,-0.5,-0.5),
				glm::vec3( 0.5, 0.5,-0.5), glm::vec3(-0.5, 0.5,-0.5), glm::vec3(-0.5, 0.5, 0.5),
				glm::vec3( 0.5, 0.5,-0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3( 0.5, 0.5, 0.5),
				glm::vec3( 0.5,-0.5,-0.5), glm::vec3( 0.5, 0.5,-0.5), glm::vec3( 0.5, 0.5, 0.5),
				glm::vec3( 0.5,-0.5,-0.5), glm::vec3( 0.5, 0.5, 0.5), glm::vec3( 0.5,-0.5, 0.5),
				glm::vec3( 0.5,-0.5, 0.5), glm::vec3( 0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5),
				glm::vec3( 0.5,-0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5,-0.5, 0.5),
				glm::vec3(-0.5,-0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5,-0.5),
				glm::vec3(-0.5,-0.5, 0.5), glm::vec3(-0.5, 0.5,-0.5), glm::vec3(-0.5,-0.5,-0.5),
				glm::vec3( 0.5, 0.5,-0.5), glm::vec3( 0.5,-0.5,-0.5), glm::vec3(-0.5,-0.5,-0.5),
				glm::vec3( 0.5, 0.5,-0.5), glm::vec3(-0.5,-0.5,-0.5), glm::vec3(-0.5, 0.5,-0.5)};

	compute_normals();

	triangle_groups.emplace_back();

	for (int i = 0; i < vertices.size(); ++i)
	{
		triangle_groups.back().triangles.push_back(i);
	}
}

model::~model() {
	delete_model();
}

void model::delete_model() {
	if(vbo_vertices)
		glDeleteBuffers(1, &vbo_vertices);
	if(vbo_normals)
		glDeleteBuffers(1, &vbo_normals);
	for (int i = 0; i < triangle_groups.size(); ++i)
	{
		if(triangle_groups[i].ibo_triangles)
			glDeleteBuffers(1, &triangle_groups[i].ibo_triangles);
	}

}

void model::set_colour(long tri_group, glm::vec3 colour) {
	if(tri_group < 0) {
		for (int i = 0; i < triangle_groups.size(); ++i)
		{
			triangle_groups[i].colour = colour;
		}
	} else if(tri_group < triangle_groups.size()) {
		triangle_groups[tri_group].colour = colour;
	}
}

void model::compute_normals() {
	normals.clear();
	vector<int> seen(vertices.size(), 0);
	normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < triangle_groups.size(); i++) {
        for (int j = 0; j < triangle_groups[i].triangles.size(); j += 3) {
            GLuint v[3] = {triangle_groups[i].triangles[j], triangle_groups[i].triangles[j+1], triangle_groups[i].triangles[j+2]};
            
            glm::vec3 normal = glm::normalize(glm::cross(vertices[v[1]] - vertices[v[0]], vertices[v[2]] - vertices[v[0]]));
                        
            for(int k=0; k<3; k++) {
                GLuint cur_v = v[k];
                seen[cur_v]++;
                if (seen[cur_v] == 1) {
                    normals[cur_v] = normal;
                } else {
                    normals[cur_v] = glm::normalize(normals[cur_v] * (1.0f-1.0f/seen[cur_v]) + normal * (1.0f/seen[cur_v]));
                }
            }
        }
    }
}

void model::upload(renderer& r) {
	delete_model();

	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);

	for (int i = 0; i < triangle_groups.size(); ++i)
	{
		glGenBuffers(1, &triangle_groups[i].ibo_triangles);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_groups[i].ibo_triangles);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_groups[i].triangles.size() * sizeof(triangle_groups[i].triangles[0]), triangle_groups[i].triangles.data(), GL_STATIC_DRAW);
	}

	a_position = glGetAttribLocation(r.program, "position");
	a_normal = glGetAttribLocation(r.program, "normal");
	u_colour = glGetUniformLocation(r.program, "colour");
}

void model::render() {
	glEnableVertexAttribArray(a_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(a_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glVertexAttribPointer(a_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < triangle_groups.size(); ++i)
	{
		glUniform3fv(u_colour, 1, glm::value_ptr(triangle_groups[i].colour));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_groups[i].ibo_triangles);
		glDrawElements(GL_TRIANGLES, triangle_groups[i].triangles.size(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(a_position);
	glDisableVertexAttribArray(a_normal);
}