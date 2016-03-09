#ifndef CORE_GUI_FONT
#define CORE_GUI_FONT

#include <GL/glew.h>
#include <core/gfx/Texture.h>
#include <glm/glm.hpp>
#include <utility>
#include <map>
#include <string>

namespace gui {

    struct Glyph {
        glm::vec2 uv_pos;
        glm::vec2 uv_dim;
        glm::vec2 size;
        glm::vec2 offset;
        float advance;
    };

    class Font {
    public:
        Font();
        ~Font();

        Glyph get_glyph(char c);
        
        void upload_texture(GLuint shader, std::string sampler);
    private:
        Texture fontmap;  
        std::map<char, Glyph> glyphs;
        
        void load_glyphs(std::string);
    };

}

#endif