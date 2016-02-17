#include "TrackModel.h"
#include <glm/gtx/euler_angles.hpp>
#include <iostream>
#include <algorithm>

using namespace glm;

TrackModel::TrackModel() {
    model = new Model("./res/models/spa_circuit.model");
}

TrackModel::~TrackModel() {
    if (model) {
        delete model;
    }
}

Model* TrackModel::get_model() {
    return model;
}


bool overlaps(const Triangle& tri, vector<Vertex> vertices, const glm::vec3& point) {
    glm::vec3 v1 = vertices[tri.i1].pos;
    glm::vec3 v2 = vertices[tri.i2].pos;
    glm::vec3 v3 = vertices[tri.i3].pos;
    float d = ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x ) * (v1.y - v3.y));
    float a = ((v2.y - v3.y) * (point.x - v3.x) + (v3.x - v2.x) * (point.y - v3.y)) / d;
    float b = ((v3.y - v1.y) * (point.x - v3.x) + (v1.x - v3.x) * (point.y - v3.y)) / d;
    float c = 1.0f - a - b;
    return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}


bool TrackModel::on_track(const glm::vec3& point) {
    for (int i = 0; i < model->get_mesh()->triangles.size(); i++) {
        if (overlaps(model->get_mesh()->triangles[i], model->get_mesh()->vertices, point)) return true;
    }
    return false;
}

void TrackModel::fillTrackMatrix(TrackGrid& grid, glm::vec3& position, glm::vec3& direction) {
    // Clear the array
    for (int i = 0; i < grid.size; i++) {
        grid.data[i] = grid.value_not_track;
    }

    vec2 transPre = vec2(position.x, position.y) * -1.f;
	float rotationAngle = atan2(direction.y, direction.x) + 3.14159265359/2;
    mat2 rotationMatrix = orientate2(rotationAngle); // 'direction' should be the new y-axis
    vec2 transPost = vec2(grid.width * grid.cell_size / 2, 0);

	vec2 testRotateDirection = rotationMatrix * vec2(direction.x, direction.y);

    // For each triangle in the mesh, find if it overlaps a cell
    for (int i = 0; i < model->get_mesh()->triangles.size(); i++) { // TODO: make foreach
        Triangle triangle = model->get_mesh()->triangles[i];

        // translatePre -> rotate -> translatePost
        vec3 v1 = model->get_mesh()->vertices[triangle.i1].pos;
        vec3 v2 = model->get_mesh()->vertices[triangle.i2].pos;
        vec3 v3 = model->get_mesh()->vertices[triangle.i3].pos;

        v1 = vec3((rotationMatrix * (vec2(v1.x, v1.y) + transPre)) + transPost, 0);
        v2 = vec3((rotationMatrix * (vec2(v2.x, v2.y) + transPre)) + transPost, 0);
        v3 = vec3((rotationMatrix * (vec2(v3.x, v3.y) + transPre)) + transPost, 0);

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
				cout << "triangle:" << triangle.i1+triangle.i2+triangle.i3 << " x: " << xMin << "-" << xMax << " y: " << yMin << "-" << yMax << "\n";
                if (overlaps(triangle, model->get_mesh()->vertices, vec3(x*grid.cell_size, y*grid.cell_size, 0))) {
					grid.data[x + y * grid.depth] = grid.value_track;
                }
            }
        }
    }
}

/**
 * Helper method that calculates the value in the middle of two floats.
 */
float middleOf(float left, float right) {
    float diff = glm::abs(left - right);
    float min = glm::min(left, right);
    return min + diff / 2;
}

/**
 * Get checkpoint in the middle of the road at every new two triangles.
 */
void TrackModel::get_checkpoints(glm::vec3** result, int* size) {
    *size = model->get_mesh()->vertices.size() / 2;
    *result = new glm::vec3[*size];
    // NOTE: We currently only loop the vertices in the order which they are read from the
    //       obj file. Which is not a good approach, since it depends on how the model was
    //       constructed in the modeling program.
    //       TODO: Change this later, probably when we use a b-spline for the model instead.
    Vertex v1, v2;
    float x, y, z;
    for (int i = 0; i < model->get_mesh()->vertices.size() / 2; i += 2) {
        v1 = model->get_mesh()->vertices[i];
        v2 = model->get_mesh()->vertices[i + 1];
        x = middleOf(v1.pos.x, v2.pos.x);
        y = middleOf(v1.pos.y, v2.pos.y);
        z = middleOf(v1.pos.z, v2.pos.z);

        *result[i] = glm::vec3(x, y, z);
    }
}
