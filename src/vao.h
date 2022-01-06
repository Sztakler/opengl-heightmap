#ifndef VAO_H
#define VAO_H

// #include <glad/glad.h>
#include "includes.h"
#include "vbo.h"

class VAO
{
    public:
        GLuint id;
        
    public:
        VAO();

        void link_vbo(VBO vbo, GLuint layout, GLuint size);
        void link_instance_vbo(VBO vbo, GLuint layout, GLuint size, GLuint divisor);
        void Bind();
        void Unbind();
        void Delete();
};

#endif