#include "TusBody.h"

namespace snail {
TusBody::TusBody(const std::string &filepath, off_t offset) : cpr::Body() {
  std::ifstream is(filepath, std::ifstream::binary);
  if (is) {
    is.seekg(0, is.end);
    int length = is.tellg() - offset;
    is.seekg(offset, is.beg);
    std::vector<char> buffer;
    buffer.resize(length);
    is.read(&buffer[0], length);
    is.close();
    str_ = std::string(buffer.begin(), buffer.end());
  } else {
    throw std::invalid_argument("Can't open the file for HTTP request body!");
  }
}
} // namespace snail