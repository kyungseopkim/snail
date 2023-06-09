//
// Created by Jerry Kim on 4/25/23.
//

#include "TusFileUtils.h"
#include <sstream>
#include <sys/stat.h>

namespace snail {
    TusFileUtils::TusFileUtils() = default;

    TusFileUtils::~TusFileUtils() = default;

    static struct stat getStat(const std::string &file) {
        struct stat stat_buf{};
        int rc = stat(file.c_str(), &stat_buf);
        if (rc != 0) {
            throw std::runtime_error("File not found");
        }
        return stat_buf;
    }

// get file size
    off_t TusFileUtils::getFileSize(const std::string &file) {
        struct stat stat_buf = getStat(file);
        return stat_buf.st_size;
    }

    std::string TusFileUtils::getFingerPrint(const std::string &path) {
        struct stat stat_buf = getStat(path);
        std::ostringstream oss;
#ifdef __OSX__
        oss << path << ":" << stat_buf.st_size << ":" << stat_buf.st_mtimespec.tv_sec;
        return oss.str();
#else
        oss << path << ":" << stat_buf.st_size << ":" << stat_buf.st_mtime;
        return oss.str();
#endif
    }

    bool TusFileUtils::isFileExist(const std::string &file) {
        struct stat stat_buf{};
        int rc = stat(file.c_str(), &stat_buf);
        return rc == 0;
    }

} // namespace snail
