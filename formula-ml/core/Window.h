#ifndef CORE_WINDOW
#define CORE_WINDOW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "WindowState.h"
#include <core/gfx/Framebuffer.h>

class Window {
public:
	Window();
	~Window();

	void run();

	void setState(WindowState * s);
private:
	GLFWwindow * window;
	int screen_width, screen_height;

	WindowState * state;
    gfx::Framebuffer fbo;
};

#endif