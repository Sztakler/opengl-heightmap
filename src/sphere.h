#include "includes.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shader.h"

class Sphere
{
    public:
        float radius;
        int n_stacks;
        int n_sectors;

        std::vector<float> vertices;
        std::vector<int> indices;

        VAO vertices_array;
        VBO vertices_buffer;
        EBO indices_buffer;

        Shader shader;

        glm::vec3 position;


    public:
        Sphere(float radius, int n_stacks, int n_sectors);

    public:
        void Bind();
        void Unbind();
        void Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection, DRAWING_MODE drawing_mode);

    private:
        void generate_vertices();
        void generate_indices();
};