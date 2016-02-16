#include "TrackModel.h"
#include <vector>

TrackModel::TrackModel(Model * track_model) {
    std::vector<int> idx = track_model->getIndices();
    std::vector<float> verts = track_model->getVertices();

    track = new Mesh;
    track->tri_count = idx.size() / 3;
    track->vert_count = verts.size() / 4;
    track->triangles = new Triangle[track->tri_count];
    track->vertices = new Vertex[track->vert_count];

    for (int i = 0; i < track->vert_count; i++) {
        Vertex vertex;
        vertex.pos = glm::vec3(verts[4 * (i)], verts[4 * i + 1], verts[4 * i + 2]);
        track->vertices[i] = vertex;
    }

    for (int i = 0; i < track->tri_count; i++) {
        Triangle t;
        t.i1 = idx[i * 3];
        t.i2 = idx[i * 3 + 1];
        t.i3 = idx[i * 3 + 2];
        track->triangles[i] = t;
    }
}

TrackModel::~TrackModel() {
    if (track) {
        delete[] track->triangles;
        delete track;
    }
}


bool overlaps(const Triangle& tri, Vertex* vertices, const glm::vec3& point) {
    glm::vec3 v1 = vertices[tri.i1].pos;
    glm::vec3 v2 = vertices[tri.i2].pos;
    glm::vec3 v3 = vertices[tri.i3].pos;
    float d = ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x ) * (v1.y - v3.y));
    float a = ((v2.y - v3.y) * (point.x - v3.x) + (v3.x - v2.x) * (point.y - v3.y)) / d;
    float b = ((v3.y - v1.y) * (point.x - v3.x) + (v1.x - v3.x) * (point.y - v3.y)) / d;
    float c = 1.0f - a - b;
    return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}


bool TrackModel::onTrack(const glm::vec3& point) {
    for (int i = 0; i < track->tri_count; i++) {
        if (overlaps(track->triangles[i], track->vertices, point)) return true;
    }
    return false;
}
