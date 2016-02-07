#ifndef SIM_VIEW_CAR
#define SIM_VIEW_CAR

#include <sim/model/CarModel.h>
#include <GL/glew.h>
#include <core/gfx/Model.h>

class CarView {
public:
    CarView(CarModel *dataModel);
    ~CarView();

    void setUniformLocations(GLuint shaderProgram, char* modelMatrix);
    void render();
private:
    Model *viewModel;
    CarModel *dataModel;
};


#endif
