#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <inttypes.h>
#include <vector>
#include <dirent.h>
#include <unistd.h>

char* returnnull()
{
    return nullptr;
}

/* Populates 'heights' vector with heights (in meters above sea level) from .hgt file 'filename'. 
 * 'filename' parameter is either relative or absolute path to .hgt file.
 */
// void load_heightmap(std::vector<std::vector<uint16_t>> &heights, char* filename)
void load_heightmap(std::vector<uint16_t> &heights, char* filename)
{
    std::string line;
    std::string word;
    // int height = 0;

    std::ifstream in_file_stream(filename, std::ios::in | std::ios::binary);
    
    if (!in_file_stream)
    {
        std::cout << "Couldn't open file " << filename;
        return;
    }

    in_file_stream.seekg(0, in_file_stream.end);
    int file_length = in_file_stream.tellg();
    in_file_stream.seekg(0, in_file_stream.beg);

    printf("Creating buffer of size %d bytes...", file_length);

    // char* buffer = new char[file_length];
    std::vector<unsigned char> bytes(file_length, 0);

    printf(" buffer created successfully.\nReading %d characters... ", file_length);
    // in_file_stream.read(buffer, file_length);
    in_file_stream.read((char*)&bytes[0], bytes.size());
    if (in_file_stream)
        printf("all characters read successfully.\n");
    else
        printf("error: only %ld could be read.", in_file_stream.gcount());
    
    in_file_stream.close();

    printf("dupa\n");
    char filename_copy[50];
    strcpy(filename_copy, filename);
    const char delim[2] = "/";
    // std::string fname(filename);
    // std::cout << filename_copy << "\n";

    // char* token;

    // token = strtok(filename_copy, delim);
    // std::cout << token << " ";
    
    // while (token != NULL)
    // {
    //     token = strtok(NULL, delim);
    //     std::cout << token << " ";
    // }

    // std::cout.clear();
    // std::cout << "dupa2\n";

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
    
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j += 2)
        {
            uint k = cols * i + j;

            uint8_t byte_high = bytes[k];
            uint8_t byte_low = bytes[k + 1];
            uint16_t height = (byte_high << 8) + byte_low;

            heights.push_back(height);
        }
    }

    // for (int i = 0; i < file_length; i += 2)
    // {
    //     // printf("%d %d\n", bytes[i], bytes[i+1]);
    //     uint8_t byte_high = bytes[i];
    //     uint8_t byte_low = bytes[i + 1];
    //     uint16_t height = (byte_high << 8) + byte_low;

    //     // if (height > 5000)
    //     //     printf("\033[92m%d %d %d %d %d\033[0m\n", height, byte_high, byte_low, bytes[i], bytes[i+1]);
    //     // std::cout << std::hex << byte_high << " " << byte_low << "\n";

    //     // float delta = 1.0 / 1201;

    //     // uint64_t row = i / 2 / 1201; 
    //     // uint64_t col = (i / 2) % 1201;

    //     // float Nmin = 50 - row * delta;
    //     // float Emin = 18 + col * delta;

    //     // std::cout << '[' << i << ']';
    //     // std::cout << row << ' ' << col << " N " << Nmin << " E " << Emin << "[" << height << " m n.p.m]" << '\n';

    //     heights.push_back(height);
    // }
}

/* Populates 'directories_list' vector with paths to directories inside 'directory_name'. */
void get_subdirectories_list(std::vector<std::string> &directories_list, char* directory_name)
{
    DIR* directory;

    struct dirent* dir;

    directory = opendir(directory_name);
    if (!directory)
    {
        std::cout << "Couldn't open directory " << directory_name << '\n';
        return;
    }

    while ((dir = readdir(directory)) != NULL)
    {
        /* If directory is not . (current directory) or .. (parent directory) */
        if (dir->d_name[0] !=  '.')
        {
            std::string dirname(directory_name);
            dirname.append("/");
            dirname.append(dir->d_name);
            directories_list.push_back(dirname);
        }
    }
    closedir(directory);
}

/* Populates 'files_list' vector with paths to files inside 'directory_name' of given 'extension'.
 * 'extension' needs to be provided withouth ".", e.g. "txt".
 */
void get_files_list_by_extension(std::vector<std::string> &files_list, char* directory_name, char* parent_directory, char* extension)
{
    DIR* directory = opendir(directory_name);
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

        char* extension = strtok(filename, ".");
        extension = strtok(NULL, ".");
        if (strcmp(extension, extension) == 0)
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

int main()
{

    std::vector<std::string> directories;
    std::vector<std::string> hgtmaps;
    char map_directory[] = "maps";
    char extension[] = "hgt";

    get_subdirectories_list(directories, map_directory);

    for (std::string directory_name : directories)
    {
        char* c_dirname = const_cast<char*>(directory_name.c_str());
        get_files_list_by_extension(hgtmaps, c_dirname, map_directory, extension);
    }

    // std::vector<std::vector<uint16_t>> heights;
    std::vector<uint16_t> heights;
    std::vector<float> coordinates;

    char filename[] = "maps/test/N49E020.hgt";


    load_heightmap(heights, filename);

    // int n_rows = heights.size();
    // int n_cols = heights[0].size();

    // printf("Printing %d heights.\n", n_rows * n_cols);

    // for (int i = 0; i < n_rows; i++)
    // {
    //     for (int j = 0; j < n_cols; j++)
    //     {
    //         if (heights[i][j] > 8000)
    //             std::cout << "[" << i << " " << j << "][" << i * n_rows + j <<  "] " << heights[i][j] << "\n";
    //     }
    // }

    for (uint i = 0; i < heights.size(); i++)
    {
        if (heights[i] > 8000)
            std::cout << "[" << i << "] " << heights[i] << "\n";
    }

    return 0;
}