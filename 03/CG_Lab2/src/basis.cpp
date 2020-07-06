#include <glm/gtx/quaternion.hpp>

#include "basis.h"
#include "stdio.h"

Basis::Basis(bool camera) : view(1.0), rotation(vec3()), camera(camera) {}

void Basis::move(vec3 distances) {
    view = translate(mat4(1.0), distances * BASIS_MOVE_SPEED) * view;
}

void Basis::rotate(vec3 angles) {
    if (camera) {
        for (int i = 0; i < 3; i++) {
            if (angles[i]) {
                vec3 direction;
                direction[i] = 1.0;
                view = glm::rotate(mat4(1.0), angles[i] * BASIS_MOVE_SPEED, direction) * view;
            }
        }
    } else
        rotation = quat(BASIS_ROTATE_SPEED * angles) * rotation;
}

vec4 Basis::get_camera_position() {
    return inverse(view) * vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

vec4 Basis::get_camera_direction() {
    return inverse(camera ? view : toMat4(rotation)) * vec4(0.0f, 0.0f, -1.0f, 0.0f);
}

vec4 Basis::get_position() {
    return view * vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Basis::animate(float deltaTime) {
    rotate(BASIS_ANIMATE_SPEED * animation * deltaTime);
}
