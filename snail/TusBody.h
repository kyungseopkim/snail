#include "cpr/cpr.h"

namespace snail {
class TusBody : public cpr::Body {
 public:
    TusBody(): cpr::Body() {};
    ~TusBody() = default;
    TusBody(const std::string& file, off_t offset);
};    
}