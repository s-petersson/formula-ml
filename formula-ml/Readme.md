# Build configuration 

## Visual Studio 2015
Just open the solution/project and build

## Any other compiler / OS

The project uses glfw as a window manager and glew for loading OpenGL extensions. 

Download glfw sources from http://www.glfw.org/index.html, compile for your system and link to the static library.

Download GLEW from http://glew.sourceforge.net/ and compile for your system. When linking to the static library you need to #define GLEW_STATIC either in code or 
as a compiler flag with -DGLEW_STATIC. 

Add the folder /external/include/ to the include path of the compiler. 