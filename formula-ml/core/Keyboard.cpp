#include "Keyboard.h"
#include <iostream>

// Lazy hack to get a state for each standard key, takes some unnecessary memory but is fast.
bool key_pressed[512];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (action == GLFW_PRESS) key_pressed[key] = true;
	if (action == GLFW_RELEASE) key_pressed[key] = false;
}

bool isKeyDown(int key) {
	return key_pressed[key];
}