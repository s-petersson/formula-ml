#include "Util.h"

#include <chrono>

unsigned long util::current_time() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return (unsigned long) std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}