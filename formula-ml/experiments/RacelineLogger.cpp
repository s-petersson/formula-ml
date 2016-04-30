#include "RacelineLogger.h"
#include <core/util/ImageIO.h>

void RacelineLogger::save_raceline(const std::string& loc) {
    auto sim = evaluator->getSimulator();
    //Run simulation until the car completion and update the raceline curve.
	while (!sim->has_terminated()) {
        sim->update(0.01f);
        renderer->update(0.01f);
    }
	
    //Render to multisampled fbo
    glBindFramebuffer(GL_FRAMEBUFFER, ms_fbo.id);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT| GL_STENCIL_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1920, 1080);
    renderer->render();

    //Blit renderered image to normal fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, final_fbo.id);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_fbo.id);
    glBlitFramebuffer(0, 0, ms_fbo.width, ms_fbo.height, 0, 0, ms_fbo.width, ms_fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    //Save normal fbo to file
	util::save_framebuffer(final_fbo, loc);
}

RacelineLogger::RacelineLogger(SimulationEvaluator * _eval) {
    evaluator = _eval;
}

void RacelineLogger::init() {
    renderer = make_unique<RacelineRenderer>(evaluator->getSimulator());
    ms_fbo = gfx::create_framebuffer(1920, 1080, true);
    final_fbo = gfx::create_framebuffer(1920, 1080, false);
}

RacelineLogger::~RacelineLogger() {
    if (evaluator) delete evaluator;
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
        neat::Network* nw = new neat::Network(job.genome.genes);
        *evaluator->getNetworkLocation() = nw;
        save_raceline(job.location);
        evaluator->reset();
        renderer->reset();
    }
    job_queue.clear();
	work_mutex.unlock();
}