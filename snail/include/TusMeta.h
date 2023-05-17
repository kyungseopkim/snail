#ifndef TUS_META_H
#define TUS_META_H

#include "Options.h"
#include "cpr/cpr.h"
#include "base64.h"
#include <map>
#include <string>

namespace snail {
    class TusMeta
            : public std::map<std::string, std::string, cpr::CaseInsensitiveCompare> {
    public:
        TusMeta();

        virtual ~TusMeta();

        void put(const std::string &key, const std::string &value);

        std::string get_values() const;

        void fromOptions(const Options &options);
    };
} // namespace snail
#endif // TUS_META_H