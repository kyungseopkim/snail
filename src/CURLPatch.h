//
// Created by Jerry Kim on 4/30/23.
//

#ifndef SNAIL_CURLPATCH_H
#define SNAIL_CURLPATCH_H
#include <curl/curl.h>
#include "Options.h"
#include "Tus.h"
#include "BigFileManager.h"

class CURLPatch {
private:
    CURL *_curl;
    const std::string &_location;
    const BigFileManager &_fileManager;
    struct curl_slist *_headers;

public:
    CURLPatch(const std::string& location, const Options &options, const TusHeader& header, const BigFileManager& fileManager);
    virtual ~CURLPatch();

    void run();
};


#endif //SNAIL_CURLPATCH_H
