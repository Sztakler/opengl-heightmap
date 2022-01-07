#include "heightmap.h"

Heightmap::Heightmap(const char *obj_data_filename, const char *vertex_shader_filename,
                     const char *fragment_shader_filename)
{

    // loadFromObjectFile(obj_data_filename);
    loadHGTMap(map_filename, heights, coordinates, indexes);

    // for (int i = 0; i < 9; i += 3)
    // {
    // printf("{%f, %f, %f},\n", vertices[i], vertices[i+1], vertices[i+2]);
    // }

    this->vertices_array = VAO();
    this->vertices_array.Bind();
    this->vertices_buffer = VBO(&this->vertices, this->vertices.size() * sizeof(float));

    this->shader = Shader(vertex_shader_filename, fragment_shader_filename);

    this->vertices_array.link_vbo(this->vertices_buffer, 0, 3);

    printf("\nn_verts=%d\n", vertices.size());

    // for (int i = 0; i < heights.size(); i++)
    // {
    //     printf("[i]%d [heights] %d    [coordinates] N%fE%f\n", i, heights[i], coordinates[i].latitude, coordinates[i].longitude);
    // }
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

    switch (drawing_mode)
    {
    case TRIANGLES:
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        break;

    case WIREFRAME:
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        break;
    default:
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    }
}

void Heightmap::loadData(const char *filename, std::vector<float> &data, float scale)
{
    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line))
    {
        std::string value;
        for (auto &c : line)
        {
            if (c == ',')
            {
                data.push_back(std::stof(value) * scale);
                value = "";
                continue;
            }
            value += c;
        }
    }
}

bool Heightmap::loadFromObjectFile(const char *filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
        return false;

    std::vector<vec3d> temp_vertices;
    std::vector<vec3d> temp_normals;
    vec3d vn = {0.0, 0.0, 0.0};
    vec3d v = {0.0, 0.0, 0.0};

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        std::stringstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            if (line[1] == 'n')
            {
                char delimiter[] = " ";
                std::vector<std::string> tokens;

                char *token = strtok(line, delimiter);
                while (token)
                {
                    tokens.push_back(std::string(token));
                    token = strtok(nullptr, delimiter);
                }

                vn.x = stof(tokens[1]);
                vn.y = stof(tokens[2]);
                vn.z = stof(tokens[3]);

                temp_normals.push_back(vn);
            }
            else
            {
                char delimiter[] = " ";
                std::vector<std::string> tokens;

                char *token = strtok(line, delimiter);
                while (token)
                {
                    tokens.push_back(std::string(token));
                    token = strtok(nullptr, delimiter);
                }

                v.x = stof(tokens[1]);
                v.y = stof(tokens[2]);
                v.z = stof(tokens[3]);

                temp_vertices.push_back(v);
            }
        }

        if (line[0] == 'f')
        {
            int f[3];
            int n[3];

            std::string parts[3];

            s >> junk >> parts[0] >> parts[1] >> parts[2];

            for (int i = 0; i < 3; i++)
            {
                replace(parts[i], "//", " ");

                char delimiter[] = " ";
                std::vector<std::string> numbers;

                char *token = strtok(const_cast<char *>(parts[i].c_str()), delimiter);
                while (token != nullptr)
                {
                    numbers.push_back(std::string(token));
                    token = strtok(nullptr, delimiter);
                }

                f[i] = stoi(numbers[0]);
                n[i] = stoi(numbers[1]);
            }

            for (int i = 0; i < 3; i++)
            {
                this->vertices.push_back(temp_vertices[f[i] - 1].x);
                this->vertices.push_back(temp_vertices[f[i] - 1].y);
                this->vertices.push_back(temp_vertices[f[i] - 1].z);
            }
        }
    }
    return true;
}

bool Heightmap::loadHGTMap(std::string filename, std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates, std::vector<uint> &indexes)
{
    map_loader::load_heightmap(heights, coordinates, const_cast<char *>(map_filename.c_str()));


    printf("Loaded %dx%d points.\n", heights.size(), coordinates.size());

    for (int i = 0; i < heights.size(); i++)
    {
        this->vertices.push_back((float)coordinates[i].latitude);
        this->vertices.push_back((float)coordinates[i].longitude);
        this->vertices.push_back((float)heights[i] * map_scale);
    }

    calculate_indexes(indexes);


    printf("%d\n", indexes.size());

    std::ofstream file;
    file.open("dla_bartusia.txt");

    for (uint i = 0; i < 1200*1200*6; i += 3)
    {
        // std::cout << indexes[i] << "\n";
        // printf("[%d %d %d] %d %d %d\n", i, i+1, i+2 , indexes[i], indexes[i+1], indexes[i+2]);
        file << "[" << i << "] " << std::setw(7) << indexes[i] << std::setw(7) <<  " " << indexes[i + 1] << std::setw(7) << " " << indexes[i + 2] << "\n"; 
    }
    file.close();
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

    for (uint i = 0; i < n_rows - 1; i++)
    {
        for (uint j = 0; j < n_rows - 1; j++)
        {
            indexes.push_back(i * n_rows + j);
            indexes.push_back((i + 1) * n_rows + j + 1);
            indexes.push_back(i * n_rows + j + 1);

            indexes.push_back(i * n_rows + j);
            indexes.push_back((i + 1) * n_rows + j + 1);
            indexes.push_back((i + 1) * n_rows + j);
        }
    }
    
}