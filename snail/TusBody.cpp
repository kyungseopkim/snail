#include "TusBody.h"

namespace snail {
    TusBody::TusBody(const std::string &filepath, off64_t offset, off64_t size) : cpr::Body() {
        std::ifstream is(filepath, std::ifstream::binary);
        if (is) {
            is.seekg(offset, is.beg);
            std::vector<char> buffer;
            buffer.resize(size);
            is.read(&buffer[0], static_cast<int>(size));
            is.close();
            str_ = std::string(buffer.begin(), buffer.end());
        } else {
            throw std::invalid_argument("Can't open the file for HTTP request body!");
        }
    }
} // namespace snail