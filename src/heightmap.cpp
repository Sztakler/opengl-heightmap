#include "heightmap.h"

Heightmap::Heightmap(){}

Heightmap::Heightmap(char* map_directory, std::pair<int, int> latitude_range,
                  std::pair<int, int> longitude_range, int offset)
{
    this->map_directory = map_directory;
    this->latitude_range = latitude_range;
    this->longitude_range = longitude_range;
    this->offset = offset;

    populate_indexes_list();
    load_map_chunks();
}

void Heightmap::calculate_indexes(std::vector<uint> &indexes, uint step, int n_rows)
{
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

void Heightmap::populate_indexes_list()
{
    for (uint32_t i = 0; i < 8; i++)
	{
		std::vector<uint32_t> *indexes = new std::vector<uint32_t>;
		indexes_lists.push_back(indexes);

		calculate_indexes(*indexes_lists[i], i+1, (int)ceil(1201.0 / offset));
		printf("[%d] %d %d\n", i, indexes_lists[i]->size(), i+1);
	}
}

void Heightmap::load_map_chunks()
{
    std::vector<std::string> subdirectories_list;
    map_loader::get_subdirectories_list(subdirectories_list, map_directory);

    if (subdirectories_list.size() == 0)
	{
		subdirectories_list.push_back(map_directory);
	}

    double load_start = glfwGetTime();
	int directory_index = 0;

	for (std::string subdirectory_name : subdirectories_list)
	{
		std::vector<std::string> mapfiles_list;
		map_loader::get_files_list_by_extension(mapfiles_list, const_cast<char *>(subdirectory_name.c_str()), "hgt", latitude_range, longitude_range);

		for (std::string mapfilename : mapfiles_list)
		{
			// std::cout << "\nCreating renderer for " << mapfilename << "\n";
			MapChunk *map_chunk = new MapChunk(mapfilename.c_str(), "shaders/heightmapECEF.vert", "shaders/heightmapECEF.frag", indexes_lists[5],
											latitude_range, longitude_range, offset);
			map_chunks.push_back(map_chunk);
		}

		directory_index++;

		std::cout << "Loaded data from " << subdirectory_name << " [" << directory_index << "/" << subdirectories_list.size() << "].\n";
	}

	double load_end = glfwGetTime();
	double load_time = load_end - load_start;

	std::cout << "Loaded " << map_chunks.size() << " chunks in " << load_time << "s [" << load_time / map_chunks.size() << "s per chunk]\n"
			  << "Used " << map_chunks.size() * ceil(1201.0 / offset) * ceil(1201.0 / offset) * 2 << " bytes [" << map_chunks.size() * ceil(1201.0 / offset) * ceil(1201.0 / offset) * 2 / 1000000 << " MB].\n";
}

void Heightmap::Draw(glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection, int lod, int lod_change, Shader_Mode shader_mode)
{
        for (MapChunk *map_chunk : map_chunks)
		{
			map_chunk->shader.Activate();
			map_chunk->Bind();
			if (lod_change)
			{
				map_chunk->indexes = indexes_lists[lod];
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_chunk->indexes_buffer.id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, map_chunk->indexes->size() * sizeof(uint32_t), &map_chunk->indexes->front(), GL_STATIC_DRAW);
			}
			map_chunk->Draw(model, view, projection, TRIANGLES, lod, shader_mode);
			map_chunk->Unbind();
		}
}