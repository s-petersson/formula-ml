#include "Util.h"

#include <chrono>
#include <GL/glew.h>

long util::current_time() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

void util::gl_error_check(const std::string& tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL ERROR @ ");
        printf(tag.c_str());
        printf(" : %p \n", error);
    }
}
