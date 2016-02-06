#ifndef CORE_GFX_MODEL
#define CORE_GFX_MODEL

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Model {
public:
    Model(std::string path);
    ~Model();

    GLuint loadModel(std::string path);

    GLuint getVAO();
    std::vector<float> getVertices();
    std::vector<int> getIndices();
    glm::mat4x4 getModelMatrix();
private:
    GLuint vao;

    std::vector<float> vertices;
    std::vector<int> indices;
    glm::mat4x4 modelMatrix;
};


#endif
