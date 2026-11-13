#pragma once
#include "strategies/strategy.h"

class RandomStrategy : public Strategy {
    std::mt19937                       rng;
    std::uniform_int_distribution<int> dist{0, 1};

public:
    explicit RandomStrategy(unsigned seed = 420) : rng(seed) {}

    std::string name() const override { return "Random"; }

    int act(const State &) override { return dist(rng); }
};