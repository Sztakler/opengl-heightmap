#include "heightmap.h"

Heightmap::Heightmap(const char *map_filename, const char *vertex_shader_filename,
                     const char *fragment_shader_filename, std::vector<uint32_t>* indexes)
{
    loadHGTMap(map_filename, this->vertices, this->chunk_origin);
    this->indexes = indexes;


    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(int16_t));

    this->indexes_buffer = EBO(this->indexes, this->indexes->size() * sizeof(uint32_t));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->vertices_array.link_vbo(this->vertices_buffer, 0, 3, GL_SHORT);
}


void Heightmap::Bind()
{
    this->vertices_array.Bind();
}

void Heightmap::Unbind()
{
    this->vertices_array.Unbind();
}

void Heightmap::Draw()
{
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 3);
}

void Heightmap::Draw(glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection, DRAWING_MODE drawing_mode, bool transparent, glm::vec3 camera_position)
{
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(*model));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(*projection));
    glUniform2f(3, chunk_origin.x, chunk_origin.y);

    switch (drawing_mode)
    {
    case TRIANGLES:
            glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, 0);
        break;

    case WIREFRAME:
        glDrawElements(GL_LINES, indexes->size(), GL_UNSIGNED_INT, 0);
        break;
    default:
        glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, 0);
    }
}

bool Heightmap::loadHGTMap(const char* map_filename, std::vector<int16_t> &vertices, glm::vec2 &chunk_origin)
{
    map_loader::load_heightmap(vertices, const_cast<char*>(map_filename));

    char filename_copy[50];
    // std::cout << "Map " << map_filename << "\n";
    strcpy(filename_copy, map_filename);
    const char delim[2] = "/";

    char *token;
    std::vector<char *> tokens;
    token = strtok(filename_copy, delim);

    while (token != NULL)
    {
        tokens.push_back(token);
        token = strtok(NULL, delim);
    }

    std::string fname(tokens.back());

    float latitude = (float)atoi(fname.substr(1, 2).c_str()) + 1; // add 1, because latitude from filename is actually lower left corner of square
    float longitude = (float)atoi(fname.substr(4, 6).c_str());

    chunk_origin = glm::vec2(latitude, longitude);

    printf("\033[92mLoaded %ld/%ld points to vertices [%ld bytes].\n\033[0m", vertices.size(), 1201*1201*3, vertices.size() * sizeof(int16_t));
}

bool Heightmap::replace(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void Heightmap::calculate_indexes(std::vector<uint> &indexes)
{
    int n_rows = 1201;
    int step = 10;

    for (uint i = 0; i < n_rows - step; i += step)
    {
        for (uint j = 0; j < n_rows - step; j += step)
        {
            indexes.push_back(i * n_rows + j);
            indexes.push_back((i + step) * n_rows + j + step);
            indexes.push_back(i * n_rows + j + step);

            indexes.push_back(i * n_rows + j);
            indexes.push_back((i + step) * n_rows + j + step);
            indexes.push_back((i + step) * n_rows + j);
        }
    }
}