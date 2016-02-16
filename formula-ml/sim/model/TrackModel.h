#ifndef SIM_MODEL_TRACK
#define SIM_MODEL_TRACK

#include <core/gfx/Model.h>
#include <glm/glm.hpp>

class TrackModel {
public:
    TrackModel();
    ~TrackModel();

    Model* get_model();
    bool on_track(const glm::vec3& point);
	void fillTrackMatrix(struct TrackGrid& grid, glm::vec3& position, glm::vec3& direction);
    void get_checkpoints(glm::vec3** checkpoints, int* size);

private:
    Model* model;
};

struct TrackGrid {
	float* data;
	int size, width, depth;
	float cell_size, value_track, value_not_track;
};

#endif