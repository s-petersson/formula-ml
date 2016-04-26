#ifndef CORE_UTIL_UTIL
#define CORE_UTIL_UTIL
#include <string>

namespace util {

    long current_time();

    void gl_error_check(const std::string& tag);

}

#endif