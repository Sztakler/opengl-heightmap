#ifndef EBO_H
#define EBO_H

// #include <glad/glad.h>
#include "includes.h"

class EBO
{
    public:
        GLuint id;

    public:
        EBO();
        EBO(GLuint* indices, GLsizeiptr size);
        EBO(std::vector<int>* indices, GLsizeiptr size);
        EBO(std::vector<uint32_t>* indices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};

#endif