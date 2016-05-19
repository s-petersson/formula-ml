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
        void add_circle(const  glm::vec3& min, const glm::vec3& max, const glm::vec4 & color);
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
        
        GLuint  primitive_program;
        
        // Lines
        GLuint  line_vao;
        std::vector<glm::vec4> line_positions;
        std::vector<glm::vec4> line_colors;
        GLuint line_buffers[2];

        // Quads
        GLuint  quad_vao;
        std::vector<glm::vec4> quad_positions;
        std::vector<glm::vec4> quad_colors;
        GLuint quad_buffers[2];

        // Circles
        GLuint circle_vao;
        std::vector<glm::vec4> circle_positions;
        std::vector<glm::vec4> circle_colors;
        std::vector<glm::vec2> circle_uvs;
        GLuint circle_buffers[3];
        GLuint circle_program;

        //Texts
        GLuint  text_vao;
        std::vector<glm::vec4> text_positions;
        std::vector<glm::vec4> text_colors;
        std::vector<glm::vec2> text_uvs;
        GLuint text_buffers[3];
        GLuint  text_program;


    };

}

#endif