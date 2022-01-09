#include "map_loader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <inttypes.h>
#include <vector>
#include <dirent.h>
#include <unistd.h>

namespace map_loader
{
    void load_heightmap(std::vector<uint16_t> &heights, std::vector<coordinate_t> &coordinates, char *filename)
    {
        std::string line;
        std::string word;

        std::ifstream in_file_stream(filename, std::ios::in | std::ios::binary);

        if (!in_file_stream)
        {
            std::cout << "Couldn't open file " << filename << '\n';
            return;
        }

        std::cout << "Opened file " << filename << '\n';

        in_file_stream.seekg(0, in_file_stream.end);
        int file_length = in_file_stream.tellg();
        in_file_stream.seekg(0, in_file_stream.beg);

        printf("Creating buffer of size %d bytes...", file_length);

        std::vector<unsigned char> bytes(file_length, 0);

        printf(" buffer created successfully.\nReading %d characters... ", file_length);

        in_file_stream.read((char *)&bytes[0], bytes.size());
        if (in_file_stream)
            printf("all characters read successfully.\n");
        else
            printf("error: only %ld could be read.", in_file_stream.gcount());

        in_file_stream.close();

        char filename_copy[50];
        strcpy(filename_copy, filename);
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

        double latitude = (double)atoi(fname.substr(1, 2).c_str()) + 1; // add 1, because latitude from filename is actually lower left corner of square
        double origin_longitude = (double)atoi(fname.substr(4, 6).c_str());

        double longitude = origin_longitude;

        std::cout << fname << " N" << latitude << " E" << longitude << "\n";

        uint rows = 0;
        uint cols = 0;

        if (file_length == 2 * 1201 * 1201)
        {
            rows = 1201;
            cols = 2 * 1201;
        }
        else if (file_length == 2 * 3601 * 3601)
        {
            rows = 3601;
            cols = 3601 * 2;
        }

        double delta = 1.0 / rows;

        for (uint i = 0; i < rows; i++)
        {
            for (uint j = 0; j < cols; j += 2)
            {
                uint k = cols * i + j;

                uint8_t byte_high = bytes[k];
                uint8_t byte_low = bytes[k + 1];
                uint16_t height = (byte_high << 8) + byte_low;

                heights.push_back(height);

                coordinate_t coord = {.latitude = latitude, .longitude = longitude};
                coordinates.push_back(coord);

                longitude += delta;
            }

            latitude -= delta;
            longitude = origin_longitude;
        }
    }

    void get_subdirectories_list(std::vector<std::string> &directories_list, char *directory_name)
    {
        DIR *directory;

        struct dirent *dir;

        directory = opendir(directory_name);
        if (!directory)
        {
            std::cout << "Couldn't open directory " << directory_name << '\n';
            return;
        }

        while ((dir = readdir(directory)) != NULL)
        {
            /* If directory is not . (current directory) or .. (parent directory) */
            if (dir->d_name[0] != '.')
            {
                std::string dirname(directory_name);
                dirname.append("/");
                dirname.append(dir->d_name);
                directories_list.push_back(dirname);
            }
        }
        closedir(directory);
    }

    void get_files_list_by_extension(std::vector<std::string> &files_list, char *directory_name, char *parent_directory, char *extension)
    {
        DIR *directory = opendir(directory_name);
        struct dirent *entry;

        if (!directory)
        {
            std::cout << "Couldn't open directory " << directory_name << '\n';
            return;
        }
        while ((entry = readdir(directory)) != NULL)
        {
            /* Copy original string, because strtok may destroy it. */
            char filename[50];
            strcpy(filename, entry->d_name);
            /* Omit . and .. directories. */
            if (filename[0] == '.')
                continue;

            char *ext = strtok(filename, ".");
            ext = strtok(NULL, ".");
            if (strcmp(extension, ext) == 0)
            {
                // std::cout << entry->d_name << '\n';
                std::string path(directory_name);
                path.append("/");
                path.append(entry->d_name);
                // std::cout << path << "\n";
                files_list.push_back(path);
            }
        }
        closedir(directory);
    }
}