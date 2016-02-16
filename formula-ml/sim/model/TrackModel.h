#ifndef SIM_MODEL_TRACK
#define SIM_MODEL_TRACK

#include <core/gfx/Model.h>
#include <glm/glm.hpp>

struct Triangle {
    int i1, i2, i3;
};

struct Vertex {
    glm::vec3 pos;
};

struct Mesh {
    Triangle * triangles;
    Vertex * vertices;
    int tri_count;
    int vert_count;
};

class TrackModel {
public:
    TrackModel(Model * track_model);
    ~TrackModel();

    bool onTrack(const glm::vec3& point);

private:
    Mesh * track;
};

#endif