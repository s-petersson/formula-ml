#ifndef CORE_GFX_PROGRAM
#define CORE_GFX_PROGRAM
#include <GL\glew.h>
#include <string>

GLuint CreateShader(std::string vertex_source, std::string fragment_source);

#endif