#include "ImageIO.h"
#include <FreeImage.h>
#include <iostream>

namespace util {

	void save_framebuffer(gfx::Framebuffer fb, std::string path) {
		FreeImage_Initialise();
		//Allocate image.
		auto bitmap = FreeImage_Allocate(fb.width, fb.height, 24);
		auto texture = new GLubyte[fb.width * fb.height * 4];
		//Copy Framebuffer content to the allocated image.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fb.id);
		glReadPixels(0,0,fb.width, fb.height,GL_RGBA,GL_UNSIGNED_BYTE,texture);
		
		RGBQUAD colour;
		for (int i = 0; i < fb.width; ++i) {			
			for (int j = 0; j < fb.height; ++j) {
				auto index = (j * fb.width + i) * 4;
				colour.rgbRed	= texture[index + 0];
				colour.rgbGreen = texture[index + 1];
				colour.rgbBlue	= texture[index + 2];
				FreeImage_SetPixelColor(bitmap, i, j, &colour);
			}
		}
		delete[] texture;

		//Save the image to file.
		FreeImage_Save(FIF_PNG, bitmap, path.c_str(), 0);
		FreeImage_DeInitialise();


	}

}