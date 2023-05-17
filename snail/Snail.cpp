#include "Snail.h"

namespace snail {
    Snail::Snail(Options &options) : _tus(options) {
    }

    Snail::~Snail() = default;

    void Snail::upload() {
        _tus.upload();
    }
} // namespace snail