#ifndef SIM_VIEW_TRACK
#define SIM_VIEW_TRACK

#include <core/gfx/Model.h>

class TrackView {
public:
    TrackView(TrackModel *dataModel, Model * viewModel);
    ~TrackView();

    void setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform);
    void render();
private:
    Model *viewModel;
    TrackModel *dataModel;
};

#endif