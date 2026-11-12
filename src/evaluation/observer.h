#pragma once
#include "env/cartpole.h"
#include <string>

class EpisodeObserver {
public:
    virtual ~EpisodeObserver() = default;
    virtual void onEpisodeStart(int ) {};
    virtual void onStep(const State &, int , int , const std::string &) {};
    virtual void onEpisodeEnd(int , int ) {};
};