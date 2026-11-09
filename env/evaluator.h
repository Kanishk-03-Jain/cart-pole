#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include "cartpole.h"
#include "strategy.h"
#include <vector>
#include <numeric>
#include <algorithm>

struct EvalReport {
    std::string strategy;
    int episodes;
    double meanReturn, stdReturn, stdErrors;
    int minReturn, maxReturn, medianReturn;
    double successRate;     // fraction of successful episodes that reached MAX_EPISODE_STEPS
};

class Evaluator {
    int numEpisodes;
    unsigned envSeed;

public:
    Evaluator(int numEpisodes, unsigned envSeed) : numEpisodes(numEpisodes), envSeed(envSeed) {}

    EvalReport evaluate(Strategy &strategy) const {
        if (numEpisodes <= 0) {
            std::cerr << "No episodes to run\n";
            return {};
        }
        CartPole env(envSeed);
        std::vector<int> returns(numEpisodes);

        int success = 0;
        for (int episode = 0; episode < numEpisodes; episode++) {
            State state = env.getState();
            StepResult result;
            int r = 0;
            do {
                int action = strategy.act(state);
                result = env.step(action);
                state = result.state;
                r += result.reward;
            } while (!result.terminated && !result.truncated);

            if (!result.terminated) success++;

            returns[episode] = r;
            env.reset();
        }

        EvalReport report{};
        report.strategy = strategy.name();
        report.episodes = numEpisodes;
        report.meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / numEpisodes;
        std::sort(returns.begin(), returns.end());
        report.medianReturn = returns[numEpisodes/2];
        report.minReturn = returns[0];
        report.maxReturn = returns[numEpisodes-1];
        report.stdReturn = 0;
        for (auto x : returns) report.stdReturn += (x - report.meanReturn) * (x - report.meanReturn);
        report.stdReturn = std::sqrt(report.stdReturn / (numEpisodes - 1));
        report.stdErrors = report.stdReturn / std::sqrt(numEpisodes);
        report.successRate = success / (double) numEpisodes;
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