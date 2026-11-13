#pragma once
#include "env/cartpole.h"
#include "evaluation/observer.h"
#include "strategies/strategy.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct EvalReport {
    std::string strategy;
    int         episodes;
    double      meanReturn, stdReturn, stdErrors;
    int         minReturn, maxReturn, medianReturn;
    double      successRate; // fraction of successful episodes that reached MAX_EPISODE_STEPS
};

class Evaluator {
    int              numEpisodes;
    unsigned         envSeed;
    EpisodeObserver *observer;

public:
    Evaluator(int numEpisodes, unsigned envSeed, EpisodeObserver *obs = nullptr)
        : numEpisodes(numEpisodes), envSeed(envSeed), observer(obs) {}

    EvalReport evaluate(Strategy &strategy) const {
        std::string strategyName = strategy.name();
        if (numEpisodes <= 0) {
            std::cerr << "No episodes to run\n";
            return {};
        }
        CartPole         env(envSeed);
        std::vector<int> returns(numEpisodes);

        int success = 0;
        for (int episode = 1; episode <= numEpisodes; episode++) {
            if (observer)
                observer->onEpisodeStart(episode);

            State      state = env.getState();
            StepResult result;
            int        r    = 0;
            int        step = 0;
            do {
                int action = strategy.act(state);
                result     = env.step(action);
                state      = result.state;
                r += result.reward;
                step++;

                if (observer)
                    observer->onStep(state, episode, step, strategyName);
            } while (!result.terminated && !result.truncated);

            if (!result.terminated)
                success++;

            returns[episode - 1] = r;
            if (observer)
                observer->onEpisodeEnd(episode, r);
            env.reset();
        }

        EvalReport report{};
        std::sort(returns.begin(), returns.end());

        report.strategy     = strategyName;
        report.episodes     = numEpisodes;
        report.meanReturn   = std::accumulate(returns.begin(), returns.end(), 0.0) / numEpisodes;
        report.medianReturn = returns[numEpisodes / 2];
        report.minReturn    = returns[0];
        report.maxReturn    = returns[numEpisodes - 1];
        report.stdReturn    = 0;
        
        for (auto x : returns)
            report.stdReturn += (x - report.meanReturn) * (x - report.meanReturn);
        report.stdReturn   = std::sqrt(report.stdReturn / (numEpisodes - 1));
        report.stdErrors   = report.stdReturn / std::sqrt(numEpisodes);
        report.successRate = success / (double)numEpisodes;
        return report;
    }
};

inline void printReport(const EvalReport &r) {
    std::cout << r.strategy << std::endl;
    std::cout << "Mean: " << r.meanReturn << std::endl;
    std::cout << "Median: " << r.medianReturn << std::endl;
    std::cout << "Std: " << r.stdReturn << std::endl;
    std::cout << "Min: " << r.minReturn << std::endl;
    std::cout << "Max: " << r.maxReturn << std::endl;
    std::cout << "Success: " << r.successRate << std::endl;
    std::cout << "StdErr: " << r.stdErrors << std::endl;
}