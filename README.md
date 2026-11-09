# cart-pole

A C++ implementation of the classic **cart-pole** balancing problem. It's rendered with [raylib](https://www.raylib.com/).

You write a **strategy** (a rule or agent that picks "push left" or "push right" each step), measure it over many episodes with the **evaluator**, and watch it in a window while it runs.

## Features

- **Gym-compatible environment:** the same equations, Euler integration, start-state distribution, termination limits and 500-step limit as `CartPole-v1`.
- **Pluggable strategies:** inherit from `Strategy` and implement `act()` and `name()`.
- **Evaluator:** runs N seeded episodes and reports the mean, median, standard deviation, standard error, min, max and success rate.
- **Watching:** an observer draws each step in a raylib window. If you close the window, the run continues without drawing.

## Requirements

- A C++20 compiler (clang or GCC)
- [raylib](https://github.com/raysan5/raylib), developed against raylib 6.2
- `make`

On macOS:
```sh
brew install raylib
```

On Linux, install raylib from your package manager or build it from source. Depending on how it's installed, you may need to add raylib's system libraries (for example `-lGL -lm -lpthread -ldl -lrt -lX11`) to `LDLIBS` in `env/Makefile`.

## Build and run

```sh
cd env
make run
```

`make run` builds the program (only if a file changed) and runs it. `make clean` removes the compiled program.

The current `main.cpp` evaluates the random strategy over 1000 episodes while drawing them in a window. Close the window to skip the animation; the evaluation keeps running and prints its report:

```
Random
Mean: 21.998
Median: 19
Std: 11.8321
Min: 8
Max: 105
Success: 0
StdErr: 0.374163
```

## Project structure

```
env/
├── cartpole.h          environment: physics constants, State, StepResult, CartPole
├── strategy.h          Strategy base class (the interface every strategy implements)
├── strategies/         strategy implementations
│   └── random_strategy.h
├── evaluator.h         Evaluator, EvalReport, printReport (no raylib)
├── observer.h          EpisodeObserver: hooks called during a run (no raylib)
├── renderer.h          raylib drawing, the Observer window and watch()
├── main.cpp            entry point
└── Makefile
```

Only `renderer.h` and `main.cpp` use raylib. The environment, strategies and evaluator are plain C++, so they work without a display.

## The environment

| | |
|---|---|
| **State** | `x` (cart position, m), `vx` (cart velocity, m/s), `theta` (pole angle from vertical, rad, positive = leaning right), `omega` (angular velocity, rad/s) |
| **Actions** | `0` = push left, `1` = push right, both with 10 N. There is no "no push" action. |
| **Reward** | `+1` for every step, including the last one |
| **Episode ends** (`terminated`) | \|`theta`\| > 12° or \|`x`\| > 2.4 m |
| **Time limit** (`truncated`) | 500 steps |
| **Start state** | each of the four values drawn uniformly from [-0.05, 0.05] |
| **Time step** | 0.02 s, explicit Euler integration |

The derivation of the equations of motion is written out as a comment in `cartpole.h`.

```cpp
CartPole env(42);                    // seed for the start states
State s = env.getState();
StepResult r = env.step(1);          // push right
// r.state, r.reward, r.terminated, r.truncated
env.reset();                         // new random start state
```

## Writing a strategy

Create a header in `env/strategies/`. For example, push in the direction the pole is leaning:

```cpp
#pragma once
#include "../strategy.h"

class LeanStrategy : public Strategy {
public:
    int act(const State& s) override { return s.theta > 0 ? 1 : 0; }
    std::string name() const override { return "Lean"; }
};
```

Then evaluate it in `main.cpp`:

```cpp
#include "evaluator.h"
#include "strategies/lean_strategy.h"

int main() {
    LeanStrategy lean;
    Evaluator evaluator(1000, 42);       // 1000 episodes, environment seed 42
    printReport(evaluator.evaluate(lean));
}
```

The Makefile picks up any header in `strategies/` automatically.

## Watching a strategy

Pass an `Observer` to the evaluator to draw every step:

```cpp
Observer window(50);                     // 50 FPS = real time; lower is slow motion
Evaluator evaluator(5, 42, &window);
printReport(evaluator.evaluate(strategy));
```

At 50 FPS, many episodes take a long time to watch, so use a small episode count for watching and a separate evaluator without an observer for full evaluations. Watching doesn't change the results: the observer only reads the state.

`watch(strategy, seed, fps)` in `renderer.h` plays episodes endlessly until you close the window.

## Reproducibility

- The **environment seed** (`Evaluator(numEpisodes, envSeed)`) fixes the sequence of start states, so every strategy evaluated with the same seed faces the same episodes.
- Strategies with their own randomness take their own seed, e.g. `RandomStrategy(420)` (420 is the default).
- A strategy object keeps its random generator between runs. Use a **fresh** object when you want to repeat exactly the same episodes.

## Reference results

1000 episodes, environment seed 42. These are simple rules you can write in one line of `act()`; only the random strategy is included in the repo.

| Strategy | Rule: push right if… | Mean return | Reaches 500 |
|---|---|---|---|
| Always right | always | 9.4 | 0% |
| Random | 50% chance | 22.0 | 0% |
| Lean | `theta > 0` | 42.6 | 0% |

Gymnasium considers `CartPole-v1` solved at an average return of 475 over 100 episodes.

## Next steps

- More built-in strategies, including a linear strategy with configurable weights
- Evaluator: record why each episode ended (pole angle vs. cart position) and how steady the control was
- Learning: hill climbing over linear weights, then Q-learning
- Python bindings (pybind11) with a Gymnasium-compatible wrapper, validated step by step against `CartPole-v1`
