#ifndef MULTIPLE_TRACK_EXPERIMENT
#define MULTIPLE_TRACK_EXPERIMENT

#include <functional>
#include <memory>

#include <experiments/Experiment.h>
#include <experiments/SimulationEvaluator.h>

#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>

#ifndef CLOUD_COMPUTING
#include <experiments/ExperimentWindow.h>
#endif



class MultipleTrackExperiment : public Experiment {
public:
    MultipleTrackExperiment();
    MultipleTrackExperiment(string path);
    ~MultipleTrackExperiment();
    virtual void run();

private:
    string load_network_path;

    std::shared_ptr<neat::Trainer> trainer;
#ifndef CLOUD_COMPUTING
    std::shared_ptr<ExperimentWindow> window;
#endif

    class _Evaluator : public neat::Evaluator{
    public:
        _Evaluator(AiSettings ai_settings);
        ~_Evaluator();

        EvaluationResult evaluate_network(neat::Network* network);
        static void print(const EvaluationResult& result);
        void reset();

        std::vector<SimulationEvaluator*> evaluators;
    };
};

#endif