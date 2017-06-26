#ifndef BURROWSWHEELER_TRANSFORM_HEADER_H
#define BURROWSWHEELER_TRANSFORM_HEADER_H

#include <vector>
#include <cstddef>
#include <string>
#include <fstream>

class Header{
public:
    size_t number_files;
    std::vector<size_t > file_sizes;

    std::string to_string();
};

Header make_standard_header(const std::vector<size_t > &file_sizes);
Header read_standard_header(std::istream &istream);

#endif //BURROWSWHEELER_TRANSFORM_HEADER_H
