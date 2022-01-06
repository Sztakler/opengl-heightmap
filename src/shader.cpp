#include "shader.h"

std::string get_file_contents(const char* filename)
{
    std::string shader_code;
	std::ifstream shader_stream(filename, std::ios::in);
	if(shader_stream.is_open()){
		std::stringstream sstr;
		sstr << shader_stream.rdbuf();
		shader_code = sstr.str();
		shader_stream.close();

        return shader_code;
	}else{
		printf("Impossible to open %s. Are you in the right directory?\n", filename);
		getchar();
		return 0;
	}
    
}

Shader::Shader()
{}

Shader::Shader(const char* vertex_shader_file, const char* fragment_shader_file)
{
    std::string vertex_shader_code = get_file_contents(vertex_shader_file);
    std::string fragment_shader_code = get_file_contents(fragment_shader_file);

    const char* vertex_shader_source = vertex_shader_code.c_str();
    const char* fragment_shader_source = fragment_shader_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::Activate()
{
    glUseProgram(id);
}

void Shader::Delete()
{
    glDeleteProgram(id);
}

