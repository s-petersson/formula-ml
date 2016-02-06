#include "Model.h"
#include <fstream>
#include <sstream>
#include <cstring>

// TODO: Implement setModelMatrixUniformLoc method.
using namespace std;

Model::Model(string path) {
    loadModel(path);
}

Model::~Model() {
    // TODO: delete instance variables and clear opengl memory
}

GLuint Model::getVAO() {
    return vao;
}

vector<float> Model::getVertices() {
    return vertices;
}

vector<int> Model::getIndices() {
    return indices;
}

glm::mat4x4 Model::getModelMatrix() {
    return modelMatrix;
}

void parseFile(string path, vector<float> *vertices, vector<int> *indices) {
    ifstream file(path);
    string line;
    string type;
    while(getline(file, line)) {
        istringstream stream(line);
        stream >> type;

        if (type == "v") {
            float x, y, z;
            stream >> x; stream >> y; stream >> z;

            // Save theese coordinates.
            vertices->push_back(x);
            vertices->push_back(y);
            vertices->push_back(z);
            vertices->push_back(1.0f); // w
        } else if (type == "f") {
            string first, second, third;
            stream >> first; stream >> second; stream >> third;

            indices->push_back(stoi(first.substr(0, first.find("/"))) - 1);
            indices->push_back(stoi(second.substr(0, second.find("/"))) - 1);
            indices->push_back(stoi(third.substr(0, third.find("/"))) - 1);
        }
    }
}

GLuint initModel (vector<float> vertices, vector<int> indices) {
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Load the vertices into a buffer
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Load the indices into a buffer
    GLuint indicesBuffer;
    glGenBuffers(1, &indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Specify where in the VAO the vertices are located.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glEnableVertexAttribArray(0); // Enable the vertices attribute.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return vertexArrayObject;
}

GLuint Model::loadModel(string path) {
    // Read the file and parse it accordingly, filling the
    // vectors that we supply with data.
    parseFile(path, &vertices, &indices);

    vao = initModel(vertices, indices);
}
