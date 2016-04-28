#include "RacelineLogger.h"
#include "RacelineRenderer.h"
#include <core/gfx/Framebuffer.h>
#include <core/util/ImageIO.h>

void save_raceline(Simulator * simulator, const std::string& loc) {
	
   // auto renderer = make_unique<RacelineRenderer>(simulator);
    auto ms_fbo = gfx::create_framebuffer(1920,1080, true);
    auto final_fbo = gfx::create_framebuffer(1920, 1080, false);
    //Run simulation until the car completion and update the raceline curve.
    /*
	while (!simulator->has_terminated()) {
        simulator->update(0.01f);
        renderer->update(0.01f);
    }
	*/
    //Render to multisampled fbo
    glBindFramebuffer(GL_FRAMEBUFFER, ms_fbo.id);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT| GL_STENCIL_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    //renderer->render();

    //Blit renderered image to normal fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, final_fbo.id);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_fbo.id);
    glBlitFramebuffer(0, 0, ms_fbo.width, ms_fbo.height, 0, 0, ms_fbo.width, ms_fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //Save normal fbo to file
	util::save_framebuffer(final_fbo, loc);

    //Cleanup
    gfx::destroy_framebuffer(ms_fbo);
    gfx::destroy_framebuffer(final_fbo);
	
}

void RacelineLogger::add_job(RacelineLoggerJob task) {
	work_mutex.lock();
	job_queue.push_back(task);
	work_mutex.unlock();
}

void RacelineLogger::process_jobs() {
	work_mutex.lock();
	for (auto && job : job_queue) {
		save_raceline(job.evaluator->getSimulator(), job.location);
	}
	job_queue.clear();
	work_mutex.unlock();
}