#pragma once
#include <cmath>
#include <numbers>
#include <random>

const double GRAVITY           = 9.8;                         // m/s^2
const double MASS_CART         = 1.0;                         // kg
const double MASS_POLE         = 0.1;                         // kg
const double TOTAL_MASS        = MASS_CART + MASS_POLE;       // kg
const double LENGTH            = 1.0;                         // m
const double HALF_LENGTH       = LENGTH / 2;                  // m
const double FORCE_MAG         = 10.0;                        // N
const double X_THRESHOLD       = 2.4;                         // m
const double THETA_THRESHOLD   = 12 * std::numbers::pi / 180; // rad
const double TAU               = 0.02;                        // s
const int    MAX_EPISODE_STEPS = 500;

// Cart-pole state structure
struct State {
    double x, vx, theta, omega;
};

// State of the cart-pole system after a step
struct StepResult {
    State  state;      // current state
    double reward;     // reward for this step
    bool   terminated; // if episode ended
    bool   truncated;  // if truncated early
};

// CartPole environment class
class CartPole {
    State        state;
    int          steps;
    std::mt19937 rng;
    // random start distribution for initial state, near upright so game is playable
    std::uniform_real_distribution<double> startDist{-0.05, 0.05};

public:
    CartPole(unsigned seed) { reset(seed); }

    StepResult step(int action) {
        double force = (action == 1) ? FORCE_MAG : -FORCE_MAG;
        double cos_t = cos(state.theta), sin_t = sin(state.theta);

        /*
            Analyzing the cart's motion
            Ma = force - H
            Ma = force - mx_com''       -- (1)


            Analyzing the rod's motion

            x_com is the positon of the center of mass of the rod
            x_com = x + (l/2)sinθ
            taking derivative
            x_com' = vx + (l/2)cosθ ω
            take derivative again for acceleration
            x_com'' = a - (l/2) sinθ ω^2 + (l/2) cosθ α         -- (2)

            putting (2) in (1), we get
            Ma = force - ma + m(l/2) sinθ ω^2 - m(l/2) cosθ α
            a = (force + m(l/2) sinθ ω^2 - m(l/2) cosθ α) / (m + M)
            let T = (force + m(l/2) sinθ ω^2) / (m + M)

            a = T - m(l/2) cosθ α / (m + M)                     -- (3)


            assuming I be the moment of inertia of the pole and axis of
            rotation is the hinge
            for rod, I = (1/3)ml^2

            Iα = mg(l/2)sinθ - ma(l/2)cosθ
            solving for α gives
            α = (gsinθ - a cosθ) / (4(l/2)/3)         -- (4)


            Solving the equations, we get

            α = (gsinθ - T cosθ) / (4(l/2)/3 - m(l/2)cos^2θ / (m + M))
            a = T - m(l/2) cosθ α / (m + M)

        */

        double temp =
            (force + MASS_POLE * HALF_LENGTH * sin_t * state.omega * state.omega) / TOTAL_MASS;

        double alpha = (GRAVITY * sin_t - temp * cos_t) /
                       (4 * HALF_LENGTH / 3 - MASS_POLE * HALF_LENGTH * cos_t * cos_t / TOTAL_MASS);
        double a     = temp - MASS_POLE * HALF_LENGTH * cos_t * alpha / TOTAL_MASS;

        // update state
        state.x += TAU * state.vx;
        state.vx += TAU * a;
        state.theta += TAU * state.omega;
        state.omega += TAU * alpha;

        // increment step count
        steps++;

        return StepResult{getState(), 1.0, isTerminated(), steps >= MAX_EPISODE_STEPS};
    }

    void reset() {
        state.x     = startDist(rng);
        state.vx    = startDist(rng);
        state.theta = startDist(rng);
        state.omega = startDist(rng);
        steps       = 0;
    }

    void reset(unsigned seed) {
        rng.seed(seed);
        reset();
    }

    bool isTerminated() const {
        return (state.x > X_THRESHOLD || state.x < -X_THRESHOLD || state.theta > THETA_THRESHOLD ||
                state.theta < -THETA_THRESHOLD);
    }

    State getState() const { return State{state.x, state.vx, state.theta, state.omega}; }
};