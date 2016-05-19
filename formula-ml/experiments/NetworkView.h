#ifndef EXPERIMENTS_NETWORKVIEW
#define EXPERIMENTS_NETWORKVIEW
#include <neural/neat/Network.h>
#include <core/gui/View.h>
#include <core/gfx/Renderer.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

class NetworkView : public Renderer{
public:
	NetworkView(neat::Network* net);
	~NetworkView();

    virtual void initialize();
    virtual void render();
    virtual void update(float dt);
    virtual void reset();
private:
    struct Edge{
        const int in;
        const int out;
    };

	neat::Network* network;
	gui::View* gui;
    std::vector<Edge> edges;
	std::unordered_map<int, glm::vec3> positions;
};

#endif