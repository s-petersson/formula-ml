#ifndef EXPERIMENTS_NETWORKVIEW
#define EXPERIMENTS_NETWORKVIEW
#include <neural/neat/Network.h>
#include <core/gui/View.h>
#include <core/gfx/Renderer.h>
#include <glm/glm.hpp>
#include <map>

class NetworkView : public Renderer{
public:
	NetworkView(neat::Network* net);
	~NetworkView();

	virtual void initialize();
	virtual void render();
	virtual void reset();
private:
	neat::Network* network;
	gui::View* gui;
	std::map<int, glm::vec3> positions;
};

#endif