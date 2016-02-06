#ifndef CORE_UTIL_UTIL
#define CORE_UTIL_UTIL
#include <GL/glew.h>

struct FBO
{
    GLuint id;
    GLuint color_buffer;
    GLuint depth_buffer;
    int width;
    int height;
};

FBO createFBO(int width, int height, bool multisample);

long current_time();






#endif