#pragma once
#include "cartpole.h"

class EpisodeObserver {
public:
    virtual ~EpisodeObserver() = default;
    virtual void onEpisodeStart(int ) {};
    virtual void onStep(const State &, int , int ) {};
    virtual void onEpisodeEnd(int , int ) {};
};