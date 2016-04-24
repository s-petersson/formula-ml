#ifndef SIM_MODEL_TRACK
#define SIM_MODEL_TRACK

#include <core/gfx/Model.h>
#include <glm/glm.hpp>

struct TrackGrid {
    float* data;
    int size, width, depth;
    float cell_size, value_track, value_not_track;
};

struct Checkpoint {
    glm::vec3 left, right, middle;
    float distance_on_track;
};

struct Pair {
    int a, b;
};

class TrackModel {
public:
    TrackModel(glm::vec3 start_grid_pos, const string& path);
    ~TrackModel();

    Model* get_model();
	glm::vec3 get_start_grid_pos();
    bool on_track(const glm::vec3& point);
	void fillTrackGrid(struct TrackGrid& grid, glm::vec3& position, glm::vec3& direction);
    std::vector<Checkpoint> get_checkpoints();
private:
    void create_checkpoints();

    Model* model;
	glm::vec3 start_grid_pos;
    std::vector<Checkpoint> checkpoints;
};

#endif