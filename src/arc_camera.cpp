#include "arc_camera.h"

ArcCamera::ArcCamera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float yaw, float pitch) :
    position(position),
    world_up(up),
    front(glm::vec3(0, 0, 0)),
    look_at_point(look_at),
    movement_speed(SPEED),
    mouse_sensitivity(SENSITIVITY),
    zoom(ZOOM),
    yaw(yaw),
    pitch(pitch)
{
    // this->position = position;
    // this->world_up = up;
    // this->yaw = yaw;
    // this->pitch = pitch;

    updateCameraVectors();
}

glm::mat4 ArcCamera::getViewMatrix()
{
    return glm::lookAt(position, look_at_point, up);
}

void ArcCamera::processKeyboard(float x_offset, float y_offset, int viewportWidth, int viewportHeight)
{
    glm::vec4 v4_position(position.x, position.y, position.z, 1.0f);
    glm::vec4 v4_pivot(look_at_point.x, look_at_point.y, look_at_point.z, 1.0f);

    glm::vec3 direction = glm::normalize(position - look_at_point);

    float delta_angle_x = 2 * M_PI / viewportWidth; // Movement from left to right.
    float delta_angle_y = M_PI / viewportHeight;    // Moevement from top to botom.
    float x_angle = x_offset * delta_angle_x;
    float y_angle = y_offset * delta_angle_y;

    // printf("%f %f\n", x_angle, y_angle);
    // printf("orbit height = %f%", glm::length(position - glm::vec3(0,0,0)));

    // Handle situation, when direction is the same as up vector (constaints movement up).
    float cos_angle = glm::dot(direction, up);
    if (cos_angle * glm::sign(delta_angle_y) > 0.99f)
        delta_angle_y = 0;

    // Rotate camera around the pivot point on x axis.
    glm::mat4x4 rotation_matrix_x(1.0f);
    rotation_matrix_x = glm::rotate(rotation_matrix_x, x_angle, up);
    position = rotation_matrix_x * (v4_position - v4_pivot) + v4_pivot;

    // Rotate camera around the pivot point on x axis.
    glm::vec3 viewMatrix =  glm::transpose(glm::lookAt(position, front, up))[0];
    glm::mat4x4 rotation_matrix_y(1.0f);
    rotation_matrix_y = glm::rotate(rotation_matrix_y, y_angle,  viewMatrix);

    glm::vec3 final_position = rotation_matrix_y * (v4_position - v4_pivot) + v4_pivot;

    position = final_position;
    updateCameraVectors();

    // front = v4_pivot;
}

void ArcCamera::processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (constrain_pitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // updateCameraVectors();
}

void ArcCamera::processMouseScroll(float y_offset)
{
    zoom -= (float)y_offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void ArcCamera::updateCameraVectors()
{
    glm::vec3 front_vector;
    front_vector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_vector.y = sin(glm::radians(pitch));
    front_vector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_vector);


    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}

// void ArcCamera::undoMove(float delta_time)
// {
//     for (Camera_Movement direction : previous_moves)
//     {
//         float velocity = movement_speed * delta_time;
//         switch (direction)
//         {
//             case FORWARD:
//                 position -= front * velocity;
//                 break;
//             case BACKWARD:
//                 position += front * velocity;
//                 break;
//             case LEFT:
//                 position += right * velocity;
//                 break;
//             case RIGHT:
//                  position -= right * velocity;
//                 break;
//             case UP:
//                 position -= up * velocity;
//                 break;
//             case DOWN:
//                 position += up * velocity;
//             default:
//                 break;
//         }
//     }
// }