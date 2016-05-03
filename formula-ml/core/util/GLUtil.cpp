#include "GLUtil.hpp"
#include <GL/glew.h>

void glutil::gl_error_check(const std::string& tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL ERROR @ ");
        printf(tag.c_str());
        printf(" : %p \n", error);
    }
}