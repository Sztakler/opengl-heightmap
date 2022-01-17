#ifndef MAP_CHUNK_H
#define MAP_CHUNK_H

#include "includes.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "shader.h"
#include "map_loader.h"
#include <iomanip>

class MapChunk
{
    public:
        VAO heights_array;

        VBO heights_buffer;

        EBO indexes_buffer;

        Shader shader;

        std::vector<int16_t> heights;
        std::vector<uint32_t>* indexes;

        float map_scale = 0.0001;
        int offset;
        glm::vec2 chunk_origin;

    public:
        MapChunk(const char* map_filename, const char* vertex_shader,
               const char* fragment_shader, std::vector<uint32_t>* indexes,
               std::pair<int, int> latitude_range, std::pair<int, int> longitude_range, int offset);

    public:
        void Bind();
        void Unbind();
        void Draw();
        void Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection, DRAWING_MODE drawing_mode, int lod, Shader_Mode shader_mode);

    private:
        bool replace(std::string& str, const std::string& from, const std::string& to);
        void loadData(const char* filename, std::vector<float> &data, float scale);
        bool loadFromObjectFile(const char* filename);
        bool loadHGTMap(const char* filename, std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates);
        bool loadHGTMap(const char* map_filename, std::vector<int16_t> &heights, glm::vec2 &chunk_origin,
                        std::pair<int, int> latitude_range, std::pair<int, int> longitude_range);
        void calculate_indexes(std::vector<uint> &indexes);
};

#endif