#include "FileTracker.h"
#include "TusFileUtils.h"

namespace snail {

FileTracker::FileTracker() = delete;

// FileTracker::FileTracker(const FileTracker &) = default;
FileTracker::FileTracker(const std::string &filename)
    : m_filename(filename), m_offset(0),
      m_size(TusFileUtils::getFileSize(filename)),
      m_fingerPrint(TusFileUtils::getFingerPrint(filename)) {}

FileTracker::~FileTracker() = default;

const std::string &FileTracker::file() const { return m_filename; }

off_t FileTracker::offset() const { return m_offset; }

const off_t FileTracker::size() const { return m_size; }

void FileTracker::seek(off_t offset) { m_offset = offset; }

off_t FileTracker::leftSize() const { return m_size - m_offset; }

const std::string &FileTracker::fingerPrint() const { return m_fingerPrint; }
} // namespace snail