#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GridView.h"
#include <vector>

GridView::GridView() {
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> colours;

    for (int i = 0; i < 21; i++) {
        positions.push_back(glm::vec4(-1000 + 100 * i, 1000.0f, 0, 1.0f));
        positions.push_back(glm::vec4(-1000 + 100 * i, -1000.0f, 0, 1.0f));
    }

    for (int i = 0; i < 21; i++) {
        positions.push_back(glm::vec4(-1000.0f , -1000 + 100 * i, 0, 1.0f));
        positions.push_back(glm::vec4(1000.0f, -1000 + 100 * i, 0, 1.0f));
    }


    for (int i = 0; i < 84; i++) {
        colours.push_back(glm::vec4(0.7f, 0.7f, 1.0f, 1.0f));
    }

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, &positions[0], GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 88, &colours[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void GridView::setUniformLocations(GLuint shaderProgram, char* modelMatrix) {
    modelMatrixUniformLoc = glGetUniformLocation(shaderProgram, modelMatrix);
}

void GridView::render () {
    glUniformMatrix4fv(modelMatrixUniformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 88);
}