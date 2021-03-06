#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "includes.h"
#include <string.h>
#include <inttypes.h>
#include <dirent.h>
#include <unistd.h>

namespace map_loader {
    /* Populates 'heights' vector with heights (in meters above sea level) from .hgt file 'filename'.
    *
    * filename -- parameter is either relative or absolute path to .hgt file.
    * offset -- specifies number of points to omit, while loading data (useful for large maps).
    */
    void load_data_from_file(std::vector<int16_t> &heights, char* filename, int offset);

    /* Populates 'directories_list' vector with paths to directories inside 'directory_name'. */
    void get_subdirectories_list(std::vector<std::string> &directories_list, char* directory_name);

    /* Populates 'files_list' vector with paths to files inside 'directory_name' of given 'extension'.
    * 'extension' needs to be provided withouth ".", e.g. "txt".
    */
    void get_files_list_by_extension(std::vector<std::string> &files_list, char *directory_name, char *extension,
                                     std::pair<int, int> latitude_range, std::pair<int, int> longitude_range);
}
#endif