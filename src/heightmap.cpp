#include "heightmap.h"

Heightmap::Heightmap(const char *map_filename, const char *vertex_shader_filename,
                     const char *fragment_shader_filename, std::vector<uint32_t>* indexes)
{
    printf("%p %p\n", indexes, this->indexes);
    loadHGTMap(map_filename, heights, coordinates);
    this->indexes = indexes;
    printf("%p %p\n", indexes, this->indexes);


    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(float));

    this->indexes_buffer = EBO(this->indexes, this->indexes->size() * sizeof(uint32_t));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->vertices_array.link_vbo(this->vertices_buffer, 0, 3);

    printf("\nn_verts=%d n_indexes=%d\n", vertices.size(), this->indexes->size());
}

Heightmap::Heightmap(const char *map_filename, const char *vertex_shader_filename,
                     const char *fragment_shader_filename)
{
    loadHGTMap(map_filename, heights, coordinates);
    this->indexes = indexes;

    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(float));

    this->indexes_buffer = EBO(this->indexes, this->indexes->size() * sizeof(uint32_t));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->vertices_array.link_vbo(this->vertices_buffer, 0, 3);

    printf("\nn_verts=%d n_indexes=%d\n", vertices.size(), indexes->size());
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

    glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, 0);
    // switch (drawing_mode)
    // {
    // case TRIANGLES:
    //     glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    //     break;

    // case WIREFRAME:
    //     glDrawArrays(GL_LINES, 0, vertices.size() / 3);
    //     break;
    // default:
    //     glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    // }
}

bool Heightmap::loadHGTMap(const char* map_filename, std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates)
{
    map_loader::load_heightmap(heights, coordinates, const_cast<char*>(map_filename));
// nic

    printf("Loaded %dx%d points.\n", heights.size(), coordinates.size());

    for (int i = 0; i < heights.size(); i++)
    {
        this->vertices.push_back((float)coordinates[i].latitude);

        /* Clamp incorrect height data to 0. */
        if (heights[i] > 8000)
            this->vertices.push_back(0.0);
        else
            this->vertices.push_back((float)heights[i] * map_scale);
        
        
        this->vertices.push_back((float)coordinates[i].longitude);
    }
    printf("Loaded %d points to vertices.\n", vertices.size());

    // calculate_indexes(indexes);


    // printf("n_indexes = %d\n", indexes->size());
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

    // for (uint i = 0; i < n_rows - 1; i++)
    // {
    //     for (uint j = 0; j < n_rows - 1; j++)
    //     {
    //         indexes.push_back(i * n_rows + j);
    //         indexes.push_back((i + 1) * n_rows + j + 1);
    //         indexes.push_back(i * n_rows + j + 1);

    //         indexes.push_back(i * n_rows + j);
    //         indexes.push_back((i + 1) * n_rows + j + 1);
    //         indexes.push_back((i + 1) * n_rows + j);
    //     }
    // }
    
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