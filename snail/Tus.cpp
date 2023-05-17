//
// Created by Jerry Kim on 4/24/23.
//

#include "Tus.h"
#include <cpr/cpr.h>
#include <iostream>
#include <sstream>
#include <string>

#include "Options.h"
#include "StringUtils.h"
#include "TusFileUtils.h"
#include "TusHeader.h"
#include "TusMeta.h"
#include <algorithm>
#include <utility>
#include "TusBody.h"

namespace snail {

    Tus::~Tus() { delete _db; }

    Tus::Tus(Options &options)
            : _options(options),
              _endpoint(options.validUrl()),
              _tracker(options.getFile(), options.getChunkSize()) {

        if (_endpoint.empty()) {
            std::cerr << "Invalid URL: " << options.getUrl() << std::endl;
            exit(1);
        }
        std::cout << "Config : " << _options.getConfig() << "\t";
        std::cout << "Endpoint: " << _endpoint << std::endl;

        _db_options.create_if_missing = true;
        leveldb::Status status =
                leveldb::DB::Open(_db_options, _options.getStore(), &_db);
        if (!status.ok()) {
            std::cerr << "Unable to open/create database " << _options.getStore()
                      << std::endl;
            exit(1);
        }
    }

    bool Tus::verifyResponse(cpr::Response &resp) {
        if (resp.status_code != 200 && resp.status_code != 201 &&
            resp.status_code != 204) {
            std::cerr << "Invalid response code: " << resp.status_code << std::endl;
            exit(1);
        }
        if (resp.header.find(TusHeader::TUS_RESUMABLE) != resp.header.end()) {
            return true;
        }
        return false;
    }

    std::string Tus::create() {
        TusHeader header;
        TusMeta meta;
        meta.fromOptions(_options);
        u_int64_t fileLength = _tracker.size();

        header.put(TusHeader::UPLOAD_METADATA, meta.get_values());
        header.put(TusHeader::UPLOAD_DEFER_LENGTH, "1");

        auto resp = cpr::Post(cpr::Url{_endpoint}, header,
                              cpr::Timeout{_options.getTimeout() * 1000});
        if (!verifyResponse(resp)) {
            std::cerr << "Check Invalid Server Url :" << _endpoint << std::endl;
            exit(1);
        }
        if (resp.header.find("Location") == resp.header.end()) {
            std::cerr << "Invalid response: missing Location header" << std::endl;
            exit(1);
        }

        return resp.header["Location"];
    }

    void Tus::resume(const std::string &location) {
        int64_t lastOffset = -1;
        int64_t offset = 0;
        while (offset < _tracker.size()) {
            TusHeader info = head(location);
            offset = info.getInt(TusHeader::UPLOAD_OFFSET);
            if (lastOffset == offset) {
                std::cerr << "Error: cannot move forward" << std::endl;
                exit(1);
            }
            if (offset == _tracker.size())
                break;

            while (offset < _tracker.size()) {
                patch(location);
                offset = _tracker.offset();
                std::cout << "." << std::flush;
            }
            lastOffset = offset;
        }
    }

    void Tus::upload() {
        if (_options.isNoSave()) {
            const std::string location = create();
            resume(location);
        } else {
            std::string location;
            leveldb::Status s;
            s = this->_db->Get(leveldb::ReadOptions(), _tracker.fingerPrint(), &location);
            if (!s.ok()) {
                location = create();
                s = this->_db->Put(leveldb::WriteOptions(), _tracker.fingerPrint(), location);
                if (!s.ok()) {
                    std::cerr << "Unable to save key: " << _tracker.fingerPrint() << std::endl;
                    exit(1);
                }
            }
            resume(location);
            s = this->_db->Delete(leveldb::WriteOptions(), _tracker.fingerPrint());
            if (!s.ok()) {
                std::cerr << "Unable to delete key: " << _tracker.fingerPrint() << std::endl;
                exit(1);
            }
        }
        std::cout << std::endl;
        std::cout << "Upload complete : " << _options.getFile() << std::endl;
    }

    Options &Tus::options() {
        return _options;
    }

    TusHeader Tus::head(const std::string &location) {
        TusHeader request;
        auto resp = cpr::Head(cpr::Url{location}, request,
                              cpr::Timeout{_options.getTimeout() * 1000});
        if (!verifyResponse(resp)) {
            std::cerr << "Invalid response code: " << resp.status_code << std::endl;
            exit(1);
        }
        if (resp.header.find(TusHeader::UPLOAD_OFFSET) == resp.header.end()) {
            std::cerr << "Invalid response: missing Upload-Offset header" << std::endl;
            exit(1);
        }
        return TusHeader(resp.header);
    }

    TusHeader Tus::patch(const std::string &location) {
        TusHeader header;
        header.put(TusHeader::CONTENT_LENGTH, std::to_string(_tracker.leftSize()));
        header.put(TusHeader::CONTENT_TYPE, TusHeader::OCTET_STREAM);
        header.put(TusHeader::UPLOAD_OFFSET, std::to_string(_tracker.offset()));

        cpr::Response resp = cpr::Patch(cpr::Url{location}, header,
                                        TusBody{_tracker.file(), _tracker.offset(), _tracker.leftSize()},
                                        cpr::Timeout{_options.getTimeout() * 1000});
        if (!verifyResponse(resp)) {
            std::cerr << "Invalid response code: " << resp.status_code << std::endl;
            exit(1);
        }
        _tracker.seek(_tracker.offset() + _tracker.leftSize());
        return header;
    }
} // namespace snail
