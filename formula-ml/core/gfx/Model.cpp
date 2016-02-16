#include "Model.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>

using namespace std;

Model::Model(string path) {
    mesh = new Mesh();
    load_model(path);
}

Model::~Model() {
    delete mesh;
}

std::vector<float> Model::get_raw_vertices(bool use_w) {
    vector<float> vertices;
    for (int i = 0; i < mesh->vertices.size(); i++) {
        vertices.push_back(mesh->vertices[i].pos.x);
        vertices.push_back(mesh->vertices[i].pos.y);
        vertices.push_back(mesh->vertices[i].pos.z);
        if (use_w){
            vertices.push_back(1.0f);
        }
    }
    return vertices;
}

vector<int> Model::get_indices() {
    vector<int> indices;
    for (int i = 0; i < mesh->triangles.size(); i++) {
        indices.push_back(mesh->triangles[i].i1);
        indices.push_back(mesh->triangles[i].i2);
        indices.push_back(mesh->triangles[i].i3);
    }
    return indices;
}

Mesh* Model::get_mesh() {
    return mesh;
}

GLuint Model::getVAO() {
    return vao;
}

glm::mat4x4 Model::get_model_matrix() {
    return model_matrix;
}

GLuint Model::get_model_matrix_loc() {
    return model_matrix_loc;
}

void Model::set_model_matrix_loc(GLuint location) {
    model_matrix_loc = location;
}

void parse_file(string path, vector<Vertex>* vertices, vector<Triangle>* triangles) {
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
            Vertex vertex;
            vertex.pos = glm::vec3(x, y, z);
            vertices->push_back(vertex);
        } else if (type == "f") {
            string first, second, third;
            stream >> first; stream >> second; stream >> third;

            Triangle triangle;
            triangle.i1 = stoi(first.substr(0, first.find("/"))) - 1;
            triangle.i2 = stoi(second.substr(0, second.find("/"))) - 1;
            triangle.i3 = stoi(third.substr(0, third.find("/"))) - 1;
            triangles->push_back(triangle);
        }
    }
}

void Model::create_vao() {

    vector<float> vertices = get_raw_vertices();
    std::cout << "vertices: " << vertices.size() << std::endl;
    vector<int> indices = get_indices();
    std::cout << "indices: " << indices.size() << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Load the vertices into a buffer
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Load the indices into a buffer
    GLuint indices_buffer;
    glGenBuffers(1, &indices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Specify where in the VAO the vertices are located.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glEnableVertexAttribArray(0); // Enable the vertices attribute.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


void Model::load_model(string path) {
    // Read the file and parse it accordingly, filling the
    // vectors that we supply with data.
    parse_file(path, &(mesh->vertices), &(mesh->triangles));
}
