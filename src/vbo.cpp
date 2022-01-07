#include "vbo.h"

VBO::VBO()
{}

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO(std::vector<float>* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, &vertices->front(), GL_STATIC_DRAW);
}

VBO::VBO(std::vector<double>* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, &vertices->front(), GL_STATIC_DRAW);
}

VBO::VBO(std::vector<coordinate_t>* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, &vertices->front(), GL_STATIC_DRAW);
}

VBO::VBO(std::vector<glm::vec2>* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, &vertices->front(), GL_STATIC_DRAW);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Update(std::vector<float>* vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), &vertices->front(), GL_STATIC_DRAW);
}

void VBO::Delete()
{
    glDeleteBuffers(1, &id);
}