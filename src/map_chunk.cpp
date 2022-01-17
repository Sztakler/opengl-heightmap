#include "map_chunk.h"

MapChunk::MapChunk(const char *map_filename, const char *vertex_shader_filename,
                     const char *fragment_shader_filename, std::vector<uint32_t>* indexes,
                     std::pair<int, int> latitude_range, std::pair<int, int> longitude_range, int offset)
{
    this->offset = offset;
    loadHGTMap(map_filename, this->heights, this->chunk_origin, latitude_range, longitude_range);
    this->indexes = indexes;

    this->heights_array = VAO();
    this->heights_array.Bind();
    this->heights_buffer = VBO(&this->heights, this->heights.size() * sizeof(int16_t));

    this->indexes_buffer = EBO(this->indexes, this->indexes->size() * sizeof(uint32_t));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->heights_array.link_vbo(this->heights_buffer, 0, 1, GL_SHORT);
}


void MapChunk::Bind()
{
    this->heights_array.Bind();
    this->heights_buffer.Bind();
    this->indexes_buffer.Bind();
}

void MapChunk::Unbind()
{
    this->heights_array.Unbind();
    this->heights_buffer.Unbind();
    this->indexes_buffer.Unbind();
}

void MapChunk::Draw()
{
    glDrawArrays(GL_TRIANGLE_FAN, 0, heights.size());
}

void MapChunk::Draw(glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection, DRAWING_MODE drawing_mode, int lod)
{
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes_buffer.id);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexes[3]->size(), this->indexes[3], GL_STATIC_DRAW);


    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(*model));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(*projection));
    glUniform2f(3, chunk_origin.x, chunk_origin.y);
    glUniform1i(4, (int)ceil(1201.0 / offset));
    // printf("lod = %d, size = %d\n", lod, indexes[lod]->size());
    switch (drawing_mode)
    {
    case TRIANGLES:
            // printf("DRAW TRIANGLES %d indexes\n", indexes->size());
            glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, 0);
        break;

    case WIREFRAME:
        glDrawElements(GL_LINES, indexes->size(), GL_UNSIGNED_INT, 0);
        break;
    default:
        glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, 0);
    }
}

bool MapChunk::loadHGTMap(const char* map_filename, std::vector<int16_t> &heights, glm::vec2 &chunk_origin,
                           std::pair<int, int> latitude_range, std::pair<int, int> longitude_range)
{
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

    int latitude = atoi(fname.substr(1, 2).c_str());
    int longitude = atoi(fname.substr(4, 3).c_str());

    if (!fname.substr(0, 1).compare("S"))
        latitude = -latitude;

    if (!fname.substr(3, 1).compare("W"))
        longitude = -longitude;


    chunk_origin = glm::vec2((float)(latitude+1), (float)longitude); // add 1, because latitude from filename is actually lower left corner of square

    map_loader::load_data_from_file(heights, const_cast<char*>(map_filename), this->offset);

    return true;
}

bool MapChunk::replace(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void MapChunk::calculate_indexes(std::vector<uint> &indexes)
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