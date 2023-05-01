//
// Created by Jerry Kim on 4/30/23.
//

#ifndef SNAIL_STRINGUTILS_H
#define SNAIL_STRINGUTILS_H
#include <iostream>
#include <vector>
#include <string>

class StringUtils {
private:
    std::vector< std::string > _strings;
public:
    StringUtils();
    virtual ~StringUtils();

    void split(std::string str, char separator);
    void list(std::string dir);
    const std::vector<std::string> &getStrings() const;
};


#endif //SNAIL_STRINGUTILS_H
