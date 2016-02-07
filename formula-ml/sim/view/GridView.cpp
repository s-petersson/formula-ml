#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GridView.h"

GridView::GridView() {
    glm::vec4 * positions = new glm::vec4[84];
    glm::vec4 * colours = new glm::vec4[84];

    for (int i = 0; i < 21; i++) {
        positions[2 * i] = glm::vec4(-1000 + 100 * i, 1000.0f, 0, 1.0f);
        positions[2 * i + 1] = glm::vec4(-1000 + 100 * i, -1000.0f, 0, 1.0f);
    }

    for (int i = 0; i < 21; i++) {
        positions[42 + 2 * i] = glm::vec4(-1000.0f , -1000 + 100 * i, 0, 1.0f);
        positions[42 + 2 * i + 1] = glm::vec4(1000.0f, -1000 + 100 * i, 0, 1.0f);
    }


    for (int i = 0; i < 84; i++) {
        colours[i] = glm::vec4(0.7f, 0.7f, 1.0f, 1.0f);
    }

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, positions, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, colours, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    delete[] positions;
    delete[] colours;
}

void GridView::setUniformLocations(GLuint shaderProgram, char* modelMatrix) {
    modelMatrixUniformLoc = glGetUniformLocation(shaderProgram, modelMatrix);
}

void GridView::render () {
    glUniformMatrix4fv(modelMatrixUniformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 88);
}