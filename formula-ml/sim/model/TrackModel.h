#ifndef SIM_MODEL_TRACK
#define SIM_MODEL_TRACK

#include <core/gfx/Model.h>
#include <glm/glm.hpp>

struct Triangle {
    glm::vec3 v1, v2, v3;
};

struct Mesh {
    Triangle * triangles;
    int tri_count;
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