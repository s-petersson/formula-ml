#ifndef CORE_KEYBOARD
#define CORE_KEYBOARD

#include <GLFW\glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//void init_keyboard(GLFWwindow * window);

bool isKeyDown(int key);


#endif