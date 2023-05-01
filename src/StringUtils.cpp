//
// Created by Jerry Kim on 4/30/23.
//

#include "StringUtils.h"
#include <dirent.h>

StringUtils::StringUtils() = default;

StringUtils::~StringUtils() = default;

void StringUtils::split(std::string str, char separator) {
    _strings.clear();
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {

        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            _strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
}

const std::vector<std::string> &StringUtils::getStrings() const {
    return _strings;
}

void StringUtils::list(std::string dir) {
    _strings.clear();
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == nullptr) {
        throw std::runtime_error("Error(" + std::to_string(errno) + ") opening " + dir);
    }

    while ((dirp = readdir(dp)) != nullptr) {
        _strings.emplace_back(dirp->d_name);
    }
    closedir(dp);
}
