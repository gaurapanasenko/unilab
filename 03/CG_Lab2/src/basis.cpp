#include "basis.h"
#include "stdio.h"

Basis::Basis() : view(1.0) {
    reset();
}

void Basis::move(glm::vec3 direction, float deltaTime) {
    glm::vec3 vec = direction * BASIS_MOVE_SPEED * deltaTime;
    view = glm::translate(glm::mat4(1.0), vec) * view;
}

void Basis::rotate(glm::vec3 direction, float deltaTime,
                   bool fromPosition) {
    float velocity = BASIS_ROTATE_SPEED * deltaTime;
    glm::mat4 transform = glm::mat4(1.0);
    if (fromPosition) {
        glm::vec3 position = get_position();
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, -velocity, direction);
        transform = glm::translate(transform, -position);
    } else {
        transform = glm::rotate(transform, -velocity, direction);
    }
    view = transform * view;
}

void Basis::reset() {
}

glm::vec4 Basis::get_camera_position() {
    return glm::inverse(view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 Basis::get_camera_direction() {
    return glm::inverse(view) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}

glm::vec4 Basis::get_position() {
    return view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Basis::animate(float deltaTime) {
    float velocity = deltaTime * BASIS_ANIMATE_SPEED;
    float x = velocity * animation.x;
    float y = velocity * animation.y;
    float z = velocity * animation.z;
    rotate(X, x, true);
    rotate(Y, y, true);
    rotate(Z, z, true);
}
