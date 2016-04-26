#ifndef CORE_UTIL_IMAGEIO
#define CORE_UTIL_IMAGEIO
#include <string>
#include <gl/glew.h>
#include <core/gfx/Framebuffer.h>

namespace util {

	/**
	 *	Saves the contents of a framebuffer to the path specifies.
	 *	Assumes that the Pixel format is RGBA8 and that the bufffer is properly initialised.
     *  Make sure that the framebuffer is not multisampled. 
	 */
	void save_framebuffer(gfx::Framebuffer fb, std::string path);
}

#endif