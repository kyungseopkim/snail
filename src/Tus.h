//
// Created by Jerry Kim on 4/24/23.
//

#ifndef TUS_CPP_TUS_H
#define TUS_CPP_TUS_H

#include <iostream>
#include <map>
#include "Options.h"
#include "leveldb/db.h"
#include "cpr/cpr.h"
#include "BigFileManager.h"

class TusMeta : public std::map<std::string, std::string, cpr::CaseInsensitiveCompare> {
public:
    TusMeta();
    virtual ~TusMeta();

    void put(const std::string& key, const std::string& value);

    std::string get_values() const;
    void fromOptions(const Options& options);
};

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
    TusHeader(cpr::Header& header);
    virtual ~TusHeader();

    void put(const std::string& key, const std::string& value);
    std::string toString() const;
    int64_t getInt(const std::string& key) ;
    void debug();
private:
    void init();

};



class Tus {
private:
    std::string _endpoint;
    const Options& _options;
    leveldb::DB *_db{};
    leveldb::Options _db_options;
    const std::string _finger_print;
public:
    explicit Tus(Options& options);
    ~Tus();

    std::string create();
    void resume(const std::string &location);
    void upload();
    static bool verifyResponse(cpr::Response& resp);
    TusHeader options();
    TusHeader head(const std::string& location);
    TusHeader patch(const std::string& location, BigFileManager& fileManager);
};


#endif //TUS_CPP_TUS_H
