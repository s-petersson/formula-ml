#ifndef SIM_VIEW_TRACK
#define SIM_VIEW_TRACK

#include <core/gfx/Model.h>
#include <sim/model/TrackModel.h>

class TrackView {
public:
    TrackView(TrackModel *dataModel);
    ~TrackView();

    void setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform);
    void render();
private:
    GLuint checkpoints_vao;
    GLuint checkpoint_triangles;

    Model *viewModel;
    TrackModel *dataModel;
};

#endif