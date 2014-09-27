#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

camera::camera() {

}

camera::~camera() {
	
}

glm::mat4 camera::get_view() {
	return glm::perspective(110.0f, 1280.0f/800.0f, 0.01f, 1000.0f) * glm::lookAt(glm::vec3(x, y, z), glm::vec3(x, y, z) + rotation * glm::vec3(0, 0, -1), glm::vec3(0.0f, 1.0f, 0.0f));
}
