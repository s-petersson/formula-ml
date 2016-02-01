#include <core/Window.h>
#include "SimulationState.h"

int main(void)
{	
	
	Window * window = new Window;
	SimulationState * s = new SimulationState;
	window->setState(s);
	window->run();
	delete window;
	return 0;
}