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


} // namespace snail

#endif // TUS_CPP_TUS_H
