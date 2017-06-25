#include <string>

std::string rotate_right(const std::string &s){
    if (s.empty())
        return s;

    return s.back() + s.substr(0, s.size() - 1);
}
