#ifndef BASIS_H
#define BASIS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Y = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);

// Default camera values
const float BASIS_MOVE_SPEED   =  0.5f;
const float BASIS_ROTATE_SPEED =  0.5f;
const float BASIS_ANIMATE_SPEED = 1.0f;

class Basis {
public:
    glm::mat4 view;
    glm::vec3 animation;
    Basis();
    void move(glm::vec3 direction, float deltaTime);
    void rotate(glm::vec3 direction, float deltaTime,
                bool fromPosition = false);

    glm::vec4 get_camera_position();
    glm::vec4 get_camera_direction();

    glm::vec4 get_position();

    void animate(float deltaTime);
};

#endif // BASIS_H
