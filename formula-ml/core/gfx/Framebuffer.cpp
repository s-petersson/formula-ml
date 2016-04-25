#include "Framebuffer.h"

namespace gfx {
	Framebuffer create_framebuffer(int width, int height, bool multisample) {
		Framebuffer fbo;
		fbo.width = width;
		fbo.height = height;

		int samples;
		glGetIntegerv(GL_MAX_SAMPLES, &samples);
		glGenFramebuffers(1, &fbo.id);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);

		glGenRenderbuffers(1, &fbo.color_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, fbo.color_buffer);
		if (multisample) {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, fbo.width, fbo.height);
		}
		else {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, fbo.width, fbo.height);
		}


		glGenRenderbuffers(1, &fbo.depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, fbo.depth_buffer);
		if (multisample) {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, fbo.width, fbo.height);
		}
		else {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo.width, fbo.height);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo.color_buffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.depth_buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return fbo;
	}


}