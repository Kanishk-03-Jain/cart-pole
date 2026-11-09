#pragma once
#include "env/cartpole.h"
#include <string>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int act(const State &) = 0; // returns 0 or 1 for action
    virtual std::string name() const = 0;     // name of strategy
};