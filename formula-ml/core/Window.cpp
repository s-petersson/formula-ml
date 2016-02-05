#include "Window.h"

#include <glm/glm.hpp>
#include <iostream>

#include "core/Keyboard.h"



void initGL() {

	
}

Window::Window() {

	/* Initialize the library */
	if (!glfwInit()) {
		throw std::runtime_error("GLFW Initialization failed.");
	}

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Formula ML", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Window creation failed.");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}
	glfwSetKeyCallback(window, key_callback);

	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	initGL();
}

Window::~Window() {

}

void Window::setState(WindowState * s) {
	state = s;
}

void Window::run() {

//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
	glViewport(0, 0, screen_width, screen_height);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (state) {
			state->update(0.0123456789);
			state->render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}