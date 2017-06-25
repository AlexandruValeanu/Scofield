#include "burrows_wheeler_transform.h"
#include "strings_utility.h"
#include <algorithm>
#include <iostream>
#include "constants.h"

std::string bwt(const std::string &s){
    auto t = s + END_SYMBOL;
    std::vector<std::string> table(t.size());
    table[0] = t;

    for (int i = 1; i < t.size(); ++i) {
        table[i] = rotate_right(table[i - 1]);
    }

    sort(table.begin(), table.end());

    std::string ss;

    for (auto &x: table)
        ss.push_back(x.back());

    return ss;
}


std::string inverse_bwt(const std::string &s){
    std::vector<std::string> table(s.size());

    for (int i = 0; i < s.size(); i++){
        for (int j = 0; j < s.size(); j++)
            table[j] = s[j] + table[j];

        sort(table.begin(), table.end());
    }

    for (auto &t: table)
        if (t.back() == END_SYMBOL)
            return t;

    throw std::runtime_error{"inverse_bwt:: no string ends with END_SYMBOL"};
}