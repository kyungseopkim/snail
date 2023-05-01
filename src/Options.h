//
// Created by Jerry Kim on 4/29/23.
//

#ifndef SNAIL_OPTIONS_H
#define SNAIL_OPTIONS_H


#include <string>

class Options {
public:
    Options();
    ~Options();

    const std::string &getConfig() const;

    const std::string &getVin() const;

    const std::string &getUrl() const;

    const std::string &getMeta() const;

    const std::string &getStore() const;

    int getChunkSize() const;

    int getTimeout() const;

    bool isNoSave() const;

    bool isPatchMethod() const;

    void setVin(const std::string &vin);

    void setUrl(const std::string &url);

    void setMeta(const std::string &meta);

    void setStore(const std::string &store);

    void setChunkSize(int chunkSize);

    void setTimeout(int timeout);

    void setNoSave(bool noSave);

    void setPatchMethod(bool patchMethod);

    void override(const std::string& vin,
                  const std::string& url,
                  const std::string& meta,
                  const std::string& store,
                  int chunkSize,
                  int timeout,
                  bool noSave,
                  bool patchMethod);

    std::string validUrl();

    const std::string &getFile() const;

    void setFile(const std::string &file);

    std::string toString() const;
private:
    std::string _config;
    std::string _file;
    std::string _vin;
    std::string _url;
    std::string _meta;
    std::string _store;
    int _chunkSize{1024 * 1024};
    int _timeout{120};
    bool _noSave = false;
    bool _patchMethod = false;
};


#endif //SNAIL_OPTIONS_H
