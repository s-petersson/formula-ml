#include "TrackModel.h"
#include <vector>

TrackModel::TrackModel(Model * track_model) {
    std::vector<int> idx = track_model->getIndices();
    std::vector<float> verts = track_model->getVertices();

    track = new Mesh;
    track->tri_count = idx.size() / 3;
    track->triangles = new Triangle[track->tri_count];

    for (int i = 0; i < track->tri_count; i++) {
        Triangle t;
        t.v1 = glm::vec3(verts[4 * idx[i * 3]],     verts[4 * idx[i * 3] +1],       verts[4 * idx[i * 3] + 2]);
        t.v2 = glm::vec3(verts[4 * idx[i * 3 + 1]], verts[4 * idx[i * 3 + 1] + 1],  verts[4 * idx[i * 3 + 1] + 2]);
        t.v3 = glm::vec3(verts[4 * idx[i * 3 + 2]], verts[4 * idx[i * 3 + 1] + 1],  verts[4 * idx[i * 3 + 2] + 2]);
        track->triangles[i] = t;
    }
}

TrackModel::~TrackModel() {
    if (track) {
        delete[] track->triangles;
        delete track;
    }
}


bool overlaps(const Triangle& tri, const glm::vec3& point) {
    float d = ((tri.v2.y - tri.v3.y)*(tri.v1.x - tri.v3.x) + (tri.v3.x - tri.v2.x )*(tri.v1.y - tri.v3.y));
    float a = ((tri.v2.y - tri.v3.y)*(point.x - tri.v3.x) + (tri.v3.x - tri.v2.x)*(point.y - tri.v3.y)) / d;
    float b = ((tri.v3.y - tri.v1.y)*(point.x - tri.v3.x) + (tri.v1.x - tri.v3.x)*(point.y - tri.v3.y)) / d;
    float c = 1.0f - a - b;
    return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}


bool TrackModel::onTrack(const glm::vec3& point) {
    for (int i = 0; i < track->tri_count; i++) {
        if (overlaps(track->triangles[i], point)) return true;
    }
    return false;
}
