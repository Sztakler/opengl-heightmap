#ifndef OPENGL_HEIGHTMAP_H
#define OPENGL_HEIGHTMAP_H

#include <iostream>
#include <math.h>
#include <time.h>

#include "includes.h"

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "camera.h"
#include "arcball_camera.h"
#include "heightmap.h"
#include "map_chunk.h"
#include "drawable.h"
#include "sphere.h"

#include <GLFW/glfw3.h>

static bool first_mouse_movement = true;
static float delta_time;
static float last_change;

static int SCR_WIDTH = 1920;
static int SCR_HEIGHT = 1080;

static float last_x = SCR_WIDTH / 2;
static float last_y = SCR_HEIGHT / 2;

static float fov = 45.0f;

static float pitch = 0.0f;
static float yaw = -90.0f;

static int lod = 0;
static int lod_change = 0;
static int lod_auto = 1;

static GLFWwindow *window;
static ArcballCamera arcball_camera;
static Heightmap heightmap;
static Sphere globe;

static glm::mat4 model = glm::mat4(1.0f);
static glm::mat4 view = glm::mat4(1.0f);
static glm::mat4 projection = glm::mat4(1.0f);

static float rotation = 0.0f;
static double prev_time = glfwGetTime();

        // float deltaTime = 0.0f;	// Time between current frame and last frame
static float lastFrame = 0.0f; // Time of last frame

static unsigned int counter = 0;

class OpenGLHeightmap
{
    private:

        // glfwSwapInterval(0); // disables VSync -- unlimited FPS

    public:
        OpenGLHeightmap(int window_width, int window_height, const char* window_title,
            char* map_directory, std::pair<int, int> latitude_range, std::pair<int, int> longitude_range,
            int offset);

    public:
        int Initialize();
        int Render();
        void Update();
        void Draw();

    private:
        static void windowResizeHandler(GLFWwindow *window, int width, int height);
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
        static void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
        static void processInput(GLFWwindow *window);
};

#endif // !OPENGL_HEIGHTMAP_H