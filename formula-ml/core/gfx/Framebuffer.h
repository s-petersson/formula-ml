#ifndef CORE_UTIL_GFX_FRAMEBUFFER
#define CORE_UTIL_GFX_FRAMEBUFFER
#include <gl/glew.h>

namespace gfx {
	struct Framebuffer
	{
		GLuint id;
		GLuint color_buffer;
		GLuint depth_buffer;
		int width;
		int height;
	};

	Framebuffer create_framebuffer(int width, int height, bool multisample);

    void destroy_framebuffer(Framebuffer fbo);
}

#endif