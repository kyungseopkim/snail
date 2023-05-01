//
// Created by Jerry Kim on 4/24/23.
//

#include "Tus.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cpr/cpr.h>
#include "base64.h"
#include "TusFileUtils.h"
#include "Options.h"
#include "StringUtils.h"
#include <algorithm>
#include <utility>
#include "BigFileManager.h"
#include "CURLPatch.h"

Tus::~Tus() {
    delete _db;
}

Tus::Tus(Options &options) : _options(options),
                             _endpoint(options.validUrl()),
                             _finger_print(TusFileUtils::getFingerPrint(options.getFile())) {

    if (_endpoint.empty()) {
        std::cerr << "Invalid URL: " << options.getUrl() << std::endl;
        exit(1);
    }
    std::cout << "Config : " << _options.getConfig() << "\t";
    std::cout << "Endpoint: " << _endpoint << std::endl;


    _db_options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(_db_options, _options.getStore(), &_db);
    if (!status.ok()) {
        std::cerr << "Unable to open/create database " << _options.getStore() << std::endl;
        exit(1);
    }
}

//bool Tus::checkFingerPrint(const std::string &file, std::string &hash) {
//    leveldb::Status status = _db->Get(leveldb::ReadOptions(), _finger_print, &hash);
//    if (status.ok()) {
//        return true;
//    }
//    return false;
//}

bool Tus::verifyResponse(cpr::Response &resp) {
    if (resp.status_code != 200 &&
        resp.status_code != 201 &&
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
    u_int64_t fileLength = TusFileUtils::getFileSize(_options.getFile());

    header.put(TusHeader::UPLOAD_METADATA, meta.get_values());
    header.put(TusHeader::UPLOAD_DEFER_LENGTH, "1");

    auto resp = cpr::Post(
            cpr::Url{_endpoint},
            header,
            cpr::Timeout{_options.getTimeout() * 1000}

    );
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
    BigFileManager fileManager(_options.getFile(), _options.getChunkSize());
    int64_t lastOffset = -1;
    int64_t offset = 0;
    while (offset < fileManager.getSize()) {
        TusHeader info = head(location);
        offset = info.getInt(TusHeader::UPLOAD_OFFSET);
        if (lastOffset == offset) {
            std::cerr << "Error: cannot move forward" << std::endl;
            exit(1);
        }
        if (offset == fileManager.getSize()) break;
        fileManager.seek(offset);
        patch(location,fileManager);
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
        s = this->_db->Get(leveldb::ReadOptions(), _finger_print, &location);
        if (!s.ok()) {
            location = create();
            s = this->_db->Put(leveldb::WriteOptions(), _finger_print, location);
            if (!s.ok()) {
                std::cerr << "Unable to save key: " << _finger_print << std::endl;
                exit(1);
            }
        }
        resume(location);
        s = this->_db->Delete(leveldb::WriteOptions(), _finger_print);
        if (!s.ok()) {
            std::cerr << "Unable to delete key: " << _finger_print << std::endl;
            exit(1);
        }
    }
    std::cout << "Upload complete : "<< _options.getFile() << std::endl;
}

TusHeader Tus::options() {
    std::cout << _endpoint << std::endl;
    auto resp = cpr::Options(
            cpr::Url{_endpoint},
            cpr::Timeout{_options.getTimeout() * 1000}
    );

    TusHeader header;
    if (resp.status_code == 200) {
        for (auto &kv: resp.header) {
            header.put(kv.first, kv.second);
        }
    } else {
        std::cerr << "Invalid response code: " << resp.status_code << std::endl;
        exit(1);
    }
    return header;
}

TusHeader Tus::head(const std::string &location) {
    TusHeader request;
    auto resp = cpr::Head(
            cpr::Url{location},
            request,
            cpr::Timeout{_options.getTimeout() * 1000}
    );
    if (!verifyResponse(resp)) {
        std::cerr << "Invalid response code: " << resp.status_code << std::endl;
        exit(1);
    }
    if (resp.header.find(TusHeader::UPLOAD_OFFSET) == resp.header.end()) {
        std::cerr << "Invalid response: missing Upload-Offset header" << std::endl;
        exit(1);
    }
    return resp.header;
}

TusHeader Tus::patch(const std::string &location, BigFileManager& fileManager) {
    TusHeader header;
    header.put(TusHeader::CONTENT_TYPE, TusHeader::OCTET_STREAM);
    header.put(TusHeader::CONTENT_LENGTH, std::to_string(fileManager.leftSize()));
    header.put(TusHeader::CONTENT_TYPE, TusHeader::OCTET_STREAM);
    header.put(TusHeader::UPLOAD_OFFSET, std::to_string(fileManager.getOffset()));
    header.put(TusHeader::UPLOAD_LENGTH, std::to_string(fileManager.getSize()));
    CURLPatch patch(location, _options, header, fileManager);
    patch.run();
    return header;
}


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


TusHeader::TusHeader() {
    init();
}

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

void TusHeader::debug() {
    std::cout << toString() << std::endl;
}

TusHeader::~TusHeader() = default;


TusMeta::~TusMeta() = default;

TusMeta::TusMeta() = default;

void TusMeta::put(const std::string &key, const std::string &value) {
    this->insert(std::pair<std::string, std::string>(key, base64_encode(value)));
}


std::string TusMeta::get_values() const {
    std::stringstream ss;
    for (auto &it: *this) {
        ss << it.first << " " << it.second;
        if (*std::prev(this->end()) != it) {
            ss << ",";
        }
    }
    return ss.str();
}

struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};

std::string basename( std::string const& pathname )
{
return std::string{
        std::find_if( pathname.rbegin(), pathname.rend(),
                      MatchPathSeparator() ).base(),
        pathname.end() };
}

void TusMeta::fromOptions(const Options &options) {

    put("filename", basename(options.getFile()));
    put("vin", options.getVin());
    const auto &metaValues = options.getMeta();
    StringUtils metaString;
    metaString.split(metaValues, ',');
    for (auto &it: metaString.getStrings()) {
        StringUtils itemString;
        itemString.split(it, ' ');
        auto kv = itemString.getStrings();
        if (kv.size() == 2) {
            put(kv[0], kv[1]);
        }
    }
}

