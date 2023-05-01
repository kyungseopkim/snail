//
// Created by Jerry Kim on 4/30/23.
//

#include "BigFileManager.h"
#include "TusFileUtils.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

BigFileManager::BigFileManager(const std::string& file, const int chunkSize):
_filename(file), _offset(0), _size(TusFileUtils::getFileSize(file)), _chunkSize(chunkSize) {
    _fd = open(_filename.c_str(), O_RDONLY);
    if (_fd == -1) {
        std::cout << "Error opening file: " << _filename << std::endl;
        throw std::runtime_error("Error opening file");
    }
    _file = fopen(_filename.c_str(), "rb");
}

void BigFileManager::seek(int64_t offset) {
    _offset = offset;
    if (lseek(_fd, offset, SEEK_SET) == -1) {
        std::cout << "Error seeking file: " << _filename << std::endl;
        exit(1);
    }
}

ssize_t BigFileManager::read(std::vector<uint8_t> &buffer, int64_t size) {
    return ::read(_fd, buffer.data(), size);
}

BigFileManager::~BigFileManager() {
    fclose(_file);
    ::close(_fd);
}

int BigFileManager::getFd() const {
    return _fd;
}

int BigFileManager::leftSize() const {
//    if (_offset + _chunkSize < _size) {
//        return _chunkSize;
//    }
    return static_cast<int>(_size - _offset);
}

u_int64_t BigFileManager::getSize() const {
    return _size;
}

off_t BigFileManager::getOffset() const {
    return _offset;
}

FILE *BigFileManager::getFile() const {
    return _file;
}




