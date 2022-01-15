#include "arcball_camera.h"

ArcballCamera::ArcballCamera() = default;

ArcballCamera::ArcballCamera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector, float yaw, float pitch) :
    m_eye(std::move(eye)),
    m_lookAt(std::move(lookAt)),
    m_upVector(std::move(upVector)),
    yaw(yaw),
    pitch(pitch)
{
    this->zoom = 45.0f;
    this->mouse_sensitivity = 0.1f;

    updateViewMatrix();

}


glm::mat4x4 ArcballCamera::getViewMatrix() const
{
    return m_viewMatrix;
}

glm::vec3 ArcballCamera::getEye() const
{
    return m_eye;
}

glm::vec3 ArcballCamera::getUpVector() const
{
    return m_upVector;
}

glm::vec3 ArcballCamera::getLookAt() const
{
    return m_lookAt;
}

// Camera forward axis is defined as -z
glm::vec3 ArcballCamera::getViewDirection() const
{
    return -glm::transpose(m_viewMatrix)[2];
}

glm::vec3 ArcballCamera::getRightVector() const
{
    return glm::transpose(m_viewMatrix)[0];
}

void ArcballCamera::setCameraView(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up)
{
    m_eye = std::move(eye);
    m_lookAt = std::move(lookAt);
    m_upVector = std::move(up);

    updateViewMatrix();
}

void ArcballCamera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}

void ArcballCamera::processMouseTilt(float x_offset, float y_offset)
{

    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;


    // Constrain pitch.
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    updateCameraVectors();
}

void ArcballCamera::processMouseScroll(float y_offset)
{
    zoom -= (float)y_offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void ArcballCamera::updateCameraVectors()
{
    glm::vec3 front_vector;
    front_vector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_vector.y = sin(glm::radians(pitch));
    front_vector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // m_upVector += sin(glm::radians(pitch));

    // also re-calculate the Right and Up vector
    // m_lookAt = glm::normalize(front_vector);
    // m_lookAt.z = 0;
    // m_right = glm::normalize(glm::cross(m_lookAt, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    // m_upVector = glm::normalize(glm::cross(m_right, m_lookAt));
    // printf("m_lookAt = (%f, %f, %f)\n", m_lookAt.x, m_lookAt.y, m_lookAt.z);
    // setCameraView(m_eye, m_lookAt, m_upVector);
}

void ArcballCamera::processMouseRotation(float x_offset, float y_offset, int viewportWidth, int viewportHeight)
{
    glm::vec4 position(m_eye.x, m_eye.y, m_eye.z, 1.0f);
    glm::vec4 pivot(m_lookAt.x, m_lookAt.y, m_lookAt.z, 1.0f);

    float delta_angle_x = 2 * M_PI / viewportWidth; // Movement from left to right.
    float delta_angle_y = M_PI / viewportHeight;    // Moevement from top to botom.
    float x_angle = x_offset * delta_angle_x;
    float y_angle = y_offset * delta_angle_y;

    // Handle situation, when direction is the same as up vector (constaints movement up).
    float cos_angle = glm::dot(getViewDirection(), m_upVector);
    if (cos_angle * glm::sign(delta_angle_y) > 0.99f)
        delta_angle_y = 0;

    // Rotate camera around the pivot point on x axis.
    glm::mat4x4 rotation_matrix_x(1.0f);
    rotation_matrix_x = glm::rotate(rotation_matrix_x, x_angle, m_upVector);
    position = rotation_matrix_x * (position - pivot) + pivot;

    // Rotate camera around the pivot point on x axis.
    glm::mat4x4 rotation_matrix_y(1.0f);
    rotation_matrix_y = glm::rotate(rotation_matrix_y, y_angle, getRightVector());

    glm::vec3 final_position = rotation_matrix_y * (position - pivot) + pivot;

    setCameraView(final_position, getLookAt(), m_upVector);
}