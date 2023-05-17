#pragma once

#include <iostream>
#include "Options.h"
#include "Tus.h"

namespace snail {
    class Snail {
    public:
        Snail(Options &options);

        ~Snail();

        void upload();

    private:
        Tus _tus;
    };
} // namespace snail