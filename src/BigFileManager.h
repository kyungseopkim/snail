//
// Created by Jerry Kim on 4/30/23.
//

#ifndef EXAMPLEPROJECT_BIGFILEMANAGER_H
#define EXAMPLEPROJECT_BIGFILEMANAGER_H
#include<iostream>
#include <vector>

class BigFileManager {
private:
    const std::string& _filename;
    int _fd;
    u_int64_t _size;
    off_t _offset;
    const int _chunkSize;
    FILE *_file;
public:
    explicit BigFileManager(const std::string&, const int chunkSize);
    virtual ~BigFileManager();

    ssize_t read(std::vector<uint8_t> &buffer, int64_t size);
    void seek(int64_t offset);

    int getFd() const;
    int leftSize() const;

    u_int64_t getSize() const;

    off_t getOffset() const;

    FILE *getFile() const;
};


#endif //EXAMPLEPROJECT_BIGFILEMANAGER_H
