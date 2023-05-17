#include "TusHeader.h"

namespace snail {

// http header keys
    const std::string TusHeader::TUS_RESUMABLE = "Tus-Resumable";
    const std::string TusHeader::UPLOAD_LENGTH = "Upload-Length";
    const std::string TusHeader::UPLOAD_OFFSET = "Upload-Offset";
    const std::string TusHeader::UPLOAD_METADATA = "Upload-Metadata";
    const std::string TusHeader::CONTENT_LENGTH = "Content-Length";
    const std::string TusHeader::CONTENT_TYPE = "Content-Type";
    const std::string TusHeader::UPLOAD_DEFER_LENGTH = "Upload-Defer-Length";
    const std::string TusHeader::UPLOAD_CONCAT = "Upload-Concat";
    const std::string TusHeader::UPLOAD_CHECKSUM = "Upload-Checksum";

// http header values
    const std::string TusHeader::TUS_VERSION = "1.0.0";
    const std::string TusHeader::OCTET_STREAM = "application/offset+octet-stream";

    TusHeader::TusHeader() { init(); }

    void TusHeader::init() {
        this->clear();
        put(TUS_RESUMABLE, TUS_VERSION);
    }

    void TusHeader::put(const std::string &key, const std::string &value) {
        this->insert(std::pair<std::string, std::string>(key, value));
    }

    std::string TusHeader::toString() const {
        std::stringstream ss;
        for (auto &it: *this) {
            ss << it.first << ": " << it.second << std::endl;
        }
        return ss.str();
    }

    int64_t TusHeader::getInt(const std::string &key) {
        if (find(key) == end()) {
            return 0;
        }
        return std::stoull(this->at(key));
    }

    TusHeader::TusHeader(cpr::Header &header) {
        init();
        for (auto &kv: header) {
            put(kv.first, kv.second);
        }
    }

    void TusHeader::debug() { std::cout << toString() << std::endl; }

    TusHeader::~TusHeader() = default;
} // namespace snail