#include "Window.h"

#include <glm/glm.hpp>
#include <iostream>
#include "core/Keyboard.h"



Window::Window() {

	/* Initialize the library */
	if (!glfwInit()) {
		throw std::runtime_error("GLFW Initialization failed.");
	}
	/*
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	*/
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
    fbo = createFBO(screen_width, screen_height, false);
}

Window::~Window() {

}

void Window::setState(WindowState * s) {
	state = s;
}

void Window::run() {
    
    glClearColor(0.f, 0.f, 0.f, 1.0f);
	
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
    glViewport(0, 0, screen_width, screen_height);

    long last_frame = current_time();
    int fps = 0;
    long fps_timer = 0;

	while (!glfwWindowShouldClose(window)) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        long dt = current_time() - last_frame;
        last_frame = current_time();

        fps++;
        fps_timer += dt;
        if (fps_timer >= 1000) {
            //printf("%i \n", fps);
            fps = 0;
            fps_timer -= 1000;
        }

        if (state) {
			state->update((float)dt / 1000);
			state->render();
		}

        //Blit the framebuffer to screen.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.id);
        glBlitFramebuffer(0, 0, fbo.width, fbo.height, 0, 0, fbo.width, fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glfwSwapBuffers(window);
		glfwPollEvents();
        glfwSwapInterval(1);
	}

	glfwTerminate();
}