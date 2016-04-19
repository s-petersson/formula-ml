#include "TrackModel.h"
#include <glm/gtx/euler_angles.hpp>
#include <iostream>
#include <algorithm>
#include <map>

using namespace glm;

TrackModel::TrackModel(vec3 start_grid_pos) {
    model = new Model("./res/models/circuit_narrow.model");
    this->start_grid_pos = start_grid_pos;
   
	create_checkpoints();

}

TrackModel::~TrackModel() {
    if (model) {
        delete model;
    }
}

Model* TrackModel::get_model() {
    return model;
}

bool overlaps(const glm::vec3& point, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    float d = ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    float a = ((v2.y - v3.y) * (point.x - v3.x) + (v3.x - v2.x) * (point.y - v3.y)) / d;
    float b = ((v3.y - v1.y) * (point.x - v3.x) + (v1.x - v3.x) * (point.y - v3.y)) / d;
    float c = 1.0f - a - b;
    return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
}

bool overlaps(const Triangle& tri, vector<Vertex>& vertices, const glm::vec3& point) {
    glm::vec3 v1 = vertices[tri.i1].pos;
    glm::vec3 v2 = vertices[tri.i2].pos;
    glm::vec3 v3 = vertices[tri.i3].pos;
    return overlaps(point, v1, v2, v3);
}

bool TrackModel::on_track(const glm::vec3& point) {
    for (auto && t : model->get_mesh()->triangles) {
        if (overlaps(t, model->get_mesh()->vertices, point)) return true;
    }

    return false;
}


int fillTrackGridCounter = 0;
// TODO?: It seems to fill the grid wrong way in the x-axis
void TrackModel::fillTrackGrid(TrackGrid& grid, glm::vec3& position, glm::vec3& direction) {
    // Clear the array
    for (int i = 0; i < grid.size; i++) {
        grid.data[i] = grid.value_not_track;
    }

    vec2 transPre = vec2(position.x, position.y) * -1.f;
    float rotationAngle = -atan2(direction.y, direction.x) + 3.14159265359 / 2;
    mat2 rotationMatrix = orientate2(rotationAngle); // 'direction' should be the new y-axis
    vec2 transPost = vec2(grid.width * grid.cell_size / 2, 0);

    vec2 testRotateDirection = rotationMatrix * vec2(direction.x, direction.y);
    vec2 testRotateDirection2 = rotationMatrix * vec2(1, 0);
    vec2 testRotateDirection3 = rotationMatrix * vec2(0, 2);

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
        int xMin = glm::min(glm::min(v1.x, v2.x), v3.x) / grid.cell_size;
        int xMax = glm::ceil(glm::max(glm::max(v1.x, v2.x), v3.x) / grid.cell_size);
        int yMin = glm::min(glm::min(v1.y, v2.y), v3.y) / grid.cell_size;
        int yMax = glm::ceil(glm::max(glm::max(v1.y, v2.y), v3.y) / grid.cell_size);

        // Limit the bounding box to the track matrix
        xMin = glm::max(xMin, 0);
        xMax = glm::min(xMax, grid.width - 1);
        yMin = glm::max(yMin, 0);
        yMax = glm::min(yMax, grid.depth - 1);

        // For each point in the bounding box, check if it overlap the triangle.
        // If so, set the cell to grid.value_track
        for (int y = yMin; y <= yMax; y++) {
            for (int x = xMin; x <= xMax; x++) {
                if (overlaps(vec3(x*grid.cell_size, y*grid.cell_size, 0), v1, v2, v3)) {
					grid.data[x + y * grid.width] = grid.value_track;
                }
            }
        }
    }
	/*
	fillTrackGridCounter++;
	if (fillTrackGridCounter % 12 == 0) {
        // Print the track
		cout << "\n\n\n";
		for (int y = grid.depth; y >= 0; y--) {
            for (int x = 0; x < grid.width; x++) {
            //for (int x = grid.width - 1; x >= 0; x--) {
				if (grid.data[x + y * grid.width] == grid.value_track) {
					cout << "X ";
				} else {
					cout << "_ ";
				}
			}
			cout << "\n";
		}
	}
	/**/
}

glm::vec3 TrackModel::get_start_grid_pos() {
    //return start_grid_pos;
	return checkpoints[0].middle;
}

/**
 * Helper method that calculates the value in the middle of two floats.
 */
float middleOf(float left, float right) {
    float diff = glm::abs(left - right);
    float min = glm::min(left, right);
    return min + diff / 2;
}

void try_insert(map<int, vector<int>> *all_pairs, map<int, vector<int>> *path_map, int i1, int i2) {
    int pre_insert_size = all_pairs->size();
    int hash = 17 * i1 + 37 * i2;
    if ((*all_pairs).find(hash) == (*all_pairs).end()) {
        // First hash was not found in the map; Swap hash value
        hash = 17 * i2 + 37 * i1;
    }
    // Now we add the indexes to the correct key in the hash map
    (*all_pairs)[hash].push_back(i1);
    (*all_pairs)[hash].push_back(i2);

    // If the size of the map changed, meaning that we found
    // an edge that we have previously found (two triangles share)
    // this edge; Add that edge to the path map.
    if (all_pairs->size() == pre_insert_size) {
        (*path_map)[i1].push_back(i2);
        (*path_map)[i2].push_back(i1);
    }
}

void create_path_pairs(vector<Pair> *result, map<int, vector<int>> *path_map, int initial_index) {
    int visited     = 1;
    int last_key    = -1;
    int curr_key    = initial_index;

    while(visited < path_map->size() - 1) {
        vector<int> curr_list = path_map->at(curr_key);
        int size = curr_list.size();

        if (size == 2) {
            int index = (curr_list[0] == last_key) ? 1 : 0;

            Pair pair;
            pair.a = curr_key;
            pair.b = curr_list[index];
            result->push_back(pair);

            last_key = curr_key;
            curr_key = curr_list[index];
        } else if(size == 3) {
            int index_with_one_entry;
            for (int i = 0; i < size; i++) {
                if (path_map->at(curr_list[i]).size() == 1) {
                    index_with_one_entry = i;

                    Pair pair;
                    pair.a = curr_key;
                    pair.b = curr_list[i];
                    result->push_back(pair);
                    break;
                }
            }

            int index = (index_with_one_entry + 1) % 3 ? (index_with_one_entry + 2) % 3 : (index_with_one_entry + 1) % 3;

            Pair pair;
            pair.a = curr_key;
            pair.b = curr_list[index];
            result->push_back(pair);

            last_key = curr_key;
            curr_key = curr_list[index];
        }
        visited++;
    }
}

/**
 * Get checkpoint in the middle of the road at every new two triangles.
 * Distance on the track for all returned checkpoints will be relative to
 * the starting grid position currently set when executing this method.
 */
void TrackModel::create_checkpoints() {
    vector<Triangle> triangles = model->get_mesh()->triangles;
    map<int, vector<int>> all_pairs;
    map<int, vector<int>> path_map;
    for (int i = 0; i < triangles.size(); i++) {
        Triangle tri = triangles[i];
        try_insert(&all_pairs, &path_map, tri.i1, tri.i2);
        try_insert(&all_pairs, &path_map, tri.i2, tri.i3);
        try_insert(&all_pairs, &path_map, tri.i3, tri.i1);
    }
    vector<Pair> ordered_path_pairs;
    create_path_pairs(&ordered_path_pairs, &path_map, 0);

    int size = ordered_path_pairs.size();
    for (int i = 0; i < ordered_path_pairs.size() / 2; i++) {
        Checkpoint checkpoint;
        Vertex v1 = model->get_mesh()->vertices[ordered_path_pairs[2 * i].a];
        Vertex v2 = model->get_mesh()->vertices[ordered_path_pairs[2 * i].b];
        checkpoint.left = v1.pos;
        checkpoint.right = v2.pos;

        float x = middleOf(v1.pos.x, v2.pos.x);
        float y = middleOf(v1.pos.y, v2.pos.y);
        float z = middleOf(v1.pos.z, v2.pos.z);
        checkpoint.middle = glm::vec3(x, y, z);

        checkpoints.insert(checkpoints.begin()+i, checkpoint);
    }

    std::reverse(checkpoints.begin(), checkpoints.end());

    for (int i = 0; i < checkpoints.size(); ++i) {
        float distance_to_prev_checkpoint = 0;
        vec3 last_checkpoint_pos = start_grid_pos;
        if (i > 0) {
            Checkpoint last_checkpoint = checkpoints[i - 1];
            distance_to_prev_checkpoint = last_checkpoint.distance_on_track;
            last_checkpoint_pos = last_checkpoint.middle;
        }

        checkpoints[i].distance_on_track = distance_to_prev_checkpoint + glm::distance(checkpoints[i].middle, last_checkpoint_pos);
    }
}

std::vector<Checkpoint> TrackModel::get_checkpoints() {
    return checkpoints;
}
