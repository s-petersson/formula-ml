#include "TrackModel.h"
#include <algorithm>
#include <vector>
#include <math.h>
#include <glm/gtx/euler_angles.hpp>

using namespace glm;

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


void TrackModel::fillTrackMatrix(TrackGrid& grid, glm::vec3& position, glm::vec3& direction) {
	// Clear the array
	for (int i = 0; i < grid.size; i++) {
		grid.data[i] = grid.value_not_track;
	}
	
	vec2 transPre = vec2(position.x, position.y) * -1.f;
	mat2 rotation = orientate2(atan2(direction.y, direction.x)); // 'direction' should be the new y-axis
	vec2 transPost = vec2(grid.width*grid.cell_size/2, 0);

	
	// For each triangle in the mesh, find if it overlaps a cell 
	for (int i = 0; i < track->tri_count; i++) { // TODO: make foreach
		Triangle triangle = track->triangles[i];

		// translatePre -> rotate -> translatePost
		vec3 v1 = track->vertices[triangle.i1].pos; 
		vec3 v2 = track->vertices[triangle.i2].pos;
		vec3 v3 = track->vertices[triangle.i3].pos;

		v1 = vec3((rotation * (vec2(v1.x, v1.y) + transPre)) + transPost, 0);
		v2 = vec3((rotation * (vec2(v2.x, v2.y) + transPre)) + transPost, 0);
		v3 = vec3((rotation * (vec2(v3.x, v3.y) + transPre)) + transPost, 0);
		
		// Find the triangle bounding box
		int xMin = std::min(std::min(v1.x, v2.x), v3.x) / grid.cell_size;
		int xMax = std::min(std::min(v1.x, v2.x), v3.x) / grid.cell_size;
		int yMin = std::min(std::min(v1.y, v2.y), v3.y) / grid.cell_size;
		int yMax = std::min(std::min(v1.y, v2.y), v3.y) / grid.cell_size;

		// Limit the bounding box to the track matrix
		xMin = std::max(xMin, 0);
		xMax = std::min(xMax, grid.width);
		yMin = std::max(yMin, 0);
		yMax = std::min(yMax, grid.depth);

		// For each point in the bounding box, check if it overlap the triangle.
		// If so, set the cell to grid.value_track
		for (int x = xMin; x <= xMax; x++) {
			for (int y = yMin; y <= yMax; y++) {
				//if (overlaps(triangle, track->vertices, vec3(x*grid.cell_size, y*grid.cell_size, 0))) {
					grid.data[x + y*grid.depth] = grid.value_track;
				//}
			}
		}
	}
}
