#ifndef CORE_GFX_CURVE
#define CORE_GFX_CURVE
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Program.h"

namespace gfx {
    struct Curve {
        Curve();
        ~Curve();

        void append_vertex(glm::vec4 pos, glm::vec4 col);
        void render();

    private:
        GLuint vao;
        GLuint buffers[2];
        GLuint capacity;
        GLuint size;

        std::vector<glm::vec4> positions;
        std::vector <glm::vec4> colours;
    };
}
#endif