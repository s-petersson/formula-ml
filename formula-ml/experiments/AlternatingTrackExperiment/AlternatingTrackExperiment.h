#ifndef ALTERNATING_TRACK_EXPERIMENT
#define ALTERNATING_TRACK_EXPERIMENT

#include <experiments/Experiment.h>
#include <experiments/ExperimentWindow.h>
#include <experiments/SimulationEvaluator.h>

#include <neural/neat/Trainer.h>
#include <neural/neat/Network.h>

class AlternatingTrackExperiment : public Experiment {
public:
    AlternatingTrackExperiment();
    AlternatingTrackExperiment(string path);
    ~AlternatingTrackExperiment();
    virtual void run();

private:
    string load_network_path;

    std::shared_ptr<neat::Trainer> trainer;
    std::shared_ptr<ExperimentWindow> window;

    class _Evaluator : public neat::Evaluator {
    public:
        _Evaluator(AiSettings ai_settings);
        ~_Evaluator();

        EvaluationResult evaluate_network(neat::Network* network);
        static void print(const EvaluationResult& result);
        void reset();

        std::vector<SimulationEvaluator*> evaluators;

        static int current_evaluator;
        static int current_evaluator_count;
    };
};

#endif