#ifndef SIM_TRACK
#define SIM_TRACK

#include <core/gfx/Model.h>

class Track {
public:
    Track();
    ~Track();

    void render(GLuint modelMatrixUniformLoc);
private:
    Model *model;
};

#endif