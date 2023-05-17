#ifndef _FILE_TRACKER_H_
#define _FILE_TRACKER_H_

#include <string>

namespace snail {
    class FileTracker {
    public:
        // FileTracker(const FileTracker &ft);
        FileTracker(const std::string &filename, size_t chunkSize);

        ~FileTracker() = default;

        off64_t leftSize() const;

        off64_t size() const;

        const std::string &file() const;

        off64_t offset() const;

        void seek(off64_t offset);

        const std::string &fingerPrint() const;

    private:
        const std::string &m_filename;
        const std::string m_fingerPrint;
        const size_t m_chunkSize;
        off64_t m_offset;
        const off64_t m_size;
    };
} // namespace snail
#endif
