#include "game_object.h"
#include "glm/gtc/matrix_transform.hpp"

game_object::~game_object() {

}

glm::mat4 game_object::get_matrix() {
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * 
		   glm::mat4(rotation) * 
		   glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z)) * glm::mat4(1.0f);
}

void game_object::set_x(float new_x) {
	last_x = x;
	x = new_x;
}

void game_object::set_y(float new_y) {
	last_y = y;
	y = new_y;
}

void game_object::set_z(float new_z) {
	last_z = z;
	z = new_z;
}

void game_object::set_position(float new_x, float new_y, float new_z) {
	last_x = x;
	last_y = y;
	last_z = z;
	x = new_x;
	y = new_y;
	z = new_z;
}

void game_object::set_scale(float x_scale, float y_scale, float z_scale) {
	scale_x = x_scale;
	scale_y = y_scale;
	scale_z = z_scale;
}

void game_object::set_orientation(float angle, float axis_x, float axis_y, float axis_z) {
	rotation = glm::mat3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(axis_x, axis_y, axis_z)));
}

glm::vec3 game_object::move(float x_amount, float y_amount, float z_amount) {
	last_x = x;
	last_y = y;
	last_z = z;
	x += x_amount;
	y += y_amount;
	z += z_amount;
	return glm::vec3(x, y, z);
}

glm::vec3 game_object::move_forward(float amount) {
	glm::vec3 move_amount = glm::vec3(rotation * glm::vec3(0.0f, 0.0f, -amount));
	return move(move_amount.x, move_amount.y, move_amount.z);
}

glm::vec3 game_object::move_backward(float amount) {
	return move_forward(-amount);
}

glm::vec3 game_object::move_left(float amount) {
	glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 move_amount = amount * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));

	return move(move_amount.x, move_amount.y, move_amount.z);
}

glm::vec3 game_object::move_right(float amount) {
	return move_left(-amount);
}

glm::mat3 game_object::rotate(float angle, float axis_x, float axis_y, float axis_z) {
	rotation = glm::mat3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(axis_x, axis_y, axis_z))) * rotation;
	return rotation;
}

glm::mat3 game_object::pitch(float angle) {
	glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), rotation * glm::vec3(0.0f, 0.0f, -1.0f)));
	return rotate(angle, side.x, side.y, side.z);
}

glm::mat3 game_object::roll(float angle) {
	glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	return rotate(angle, forward.x, forward.y, forward.z);
}

glm::mat3 game_object::yaw(float angle) {
	return rotate(angle, 0, 1, 0);
}

bool game_object::collide(float point_x, float point_y, float point_z) {
	if(glm::distance(bounds.max, bounds.min)) {

	}
	return false;
}

bool game_object::collide(game_object object) {
	if(glm::distance(bounds.max, bounds.min)) {

	}
	return false;
}

glm::vec3 game_object::revert() {
	set_position(last_x, last_y, last_z);
	return glm::vec3(x, y, z);
}

void game_object::set_colour(float r, float g, float b) {
	geometry.set_colour(-1, glm::vec3(r, g, b));
}
