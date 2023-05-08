#ifndef SNAIL_TUS_HEADER_H
#define SNAIL_TUS_HEADER_H

#include "cpr/cpr.h"
#include <iostream>

namespace snail {
class TusHeader : public cpr::Header {

public:
  // keys
  static const std::string CONTENT_LENGTH;
  static const std::string CONTENT_TYPE;
  static const std::string TUS_RESUMABLE;
  static const std::string UPLOAD_LENGTH;
  static const std::string UPLOAD_OFFSET;
  static const std::string UPLOAD_METADATA;
  static const std::string UPLOAD_DEFER_LENGTH;
  static const std::string UPLOAD_CONCAT;
  static const std::string UPLOAD_CHECKSUM;
  // values
  static const std::string TUS_VERSION;
  static const std::string OCTET_STREAM;

  TusHeader();
  TusHeader(cpr::Header &header);
  virtual ~TusHeader();

  void put(const std::string &key, const std::string &value);
  std::string toString() const;
  int64_t getInt(const std::string &key);
  void debug();

private:
  void init();
};
} // namespace snail
#endif // SNAIL_TUS_HEADER_H
