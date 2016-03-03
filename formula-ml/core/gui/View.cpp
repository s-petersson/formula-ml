#include "View.h"

using namespace gui;
using namespace std;
using namespace glm;

View::View() {
    origin = vec3(0, 0, 0);
}

View::View(const vec3& o) : View() {
    origin = o;
}

View::~View() {

}

void View::render() {
    glUseProgram(primitive_program);
   // glUniformMatrix4fv(viewModel->get_model_matrix_loc(), 1, GL_FALSE, glm::value_ptr(car_transform));
    //glUniformMatrix4fv(viewModel->get_model_matrix_loc(), 1, GL_FALSE, glm::value_ptr(car_transform));
    //glUniformMatrix4fv(viewModel->get_model_matrix_loc(), 1, GL_FALSE, glm::value_ptr(car_transform));

    glBindVertexArray(line_vao);
    glDrawArrays(GL_LINE, 0, line_positions.size());
    glBindVertexArray(0);
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


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, line_colors.size() * sizeof(vec4), &line_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void View::add_rect() {

}

void View::add_text() {

}