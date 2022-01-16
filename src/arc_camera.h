#ifndef ARC_CAMERA_H
#define ARC_CAMERA_H

/* Based on https://learnopengl.com/Getting-started/Camera */

#include "includes.h"



class ArcCamera
{
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 look_at_point;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        // std::vector<Camera_Movement> previous_moves;

        float yaw;
        float pitch;
        float movement_speed;
        float mouse_sensitivity;
        float zoom;

    public:
        ArcCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 look_at = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    public:
        glm::mat4 getViewMatrix();
        void processKeyboard(float x_offset, float y_offset, int viewportWidth, int viewportHeight);
        void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
        void processMouseScroll(float y_offset);
        // void undoMove(float delta_time);

    private:
        void updateCameraVectors();
};

#endif