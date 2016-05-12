#ifndef VisualisePool_h
#define VisualisePool_h

#include <neural/neat/Network.h>
#ifndef CLOUD_COMPUTING
#include <experiments/VisualisationWindow.h>
#endif

using namespace neat;

class VisualisePool : public Experiment {
public:
    VisualisePool(string pool_path);
    ~VisualisePool();

    virtual void run();

    // Manditory
    AiSettings ai_settings;
    SimulatorSettings sim_settings;

private:
    string pool_path;

    Pool * pool;
    neural::FileWriter * fileWriter;

#ifndef CLOUD_COMPUTING
    std::shared_ptr<VisualisationWindow> window;
#endif
};

#endif