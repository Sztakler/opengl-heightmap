#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "includes.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shader.h"
#include "map_loader.h"
#include <iomanip>

class Heightmap
{
    public:
        VAO vertices_array;
        
        VBO vertices_buffer;

        EBO indexes_buffer;

        Shader shader;

        // std::vector<float> vertices;
        std::vector<int16_t> vertices;
        // std::vector<int16_t> heights;
        // std::vector<uint32_t> indexes;
        std::vector<uint32_t>* indexes;
        // std::vector<coordinate_t> coordinates;

        float map_scale = 0.0001;
        glm::vec2 chunk_origin;
        
    public:
        Heightmap(const char* obj_data_filename, const char* vertex_shader_filename,
               const char* fragment_shader_filename);
        Heightmap(const char* obj_data_filename, const char* vertex_shader_filename,
               const char* fragment_shader_filename, std::vector<uint32_t>* indexes);

    public:
        void Bind();
        void Unbind();
        void Draw();
        void Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection, DRAWING_MODE drawing_mode, bool transparent, glm::vec3 camera_position);

    private:
        bool replace(std::string& str, const std::string& from, const std::string& to);
        void loadData(const char* filename, std::vector<float> &data, float scale);
        bool loadFromObjectFile(const char* filename);
        bool loadHGTMap(const char* filename, std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates);
        bool loadHGTMap(const char* filename, std::vector<int16_t> &vertices, glm::vec2 &chunk_origin);
        void calculate_indexes(std::vector<uint> &indexes);
};

#endif