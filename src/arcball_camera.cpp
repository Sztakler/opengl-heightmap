#include "arcball_camera.h"

ArcballCamera::ArcballCamera() = default;

ArcballCamera::ArcballCamera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector, float yaw, float pitch) :
    m_eye(std::move(eye)),
    m_lookAt(std::move(lookAt)),
    m_upVector(std::move(upVector)),
    yaw(yaw),
    pitch(pitch)
{
    this->upVector_tilt = upVector;
    this->front_tilt = m_lookAt;
    this->pos = m_eye;

    this->zoom = 45.0f;
    this->mouse_sensitivity = 0.1f;

    // updateCameraVectors();

    updateViewMatrix();

}

void ArcballCamera::printViewMatrix()
{
    printf("[%f %f %f %f]\n|%f %f %f %f|\n|%f %f %f %f|\n[%f %f %f %f]\n\n",
        m_viewMatrix[0].x, m_viewMatrix[0].y, m_viewMatrix[0].z, m_viewMatrix[0].w,
        m_viewMatrix[1].x, m_viewMatrix[1].y, m_viewMatrix[1].z, m_viewMatrix[1].w,
        m_viewMatrix[2].x, m_viewMatrix[2].y, m_viewMatrix[2].z, m_viewMatrix[2].w,
        m_viewMatrix[3].x, m_viewMatrix[3].y, m_viewMatrix[3].z, m_viewMatrix[3].w);
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
    m_viewMatrix = glm::lookAt(eye, lookAt, up);
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

    // upVector_tilt += sin(glm::radians(pitch));

    // also re-calculate the Right and Up vector
    front_tilt = glm::normalize(front_vector);
    // m_lookAt.z = 0;
    right_tilt = glm::normalize(glm::cross(front_tilt, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    upVector_tilt = glm::normalize(glm::cross(m_right, front_tilt));
    // printf("m_lookAt = (%f, %f, %f)\n", m_lookAt.x, m_lookAt.y, m_lookAt.z);
    setCameraView(pos, front_tilt, upVector_tilt);

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
    // this->pos = final_position;

    m_eye = std::move(final_position);
    m_lookAt = std::move(getLookAt());
    m_upVector = std::move(m_upVector);

    setCameraView(m_eye, getLookAt(), m_upVector);
    printViewMatrix();
}