#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Transform {
    LEFT,
    RIGHT,
	UP,
	DOWN,
    FORWARD,
    BACKWARD
};

// Default camera values
const float CAMERA_MOVE_SPEED   =  0.5f;
const float CAMERA_ROTATE_SPEED =  0.5f;
const float ZOOM                =  45.0f;

class Camera {
	glm::mat4 view;
public:
	float Zoom;
	Camera() : view(1.0), Zoom(ZOOM) {
		reset();
	}
	void move(Camera_Transform direction, float deltaTime) {
		glm::vec3 vec;
        float velocity = CAMERA_MOVE_SPEED * deltaTime;
        if (direction == LEFT)
			vec = glm::vec3(velocity, 0.0f, 0.0f);
        if (direction == RIGHT)
			vec = glm::vec3(-velocity, 0.0f, 0.0f);
        if (direction == UP)
			vec = glm::vec3(0.0f, -velocity, 0.0f);
        if (direction == DOWN)
			vec = glm::vec3(0.0f, velocity, 0.0f);
        if (direction == FORWARD)
			vec = glm::vec3(0.0f, 0.0f, velocity);
        if (direction == BACKWARD)
			vec = glm::vec3(0.0f, 0.0f, -velocity);
		view = glm::translate(glm::mat4(1.0), vec) * view;
    }
	void rotate(Camera_Transform direction, float deltaTime) {
		glm::vec3 vec;
        float velocity = CAMERA_ROTATE_SPEED * deltaTime;
        if (direction == UP)
			vec = glm::vec3(-1.0f, 0.0f, 0.0f);
        if (direction == DOWN)
			vec = glm::vec3(1.0f, 0.0f, 0.0f);
        if (direction == LEFT)
			vec = glm::vec3(0.0f, -1.0f, 0.0f);
        if (direction == RIGHT)
			vec = glm::vec3(0.0f, 1.0f, 0.0f);
        if (direction == FORWARD)
			vec = glm::vec3(0.0f, 0.0f, -1.0f);
        if (direction == BACKWARD)
			vec = glm::vec3(0.0f, 0.0f, 1.0f);
		view = glm::rotate(glm::mat4(1.0), velocity, vec) * view;
    }
	void reset() {
		view = glm::translate(glm::mat4(1.0),
		                      glm::vec3(0.0f, 0.0f, -3.0f));
	}
	glm::vec3 get_position() {
		return glm::vec3(glm::inverse(view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	glm::vec3 get_direction() {
		return glm::vec3(glm::inverse(view) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	}
	glm::mat4 GetViewMatrix() {
        return view;
    }
	void ProcessMouseScroll(float yoffset) {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }
};

#endif
