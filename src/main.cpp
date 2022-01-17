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

void calculate_indexes(std::vector<uint> &indexes, uint step, int n_rows)
{
	for (uint i = 0; i < n_rows - step; i += step)
	{
		for (uint j = 0; j < n_rows - step; j += step)
		{
			// printf("i=%d j=%d\n", i, j);
			indexes.push_back(i * n_rows + j);
			indexes.push_back((i + step) * n_rows + j + step);
			indexes.push_back(i * n_rows + j + step);

			indexes.push_back(i * n_rows + j);
			indexes.push_back((i + step) * n_rows + j + step);
			indexes.push_back((i + step) * n_rows + j);
		}
	}
}

void windowResizeHandler(GLFWwindow *window, int width, int height)
{
	const float aspectRatio = (float)width / height;
	float sx = aspectRatio > 1.0f ? aspectRatio : 1.0f;
	float sy = aspectRatio > 1.0f ? 1.0f : 1.0f / aspectRatio;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-sx, sx, -sy, sy, 1, -1);

	glViewport(0, 0, width, height);
}

bool first_mouse_movement = true;
float delta_time;
float last_change;

int SCR_WIDTH = 1920;
int SCR_HEIGHT = 1080;

float last_x = SCR_WIDTH / 2;
float last_y = SCR_HEIGHT / 2;

float fov = 45.0f;

float pitch = 0.0f;
float yaw = -90.0f;

int lod = 0;
int lod_change = 0;
int lod_auto = 1;

DRAWING_MODE drawing_mode = TRIANGLES;

Camera player_camera(glm::vec3(5.0f, 0.0f, 15.0f));
ArcballCamera arcball_camera(glm::vec3(3.0, 12.0, 8.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 90.0f, 0.0f);
Camera static_camera(glm::vec3(-20.0f, 10.0f, 20.0f));
Camera *current_camera = &player_camera;
CAMERA camera_index = PLAYER_CAMERA;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
	if (first_mouse_movement)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse_movement = false;
	}

	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	player_camera.processMouseMovement(x_offset, y_offset);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		arcball_camera.processMouseRotation(-x_offset, y_offset, SCR_WIDTH, SCR_HEIGHT);
		// arc_camera.processKeyboard(x_offset, y_offset, SCR_WIDTH, SCR_HEIGHT);
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		arcball_camera.processMouseTilt(x_offset, y_offset);
		// arc_camera.processMouseMovement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
	current_camera->processMouseScroll(y_offset);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		lod = 1;
		lod_change = 1;
		lod_auto = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		lod = 2;
		lod_change = 1;
		lod_auto = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		lod = 3;
		lod_change = 1;
		lod_auto = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		lod = 4;
		lod_change = 1;
		lod_auto = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		lod = 5;
		lod_change = 1;
		lod_auto = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		lod = 6;
		lod_change = 1;
		lod_auto = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		lod = 7;
		lod_change = 1;
		lod_auto = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		lod = 4;
		lod_change = 1;
		lod_auto = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player_camera.processKeyboard(FORWARD, delta_time);
		arcball_camera.processMouseRotation(0.0f, 5.0f, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player_camera.processKeyboard(BACKWARD, delta_time);
		arcball_camera.processMouseRotation(0.0f, -5.0f, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player_camera.processKeyboard(LEFT, delta_time);
		arcball_camera.processMouseRotation(-5.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player_camera.processKeyboard(RIGHT, delta_time);
		arcball_camera.processMouseRotation(5.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		player_camera.processKeyboard(UP, delta_time);
		arcball_camera.processKeyboard(UP, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		player_camera.processKeyboard(DOWN, delta_time);
		arcball_camera.processKeyboard(DOWN, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		player_camera.processMouseMovement(0.0, 10.0);
		arcball_camera.processMouseRotation(0.0, 10., SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		player_camera.processMouseMovement(0.0, -10.0);
		arcball_camera.processMouseRotation(0.0, -10, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		player_camera.processMouseMovement(-10.0, 0.0);
		arcball_camera.processMouseRotation(-10.0, 0.0, SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		player_camera.processMouseMovement(10.0, 0.0);
		arcball_camera.processMouseRotation(10.0, 0.0, SCR_WIDTH, SCR_HEIGHT);
	}
	if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && (last_change > 0.3))
	{
		if (drawing_mode == TRIANGLES)
			drawing_mode = WIREFRAME;
		else
			drawing_mode = TRIANGLES;

		last_change = 0.0;
	}

	if ((glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) && (last_change > 0.3))
	{
		switch (camera_index)
		{
		case PLAYER_CAMERA:
			current_camera = &static_camera;
			camera_index = STATIC_CAMERA;
			break;
		case STATIC_CAMERA:
			current_camera = &player_camera;
			camera_index = PLAYER_CAMERA;
			break;
		default:
			current_camera = &static_camera;
			camera_index = STATIC_CAMERA;
			break;
		}

		last_change = 0.0;
		std::cout << "CURRENT CAMERA = " << camera_index << std::endl;
	}
}

int main(int argc, char *argv[])
{
	char *map_directory;
	std::pair<int, int> latitude_range = {1, 0};
	std::pair<int, int> longitude_range = {1, 0};
	int offset = 1;

	printf("argc = %d\n", argc);

	if (argc == 2)
	{
		// seed = atoi(argv[1]);
		// board_size = 10;
		map_directory = argv[1];
	}
	else if (argc == 3)
	{
		map_directory = argv[1];
		offset = atoi(argv[2]);
	}
	else if (argc == 9)
	{
		map_directory = argv[1];
		offset = atoi(argv[2]);
		latitude_range.first = atoi(argv[4]);
		latitude_range.second = atoi(argv[5]);

		longitude_range.first = atoi(argv[7]);
		longitude_range.second = atoi(argv[8]);
	}
	else
	{
		map_directory = "maps";
	}

	for (int i = 0; i < argc; i++)
	{
		printf("%s offset=%d ", argv[i], offset);
	}
	printf("\n");

	// Initialize GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);			   // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGl

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "heightmap"
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Heightmap-OpenGL", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hides mouse cursor and captures it
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the player_camera than the former one
	glDepthFunc(GL_LESS);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	// glFrontFace(GL_CCW);

	Heightmap heightmap(map_directory, latitude_range, longitude_range, offset);

	Sphere globe(10.0f, 50, 50);

	globe.position = glm::vec3(0.0, 0.0, 0.0);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float rotation = 0.0f;
	double prev_time = glfwGetTime();

	// float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	player_camera.front = glm::vec3(0, 0, 0);

	unsigned int counter = 0;
	// glfwSwapInterval(0); // disables VSync -- unlimited FPS

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{

		float currentFrame = glfwGetTime();
		delta_time = currentFrame - lastFrame;
		last_change += delta_time;
		counter++;

		float fps = (1.0 / delta_time) * counter;

		if (delta_time >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string(fps);
			std::string ms = std::to_string((delta_time / counter) * 1000);
			std::string newTitle = "Heightmap-OpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			counter = 0;
			lastFrame = currentFrame;
		}

		processInput(window);

		if (fps < 10.0 && lod_auto)
		{
			if (lod + 1 <= 7)
			{
				lod++;
				lod_change = 1;
			}
			printf("autolod lod=%d\n", lod);
		}
		if (fps > 59.0 && lod_auto)
		{
			if (lod - 1 >= 0)
			{
				lod--;
				lod_change = 1;
			}
			printf("autolod lod=%d\n", lod);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.07f, 0.07f, 0.07f, 1.0f);

		view = arcball_camera.getViewMatrix();
		projection = glm::perspective(glm::radians(player_camera.zoom), (float)(SCR_WIDTH) / (float)SCR_HEIGHT, 0.01f, 100.0f);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		heightmap.Draw(&model, &view, &projection, lod, lod_change);
		lod_change = 0;

		globe.shader.Activate();
		globe.Bind();
		globe.Draw(&model, &view, &projection, TRIANGLES);
		globe.Unbind();

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}