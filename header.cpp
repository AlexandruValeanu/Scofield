#include <sstream>
#include "header.h"

Header make_standard_header(const std::vector<size_t > &file_sizes){
    Header header;

    header.number_files = file_sizes.size();
    header.file_sizes = file_sizes;

    return header;
}

Header read_standard_header(std::istream &istream){
    Header header;
    std::string line;

    std::getline(istream, line);
    header.number_files = std::stoul(line);

    std::getline(istream, line);
    std::istringstream iss(line);

    size_t x;
    while (iss >> x){
        header.file_sizes.push_back(x);
    }

    return header;
}

std::string Header::to_string() {
    std::string s;
    s += std::to_string(this->number_files) + "\n";
    s += std::to_string(this->file_sizes[0]);

    for (int i = 1; i < this->number_files; i++)
        s += " " + std::to_string(this->file_sizes[i]);

    return s;
}
