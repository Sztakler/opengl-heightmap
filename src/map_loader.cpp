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
    void load_heightmap(std::vector<int16_t> &vertices, char *filename, int offset)
    {
        std::string line;
        std::string word;


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
        double longitude = (double)atoi(fname.substr(4, 6).c_str());


        std::ifstream in_file_stream(filename, std::ios::in | std::ios::binary);

        if (!in_file_stream)
        {
            std::cout << "\033[91mCouldn't open file " << filename << '\n\033[0m';
            return;
        }

        // std::cout << "Opened file " << filename << '\n';

        in_file_stream.seekg(0, in_file_stream.end);
        int file_length = in_file_stream.tellg();
        in_file_stream.seekg(0, in_file_stream.beg);

        // printf("Creating buffer of size %d bytes.\n", file_length);

        std::vector<unsigned char> bytes(file_length, 0);

        // printf("\033[92mBuffer created successfully.\033[0m\nReading %d characters...\n", file_length);

        in_file_stream.read((char *)&bytes[0], bytes.size());
        if (in_file_stream)
            ;
            // printf("\033[92mAll characters read successfully.\n\033[0m");
        else
            printf("\033[91merror: only %ld could be read.\n\033[0m", in_file_stream.gcount());

        in_file_stream.close();

        // std::cout << fname << " " << fname.substr(0, 1) << " " << fname.substr(3, 1).compare("E") << "\n";
        // std::string longitude_letter = fname.substr

        int16_t rows = 0;
        int16_t cols = 0;

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


        for (int16_t i = 0; i < rows; i += offset)
        {
            for (int16_t j = 0; j < cols; j += 2 * offset)
            {
                int k = cols * i + j;

                uint8_t byte_high = bytes[k];
                uint8_t byte_low = bytes[k + 1];
                int16_t height = (byte_high << 8) + byte_low;

                vertices.push_back(height);
            }
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

    void get_files_list_by_extension(std::vector<std::string> &files_list, char *directory_name, char *extension,
                                     std::pair<int, int> latitude_range, std::pair<int, int> longitude_range)
    {
        /* Copy original string, because strtok may destroy it. */
        char dirname[50];
        strcpy(dirname, directory_name);
        /* Check whether this isn't directory but already a file. */
        char *ext = strtok(dirname, ".");
        ext = strtok(NULL, ".");

        if (ext && strcmp(extension, ext) == 0)
        {
            std::string path(directory_name);
            std::cout << path << "\n";
            files_list.push_back(path);
            return; // If directory_name is a file, then it's the only file to add to list.
        }

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
            if (ext && strcmp(extension, ext) == 0)
            {
                std::string path(directory_name);
                path.append("/");
                path.append(entry->d_name);

                std::string fname(filename);
                int latitude = atoi(fname.substr(1, 2).c_str());
                int longitude = atoi(fname.substr(4, 3).c_str());

                if (!fname.substr(0, 1).compare("S"))
                    latitude = -latitude;
                if (!fname.substr(3, 1).compare("W"))
                    longitude = -longitude;

                // printf(" chunk %d %d    -lat %d %d -lon %d %d\n", latitude, longitude, latitude_range.first, latitude_range.second, longitude_range.first, longitude_range.second);

                /* If user defined latitude range then discard data out of this range. */
                if (latitude_range.first <= latitude_range.second)
                {
                    if ( latitude < latitude_range.first || latitude > latitude_range.second)
                    {
                        continue;
                    }
                }

                /* If user defined longitude range then discard data out of this range. */
                if (longitude_range.first <= longitude_range.second)
                {
                    if ( longitude < longitude_range.first || longitude > longitude_range.second )
                    {
                        continue;
                    }
                }

                files_list.push_back(path);
            }
        }
        closedir(directory);
    }
}