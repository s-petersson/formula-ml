#include "ImageIO.h"
#include <FreeImage.h>
#include <iostream>

namespace util {

	void save_framebuffer(gfx::Framebuffer fb, std::string path) {
		{
			auto error = glGetError();
			if (error != GL_NO_ERROR) {
				printf("1 KAOS IN THE HAOS -> GL_ERROR: %p \n", error);
			}
		}
		FreeImage_Initialise();
		//Allocate image.
		auto bitmap = FreeImage_Allocate(fb.width, fb.height, 24);
		auto texture = new GLubyte[fb.width * fb.height * 4];
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, fb.id);
		//Copy Framebuffer content to the allocated image.
		{
			auto error = glGetError();
			if (error != GL_NO_ERROR) {
				printf("2 KAOS IN THE HAOS -> GL_ERROR: %p \n", error);
			}
		}
		glReadPixels(0,0,fb.width, fb.height,GL_RGBA8,GL_UNSIGNED_BYTE,texture);
		{
			auto error = glGetError();
			if (error != GL_NO_ERROR) {
				printf("3 KAOS IN THE HAOS -> GL_ERROR: %p \n", error);
			}
		}
		

		long sum = 0;
		for (int i = 0; i < fb.width * fb.height * 4; ++i) {
			sum += texture[i];
		}
		std::cout << "TOTAL SUM OF TEXTURE: " << sum << std::endl;
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