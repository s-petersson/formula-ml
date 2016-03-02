#ifndef FORMULA_ML_RENDERER_H
#define FORMULA_ML_RENDERER_H

class Renderer {
public:
    virtual void initialize() = 0;
    virtual void render() = 0;
};

#endif
