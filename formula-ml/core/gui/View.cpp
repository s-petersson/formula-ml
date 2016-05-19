#include "View.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>

#include <core/util/Util.h>

using namespace gui;
using namespace std;
using namespace glm;

View::View() {
    font = new Font();
    primitive_program   = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");
    text_program        = CreateShader("./res/shaders/text.vert", "./res/shaders/text.frag");
    circle_program      = CreateShader("./res/shaders/circle.vert", "./res/shaders/circle.frag");

    origin = vec3(0, 0, 0);
    set_transform(mat4(1));
    set_projection(ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f));
    set_view(mat4(1));

    for (int i = 0; i < 3; i++) {
        text_buffers[i] = 0;
        circle_buffers[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        quad_buffers[i] = 0;
        line_buffers[i] = 0;
    }


    text_vao = 0;
    line_vao = 0;
    quad_vao = 0;
    circle_vao = 0;

    glGenVertexArrays(1, &text_vao);
    glBindVertexArray(text_vao);
    glGenBuffers(3, &text_buffers[0]);

    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
    glGenBuffers(2, &quad_buffers[0]);

    glGenVertexArrays(1, &line_vao);
    glBindVertexArray(line_vao);
    glGenBuffers(2, &line_buffers[0]);

    glGenVertexArrays(1, &circle_vao);
    glBindVertexArray(circle_vao);
    glGenBuffers(3, &circle_buffers[0]);

    glBindVertexArray(0);
}

View::View(const vec3& o) : View() {
    origin = o;
    set_transform(translate(mat4(1), o));
}

View::~View() {
    if (font) delete font;
    clear();
    glDeleteBuffers(3, &text_buffers[0]);
    glDeleteVertexArrays(1, &text_vao);
    glDeleteBuffers(2, &line_buffers[0]);
    glDeleteVertexArrays(1, &line_vao);
    glDeleteBuffers(2, &quad_buffers[0]);
    glDeleteVertexArrays(1, &quad_vao);
    glDeleteBuffers(3, &circle_buffers[0]);
    glDeleteVertexArrays(1, &circle_vao);


    glDeleteProgram(text_program);
    glDeleteProgram(primitive_program);
    glDeleteProgram(circle_program);
}

void View::render() {
    glUseProgram(primitive_program);
    
    glBindVertexArray(line_vao);
    glDrawArrays(GL_LINES, 0, (GLsizei) line_positions.size());
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) quad_positions.size());
    glBindVertexArray(0);
    glUseProgram(text_program);
    glBindVertexArray(text_vao);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) text_positions.size());
    glUseProgram(circle_program);
    glBindVertexArray(circle_vao);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)circle_positions.size());
    glUseProgram(0);
}

void View::clear() {
	line_positions.clear();
	line_colors.clear();

	quad_positions.clear();
	quad_colors.clear();

	text_positions.clear();
	text_colors.clear();
	text_uvs.clear();

    circle_colors.clear();
    circle_positions.clear();
    circle_uvs.clear();
}


void View::add_line(const vec3 & a, const vec3 & b, const vec4 & color) {
    // Add values to vector. 
    line_positions.push_back(vec4(origin + a, 1.0f));
    line_colors.push_back(color);
    line_positions.push_back(vec4(origin + b, 1.0f));
    line_colors.push_back(color);

    glBindVertexArray(line_vao);

    glBindBuffer(GL_ARRAY_BUFFER, line_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, line_positions.size() * sizeof(vec4), &line_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, line_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, line_colors.size() * sizeof(vec4), &line_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void View::add_rect(const  glm::vec3& min, const glm::vec3& max, const glm::vec4 & color) {
    // Add values to vector. 
    quad_positions.push_back(vec4(origin + min, 1.0f));
    quad_positions.push_back(vec4(origin + vec3(min.x, max.y, (min.z + max.z) /2.0f), 1.0f));
    quad_positions.push_back(vec4(origin + max, 1.0f));
    
    quad_positions.push_back(vec4(origin + min, 1.0f));
    quad_positions.push_back(vec4(origin + max, 1.0f));
    quad_positions.push_back(vec4(origin + vec3(max.x, min.y, (min.z + max.z) / 2.0f), 1.0f));
    
    for (int i = 0; i < 6; i++) {
        quad_colors.push_back(color);
    }

    glBindVertexArray(quad_vao);

    glBindBuffer(GL_ARRAY_BUFFER, quad_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, quad_positions.size() * sizeof(vec4), &quad_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, quad_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, quad_colors.size() * sizeof(vec4), &quad_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void View::add_circle(const  glm::vec3& min, const glm::vec3& max, const glm::vec4 & color) {
    // Add values to vector. 
    circle_positions.push_back(vec4(origin + min, 1.0f));
    circle_positions.push_back(vec4(origin + vec3(min.x, max.y, (min.z + max.z) / 2.0f), 1.0f));
    circle_positions.push_back(vec4(origin + max, 1.0f));

    circle_positions.push_back(vec4(origin + min, 1.0f));
    circle_positions.push_back(vec4(origin + max, 1.0f));
    circle_positions.push_back(vec4(origin + vec3(max.x, min.y, (min.z + max.z) / 2.0f), 1.0f));

    circle_uvs.push_back(vec2(0,0));
    circle_uvs.push_back(vec2(0, 1));
    circle_uvs.push_back(vec2(1, 1));

    circle_uvs.push_back(vec2(0, 0));
    circle_uvs.push_back(vec2(1, 1));
    circle_uvs.push_back(vec2(1, 0));

    for (int i = 0; i < 6; i++) {
        circle_colors.push_back(color);
    }

    glBindVertexArray(circle_vao);

    glBindBuffer(GL_ARRAY_BUFFER, circle_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, circle_positions.size() * sizeof(vec4), &circle_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, circle_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, circle_colors.size() * sizeof(vec4), &circle_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, circle_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, circle_uvs.size() * sizeof(vec2), &circle_uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


void View::add_text(const std::string& text, float size,glm::vec3 p, glm::vec4 color) {
    for (int i = 0; i < text.length(); i++) {
        
        Glyph g = font->get_glyph(text[i]);

        float u = g.uv_pos.x; 

        float v = 1 - g.uv_pos.y;
        float du = g.uv_dim.x;
        float dv = g.uv_dim.y;
        g.size = g.size * size;

        vec3 c = p + (vec3(g.offset.x, -g.offset.y, 0) * size );

        text_positions.push_back(vec4(c, 1));
        text_uvs.push_back(vec2(u, v));
        
        text_positions.push_back(vec4(c + vec3(g.size.x,0,0), 1));
        text_uvs.push_back(vec2(u + du, v));
        
        text_positions.push_back(vec4(c + vec3(g.size.x, -g.size.y,0), 1));
        text_uvs.push_back(vec2(u + du, v - dv));
        
        text_positions.push_back(vec4(c, 1));
        text_uvs.push_back(vec2(u, v));
        
        text_positions.push_back(vec4(c + vec3(g.size.x, -g.size.y,0), 1));
        text_uvs.push_back(vec2(u + du, v - dv));
        
        text_positions.push_back(vec4(c + vec3(0, -g.size.y, 0), 1));
        text_uvs.push_back(vec2(u, v - dv));
        p.x += g.advance * size;
    }

    for (int i = 0; i < 6 * text.length(); i++) {
        text_colors.push_back(color);
    }

    glBindVertexArray(text_vao);

    glBindBuffer(GL_ARRAY_BUFFER, text_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, text_positions.size() * sizeof(vec4), &text_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, text_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, text_colors.size() * sizeof(vec4), &text_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, text_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, text_uvs.size() * sizeof(vec2), &text_uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    

    glBindVertexArray(0);
}



void View::set_transform(const glm::mat4& mat) {
    transform = mat;
    
    glUseProgram(primitive_program);
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(transform));
    glUseProgram(text_program);
    glUniformMatrix4fv(glGetUniformLocation(text_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(transform));
    glUseProgram(circle_program);
    glUniformMatrix4fv(glGetUniformLocation(circle_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(transform));
}

void View::set_projection(const glm::mat4& mat) {
    proj = mat;
    
    glUseProgram(primitive_program);
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(proj));
    glUseProgram(text_program);
    glUniformMatrix4fv(glGetUniformLocation(text_program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(proj));
    glUseProgram(circle_program);
    glUniformMatrix4fv(glGetUniformLocation(circle_program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(proj));

}

void View::set_view(const glm::mat4& mat) {
    view = mat;
    glUseProgram(primitive_program);
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUseProgram(text_program);
    glUniformMatrix4fv(glGetUniformLocation(text_program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUseProgram(circle_program);
    glUniformMatrix4fv(glGetUniformLocation(circle_program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
}
