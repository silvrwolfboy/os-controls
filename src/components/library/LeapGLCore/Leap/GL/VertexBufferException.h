#pragma once

#include "Leap/GL/Exception.h"

#ifndef LEAP_NOEXCEPT
  #if _WIN32
    #define LEAP_NOEXCEPT
  #else
    #define LEAP_NOEXCEPT noexcept
  #endif
#endif

namespace Leap {
namespace GL {

// Base class for all Leap::GL::VertexBuffer exceptions.
class VertexBufferException : public GLException {
public:

  VertexBufferException (const std::string &message) : GLException(message) { }
};

} // end of namespace GL
} // end of namespace Leap

using namespace Leap::GL; // TEMPORARY until the Leap::GL:: scoping has been integrated into all code.
