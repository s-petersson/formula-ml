#ifndef CORE_GFX_MODEL
#define CORE_GFX_MODEL

#include <vector>
#include <string>
#include <glm/glm.hpp>

using namespace std;

struct Triangle {
    int i1, i2, i3;
};

struct Vertex {
    glm::vec3 pos;
};

struct Mesh {
    vector<Triangle> triangles;
    vector<Vertex> vertices;
};

class Model {
public:
    Model(std::string path);
    ~Model();

    void load_model(std::string path);

    vector<float> get_raw_vertices(bool use_w = true);
    vector<int> get_indices();
    glm::mat4x4 get_model_matrix();
    Mesh* get_mesh();

private:
    Mesh *mesh;
    glm::mat4x4 model_matrix;
};


#endif
