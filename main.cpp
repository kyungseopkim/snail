#include <iostream>
#include <CLI/CLI.hpp>
#include "dotenv.h"
#include "Options.h"
#include "Snail.h"
using namespace snail;

void println(const std::string& str) {
    std::cout << str << std::endl;
}

bool is_file_exists(const std::string& file) {
  struct stat stat_buf {};
  int rc = stat(file.c_str(), &stat_buf);
  return rc == 0;
}

const std::string version = "0.0.1";

int main(int argc, char **argv) {
    Options options;
    dotenv::init( options.getConfig().c_str());

    std::stringstream ss;
    ss << "snail. version : " << version;
    CLI::App app{ss.str()};
    app.set_help_flag("-h,--help", "Print this help message and exit");
    std::string file;
    app.add_option("file", file,"File to be uploaded")->required();
    std::string vin;
    app.add_option("-v,--vin", vin,"VIN [VIN]");
    std::string url;
    app.add_option("-s,--server", url,"Server URL [SERVER]");
    std::string meta;
    app.add_option("-m,--meta", meta,"meta data , key=value,... [META]");
    std::string store;
    app.add_option("--store", store,"checkpoint store file path [STORE]");
    int chunkSize{1024*1024};
    app.add_option("--chunk-size", chunkSize,"chunk size [CHUNK_SIZE]");
    int timeout{120};
    app.add_option("--timeout", timeout,"timeout [TIMEOUT]");
    bool noSave = false;
    app.add_flag("-n,--no-save", noSave,"no save meta data for resumable [NO_RESUME]");
    bool patchMethod = false;
    app.add_flag("--set-patch-method", patchMethod,"use PATCH method for resumable createAndUpload");

    CLI11_PARSE(app, argc, argv);

    if(!is_file_exists(file)) {
        println("File not found: " + file);
        return 1;
    }

    options.set(url, meta, store, chunkSize, timeout, noSave, patchMethod);
    options.setFile(file);
    Snail snail(options);
    snail.upload();
    
    return 0;
}
