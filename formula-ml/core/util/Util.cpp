#include "Util.h"

#include <chrono>

long util::current_time() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return (long) std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}