#pragma once

#include <NullGameEngine.hpp>

// not including Component.hpp may produce confusion in a source file
#include <Component.hpp>

namespace null {
    // scripts are essentially additional components
    using Script = Component;
}
