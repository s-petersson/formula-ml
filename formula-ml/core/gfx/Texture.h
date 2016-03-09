#ifndef CORE_GFX_TEXTURE
#define CORE_GFX_TEXTURE

#include <GL/glew.h>
#include <string>

struct Texture{
    GLuint id;
    GLuint width;
    GLuint height;
};

Texture create_texture(GLuint width, GLuint height, unsigned char * pixels, GLuint texture_unit, bool wrap);

Texture load_tga(const std::string& file, GLuint texture_unit, bool wrap);

void delete_texture(Texture t);

#endif
