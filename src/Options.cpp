//
// Created by Jerry Kim on 4/29/23.
//

#include "Options.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

static std::string nullToEmpty(const char* value) {
    if (value == nullptr) {
        return "";
    }
    return value;
}

static int emptyToZero(const char* key) {
    auto value = nullToEmpty(std::getenv(key));
    if (value.empty()) {
        return 0;
    }
    try {
        return std::stoi(value);
    } catch (std::invalid_argument& e) {
        return 0;
    }
    return std::stoi(value);
}

Options::Options() {
    _config = nullToEmpty(std::getenv("CONFIG_FILE"));
    if (_config.empty())
        _config = ".env";
    _vin = nullToEmpty(std::getenv("VIN"));
    _url = nullToEmpty(std::getenv("SERVER"));
    _meta = nullToEmpty(std::getenv("META"));
    _store = nullToEmpty(std::getenv("STORE"));
    if (_store.empty()) {
        _store = ".snail";
    }

    if (_url.empty()) {
        _url = "http://localhost:1080/files/";
    }

    _chunkSize = emptyToZero("CHUNK_SIZE");
    if (_chunkSize <= 0) {
        _chunkSize = 1024*1024;
    }
    _timeout = emptyToZero("TIMEOUT");
    if (_timeout <= 0) {
        _timeout = 120;
    }
    if (nullToEmpty(std::getenv("NO_RESUME")) == "true") {
        _noSave = true;
    }
}

const std::string &Options::getVin() const {
    return _vin;
}

const std::string &Options::getUrl() const {
    return _url;
}

const std::string &Options::getMeta() const {
    return _meta;
}

const std::string &Options::getStore() const {
    return _store;
}

int Options::getChunkSize() const {
    return _chunkSize;
}

int Options::getTimeout() const {
    return _timeout;
}

bool Options::isNoSave() const {
    return _noSave;
}


Options::~Options() = default;

const std::string &Options::getConfig() const {
    return _config;
}

bool Options::isPatchMethod() const {
    return _patchMethod;
}

void Options::setVin(const std::string &vin) {
    Options::_vin = vin;
}

void Options::setUrl(const std::string &url) {
    Options::_url = url;
}

void Options::setMeta(const std::string &meta) {
    Options::_meta = meta;
}

void Options::setStore(const std::string &store) {
    Options::_store = store;
}

void Options::setChunkSize(int chunkSize) {
    Options::_chunkSize = chunkSize;
}

void Options::setTimeout(int timeout) {
    Options::_timeout = timeout;
}

void Options::setNoSave(bool noSave) {
    Options::_noSave = noSave;
}

void Options::setPatchMethod(bool patchMethod) {
    Options::_patchMethod = patchMethod;
}

void
Options::override(const std::string &vin, const std::string &url, const std::string &meta, const std::string &store,
                  int chunkSize, int timeout, bool noSave, bool patchMethod) {

    if (!vin.empty()) {
        Options::_vin = vin;
    }
    if (!url.empty()) {
        Options::_url = url;
    }
    if (!meta.empty()) {
        Options::_meta = meta;
    }
    if (!store.empty()) {
        Options::_store = store;
    }
    if (chunkSize > 0) {
        Options::_chunkSize = chunkSize;
    }
    if (timeout > 0) {
        Options::_timeout = timeout;
    }
    if (noSave) {
        Options::_noSave = noSave;
    }
    if (patchMethod) {
        Options::_patchMethod = patchMethod;
    }
}

std::string Options::validUrl() {
    if (_url.back() == '/') {
        return _url;
    }
    return _url + "/";
}

const std::string &Options::getFile() const {
    return _file;
}

void Options::setFile(const std::string &file) {
    _file = file;
}

std::string Options::toString() const {
    std::ostringstream  oss;
    oss << "vin: " << _vin << std::endl;
    oss << "url: " << _url << std::endl;
    oss << "meta: " << _meta << std::endl;
    oss << "store: " << _store << std::endl;
    oss << "chunkSize: " << _chunkSize << std::endl;
    oss << "timeout: " << _timeout << std::endl;
    oss << "noSave: " << _noSave << std::endl;
    oss << "patchMethod: " << _patchMethod << std::endl;
    return oss.str();
}
