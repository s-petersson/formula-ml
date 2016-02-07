#ifndef SIM_VIEW_GRID
#define SIM_VIEW_GRID

#include <GL/glew.h>
#include <glm/vec4.hpp>

class GridView {
public:
    GridView();

    void render();
    void setUniformLocations(GLuint shaderProgram, char* modelMatrix);
private:
    GLuint vao;
    GLuint modelMatrixUniformLoc;
};



#endif //FORMULA_ML_GRIDVIEW_H
