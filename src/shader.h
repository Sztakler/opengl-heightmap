#ifndef SHADER_H
#define SHADER_H

#include "includes.h"
#include <string.h>
#include <fstream>

std::string get_file_contents(const char* filename);

class Shader
{
    public:
        GLuint id;
        
    public:
        Shader();
        Shader(const char* vertex_shader_file, const char* fragment_shader_file);

        void Activate();
        void Delete();

};

# endif