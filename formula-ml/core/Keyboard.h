#ifndef CORE_KEYBOARD
#define CORE_KEYBOARD

#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool isKeyDown(int key);

const unsigned int VK_UP = 265;
const unsigned int VK_DOWN = 264;
const unsigned int VK_LEFT = 263;
const unsigned int VK_RIGHT = 262;

#endif