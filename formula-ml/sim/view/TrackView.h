#ifndef SIM_VIEW_TRACK
#define SIM_VIEW_TRACK

#include <core/gfx/Model.h>
#include <sim/model/TrackModel.h>
#include <GL/glew.h>

class TrackView {
public:
    TrackView(TrackModel *dataModel);
    ~TrackView();

    void setUniformLocations(GLuint shaderProgram, char* modelMatrixUniform);
    void render();
private:
    GLuint modelMatrixLoc;
    GLuint track_vao;

    GLuint checkpoints_vao;
    GLuint checkpoint_triangle_count;

    void initTrack();
    void initCheckpoints();

    Model *viewModel;
    TrackModel *dataModel;
};

#endif