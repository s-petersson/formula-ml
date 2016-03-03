#include "View.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace gui;
using namespace std;
using namespace glm;

View::View() {
    primitive_program = CreateShader("./res/shaders/simple.vert", "./res/shaders/simple.frag");
    origin = vec3(0, 0, 0);
    set_transform(mat4(1));
    set_projection(ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f));
    set_view(mat4(1));
   
}

View::View(const vec3& o) : View() {
    origin = o;
}

View::~View() {

}

void View::render() {
    glUseProgram(primitive_program);
    
    glBindVertexArray(line_vao);
    glDrawArrays(GL_LINES, 0, line_positions.size());
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, quad_positions.size());
    glBindVertexArray(0);
    glUseProgram(0);
}

void View::clear() {

}




void View::add_line(const vec3 & a, const vec3 & b, const vec4 & color) {
    // Add values to vector. 
    line_positions.push_back(vec4(origin + a, 1.0f));
    line_colors.push_back(color);
    line_positions.push_back(vec4(origin + b, 1.0f));
    line_colors.push_back(color);

    //Rebuild VAO
    glDeleteVertexArrays(1, &line_vao);
    glGenVertexArrays(1, &line_vao);
    glBindVertexArray(line_vao);

    GLuint buffers[2];
    glGenBuffers(2, &buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, line_positions.size() * sizeof(vec4), &line_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
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

    //Rebuild VAO
    glDeleteVertexArrays(1, &quad_vao);
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);

    GLuint buffers[2];
    glGenBuffers(2, &buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, quad_positions.size() * sizeof(vec4), &quad_positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, quad_colors.size() * sizeof(vec4), &quad_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void View::add_text() {

}



void View::set_transform(const glm::mat4& mat) {
    glUseProgram(primitive_program);
    transform = mat;
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(transform));
}

void View::set_projection(const glm::mat4& mat) {
    glUseProgram(primitive_program);
    proj = mat;
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(proj));
}

void View::set_view(const glm::mat4& mat) {
    glUseProgram(primitive_program);
    view = mat;
    glUniformMatrix4fv(glGetUniformLocation(primitive_program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
}
