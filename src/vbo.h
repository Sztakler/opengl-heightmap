#ifndef VBO_H
#define VBO_H

// #include <glad/glad.h>
#include "includes.h"

class VBO
{
    public:
        GLuint id;

    public:
        VBO();
        VBO(GLfloat* vertices, GLsizeiptr size);
        VBO(std::vector<float>* vertices, GLsizeiptr size);
        VBO(std::vector<double>* vertices, GLsizeiptr size);
        VBO(std::vector<coordinate_t>* vertices, GLsizeiptr size);
        VBO(std::vector<glm::vec2>* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Update(std::vector<float>* vertices);
        void Delete();
};

#endif