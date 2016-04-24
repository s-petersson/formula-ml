#include "Curve.h"

using namespace glm;
using namespace gfx;

Curve::Curve() {

    buffers[0] = 0;
    buffers[1] = 1;

    capacity = 64;
    size = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Curve::~Curve() {
    glBindVertexArray(0);
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vao);
}

void Curve::append_vertex(vec4 pos, vec4 col) {
    positions.push_back(pos);
    colours.push_back(col);
    glBindVertexArray(vao);
    if (positions.size() > capacity) {
        capacity *= 2;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(vec4),&positions[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(vec4), &colours[0], GL_DYNAMIC_DRAW);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, (positions.size() - 1) * sizeof(vec4), sizeof(vec4), &positions[positions.size() - 1]);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferSubData(GL_ARRAY_BUFFER, (colours.size() - 1) * sizeof(vec4), sizeof(vec4), &colours[colours.size() - 1]);

    }
    glBindVertexArray(0);
    size++;
}



void Curve::render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES_ADJACENCY, 0, size);
}