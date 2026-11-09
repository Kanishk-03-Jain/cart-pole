#pragma once
#include "strategy.h"

class LeanStrategy : public Strategy {
public:
    int act(const State &s) override {
        return s.theta > 0 ? 1 : 0;
    }
    std::string name() const override { return "Lean"; }
};