//
// Created by Jerry Kim on 4/25/23.
//

#ifndef TUS_CPP_TUSFILEUTILS_H
#define TUS_CPP_TUSFILEUTILS_H

#include <iostream>
#include <sys/stat.h>

class TusFileUtils {
public:
    TusFileUtils();
    virtual ~TusFileUtils();

    static uint64_t getFileSize(const std::string& file);
    static std::string getFingerPrint(const std::string& file);
    static bool isFileExist(const std::string& file);
};


#endif //TUS_CPP_TUSFILEUTILS_H
