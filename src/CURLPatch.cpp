//
// Created by Jerry Kim on 4/30/23.
//

#include "CURLPatch.h"

static size_t noop_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

CURLPatch::CURLPatch(const std::string &location, const Options &options, const TusHeader &header,
                     const BigFileManager &fileManager)
        : _fileManager(fileManager), _location(location) {
    _curl = curl_easy_init();
    if (!_curl) {
        std::cerr << "curl_easy_init() failed" << std::endl;
        exit(1);
    }

    _headers = nullptr;
    for (auto item: header) {
        _headers = curl_slist_append(_headers, (item.first + ": " + item.second).c_str());
    }

    curl_easy_setopt(_curl, CURLOPT_URL, _location.c_str());
    curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, options.getTimeout());
    curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(_curl, CURLOPT_READDATA, (void*) _fileManager.getFile());
    curl_easy_setopt(_curl, CURLOPT_INFILESIZE_LARGE, _fileManager.leftSize());
    curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, noop_cb);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, noop_cb);
    curl_easy_setopt(_curl, CURLOPT_HEADER, false);


}

CURLPatch::~CURLPatch() {
    if (_headers) {
        curl_slist_free_all(_headers);
    }
    if (_curl) {
        curl_easy_cleanup(_curl);
    }
}

void CURLPatch::run() {
    CURLcode res = curl_easy_perform(_curl);
    if (res != CURLE_OK) {
        std::cerr << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
}
