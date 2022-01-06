#include "box.h"

Box::Box(){};

Box::Box(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
    this->vertices = {
    0.0, 0.0, 0.0,
    0.1, 0.0, 0.0,
    0.1, 0.1, 0.0,

    0.0, 0.0, 0.0,
    0.0, 0.1, 0.0,
    0.1, 0.1, 0.0,

    0.0, 0.0, 0.0,
    0.0, 0.0, 0.1,
    0.0, 0.1, 0.1,

    0.0, 0.0, 0.0,
    0.0, 0.1, 0.0,
    0.0, 0.1, 0.1,

    0.0, 0.1, 0.1,
    0.0, 0.1, 0.0,
    0.1, 0.1, 0.0,

    0.0, 0.1, 0.1,
    0.1, 0.1, 0.1,
    0.1, 0.1, 0.0,

    0.1, 0.1, 0.1,
    0.1, 0.0, 0.1,
    0.1, 0.1, 0.0,

    0.1, 0.1, 0.0,
    0.1, 0.0, 0.1,
    0.1, 0.0, 0.0,

    0.1, 0.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.1,

    0.1, 0.0, 0.0,
    0.1, 0.0, 0.1,
    0.0, 0.0, 0.1,

    0.0, 0.0, 0.1,
    0.1, 0.0, 0.1,
    0.0, 0.1, 0.1,

    0.0, 0.1, 0.1,
    0.1, 0.1, 0.1,
    0.1, 0.0, 0.1};

    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(float));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->vertices_array.link_vbo(vertices_buffer, 0, 3);
}

void Box::Bind()
{
    this->vertices_array.Bind();
}

void Box::Unbind()
{
    this->vertices_array.Unbind();
}

void Box::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0,  vertices.size() / 3);
}

void Box::Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection)
{
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(*model));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(*projection));
    glDrawArrays(GL_TRIANGLES, 0,  vertices.size() / 3);
}
