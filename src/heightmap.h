#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "includes.h"
#include "vao.h"
#include "vbo.h"
#include "shader.h"
#include "map_loader.h"
#include <iomanip>

class Heightmap
{
    public:
        VAO vertices_array;
        
        VBO vertices_buffer;

        Shader shader;

        std::vector<float> vertices;

        std::vector<uint16_t> heights;
        std::vector<uint32_t> indexes;
        std::vector<coordinate_t> coordinates;


        std::string map_filename = "maps/test/n48e013.hgt";
        float map_scale = 0.1;
        

    public:
        Heightmap(const char* obj_data_filename, const char* vertex_shader_filename,
               const char* fragment_shader_filename);

    public:
        void Bind();
        void Unbind();
        void Draw();
        void Draw(glm::mat4* model, glm::mat4* view, glm::mat4* projection, DRAWING_MODE drawing_mode, bool transparent, glm::vec3 camera_position);

    private:
        bool replace(std::string& str, const std::string& from, const std::string& to);
        void loadData(const char* filename, std::vector<float> &data, float scale);
        bool loadFromObjectFile(const char* filename);
        bool loadHGTMap(std::string filename, std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates, std::vector<uint> &indexes);
        void calculate_indexes(std::vector<uint> &indexes);
};

#endif