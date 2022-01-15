#ifndef ARCBALL_CAMERA_H
#define ARCBALL_CAMERA_H

#include "includes.h"

class ArcballCamera
{
    private:
        glm::mat4x4 m_viewMatrix;
        glm::vec3 m_eye;            // Camera position in 3D space.
        glm::vec3 m_lookAt;         // Point that the camera is looking at.
        glm::vec3 m_upVector;       // Camera's orientation.
        glm::vec3 world_up;         // Camera's orientation.
        glm::vec3 m_right;

        float yaw;
        float pitch;
        float zoom;

        float mouse_sensitivity;

    public:
        /* Default constructor for arcball camera. */
        ArcballCamera();
        /* Constructor for arcball camera.
        *
        * eye -- camera position in 3D space.
        * lookAt -- point that the camera is looking at.
        * upVector -- camera's orientation (vector of it's 'up' direction).
        */
        ArcballCamera(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 upVector, float yaw = -90.0f, float pitch = 0.0f);

    public:
        glm::mat4x4 getViewMatrix() const;
        glm::vec3 getEye() const;
        glm::vec3 getUpVector() const;
        glm::vec3 getLookAt() const;

        // Camera forward axis is defined as -z
        glm::vec3 getViewDirection() const;
        glm::vec3 getRightVector() const;

        void processKeyboard(float x_offset, float y_offset);
        void processMouseMovement(float x_offset, float y_offset, int viewportWidth, int viewportHeight);
        void processMouseScroll(float y_offset);

        void setCameraView(glm::vec3 eye, glm::vec3 look_at, glm::vec3 up);
        void updateViewMatrix();
        void updateCameraVectors();
};

#endif