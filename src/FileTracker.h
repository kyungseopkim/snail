#ifndef _FILE_TRACKER_H_
#define _FILE_TRACKER_H_
#include <string>

namespace snail {
class FileTracker {
public:
  FileTracker();
  // FileTracker(const FileTracker &ft);
  FileTracker(const std::string &filename);
  ~FileTracker();

  off_t leftSize() const;

  const off_t size() const;

  const std::string &file() const;

  off_t offset() const;

  void seek(off_t offset);

  const std::string &fingerPrint() const;

private:
  const std::string &m_filename;
  const std::string m_fingerPrint;
  off_t m_offset;
  const off_t m_size;
};
} // namespace snail
#endif
