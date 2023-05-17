#include "cpr/cpr.h"

namespace snail {
    class TusBody : public cpr::Body {
    public:
        TusBody() : cpr::Body() {};

        ~TusBody() override = default;

        TusBody(const std::string &file, off64_t offset, off64_t size);
    };
} // namespace snail