//
// Created by Jerry Kim on 4/25/23.
//

#ifndef TUS_CPP_TUSFILEUTILS_H
#define TUS_CPP_TUSFILEUTILS_H

#include <iostream>
#include <sys/stat.h>

namespace snail {

    class TusFileUtils {
    public:
        TusFileUtils();

        virtual ~TusFileUtils();

        static off_t getFileSize(const std::string &file);

        static std::string getFingerPrint(const std::string &file);

        static bool isFileExist(const std::string &file);
    };

} // namespace snail
#endif // TUS_CPP_TUSFILEUTILS_H
