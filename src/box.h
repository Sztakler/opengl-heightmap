#include "includes.h"
#include "vao.h"
#include "vbo.h"
#include "shader.h"

class Box
{
    public:
        VAO vertices_array;
        VBO vertices_buffer;

        Shader shader;

        std::vector<float> vertices;


    public:
        Box();
        Box(const char* vertex_shader_filename, const char* fragment_shader_filename);

    public:
        void Bind();
        void Unbind();
        void Draw();
        void Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection);
};