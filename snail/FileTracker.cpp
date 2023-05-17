#include "FileTracker.h"
#include "TusFileUtils.h"

namespace snail {

// FileTracker::FileTracker(const FileTracker &) = default;
    FileTracker::FileTracker(const std::string &filename, size_t chunkSize)
            : m_filename(filename), m_offset(0),
              m_size(TusFileUtils::getFileSize(filename)),
              m_chunkSize(chunkSize),
              m_fingerPrint(TusFileUtils::getFingerPrint(filename)) {}

    const std::string &FileTracker::file() const { return m_filename; }

    off64_t FileTracker::offset() const { return m_offset; }

    off64_t FileTracker::size() const { return m_size; }

    void FileTracker::seek(off_t offset) { m_offset = offset; }

    off64_t FileTracker::leftSize() const {
        if ((m_offset + m_chunkSize) < m_size) {
            return static_cast<off64_t>(m_chunkSize);
        }
        return m_size - m_offset;
    }

    const std::string &FileTracker::fingerPrint() const { return m_fingerPrint; }
} // namespace snail