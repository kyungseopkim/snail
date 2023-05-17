#include "TusMeta.h"
#include "StringUtils.h"

namespace snail {

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

    void TusMeta::fromOptions(const Options &options) {
        put("filename", StringUtils::basename(options.getFile()));
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
} // namespace snail