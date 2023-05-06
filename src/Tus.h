//
// Created by Jerry Kim on 4/24/23.
//

#ifndef TUS_CPP_TUS_H
#define TUS_CPP_TUS_H

#include "Options.h"
#include "cpr/cpr.h"
#include "leveldb/db.h"
#include <iostream>
#include <map>
#include "TusHeader.h"
#include "TusFileUtils.h"
#include "Options.h"
#include "cpr/body.h"
#include "FileTracker.h"

namespace snail {



class Tus {
private:
  std::string _endpoint;
  const Options &_options;
  leveldb::DB *_db{};
  leveldb::Options _db_options;
  FileTracker _tracker;
    
public:
  Tus(Options &options);
  ~Tus();

  std::string create();
  void resume(const std::string &location);
  void upload();
  static bool verifyResponse(cpr::Response &resp);
  TusHeader options();
  TusHeader head(const std::string &location);
  TusHeader patch(const std::string &location);
};

class TusBody : public cpr::Body {
 public:
    TusBody(): cpr::Body() {};
    ~TusBody() = default;


//  cpr::Body(const cpr::File& file, off_t offset) {
//         std::ifstream is(file.filepath, std::ifstream::binary);
//         if (is) {
//             is.seekg(0, is.end);
//             int length = is.tellg() - offset;
//             is.seekg(offset, is.beg);
//             std::vector<char> buffer;
//             buffer.resize(length);
//             is.read(&buffer[0], length);
//             is.close();
//             str_ = std::string(buffer.begin(), buffer.end());
//         } else {
//             throw std::invalid_argument("Can't open the file for HTTP request body!");
//         }
//     }    
};
} // namespace snail

#endif // TUS_CPP_TUS_H
