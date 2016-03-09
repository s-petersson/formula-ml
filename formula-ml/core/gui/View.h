#ifndef CORE_GUI_VIEW
#define CORE_GUI_VIEW

#include <glm/glm.hpp>
#include <gl/glew.h>
#include <core/gfx/Program.h>
#include <vector>
#include <string>
#include "Font.h"

namespace gui {

    class View {
    public:
        View();
        View(const glm::vec3 & origin);
        ~View();

        void render();
        void clear();

        void add_line(const glm::vec3 & a, const glm::vec3 & b, const glm::vec4 & color);
        void add_rect(const  glm::vec3& min, const glm::vec3& max, const glm::vec4 & color);
        void add_text(const std::string& text, float size, glm::vec3 origin, glm::vec4 color);


        void set_transform(const glm::mat4& mat);
        void set_projection(const glm::mat4& mat);
        void set_view(const glm::mat4& mat);
    
    private:
        Font* font;

        glm::mat4 proj;
        glm::mat4 view;
        glm::mat4 transform;
        glm::vec3 origin;
        
        std::vector<glm::vec4> line_positions;
        std::vector<glm::vec4> line_colors;

        std::vector<glm::vec4> quad_positions;
        std::vector<glm::vec4> quad_colors;

        std::vector<glm::vec4> text_positions;
        std::vector<glm::vec4> text_colors;
        std::vector<glm::vec2> text_uvs;

        GLuint  line_vao;
        GLuint  quad_vao;

        GLuint  primitive_program;

        GLuint  text_vao;
        GLuint  text_program;
    };

}

#endif