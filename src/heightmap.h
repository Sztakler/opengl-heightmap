#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "includes.h"
#include "map_chunk.h"

class Heightmap
{
    public:
        std::vector<MapChunk*> map_chunks;
        std::vector<std::vector<uint32_t>*> indexes_lists;

        std::pair<int, int> latitude_range;
        std::pair<int, int> longitude_range;
        int offset;

        char *map_directory;

    public:
        Heightmap(char* map_directory, std::pair<int, int> latitude_range,
                  std::pair<int, int> longitude_range, int offset);

    public:
        void Draw(glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection, int lod, int lod_change);

    private:
        void calculate_indexes(std::vector<uint> &indexes, uint step, int n_rows);
        void populate_indexes_list();
        void load_map_chunks();
};

#endif // !HEIGHTMAP_H