#include "StringUtil.h"
#include <cctype>

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    size_t last = 0;
    for (size_t i = 0; i < line.length(); i++) {
        if (std::isspace(line[i])) {
            if (last != i) {
                tokens.push_back(line.substr(last, i - last));
            }
            last = i+1;
        }
    }
    if (last < line.length()) {
        tokens.push_back(line.substr(last));
    }
    return tokens;
}
