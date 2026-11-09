#include <random>
#include "cartpole.h"
#include "renderer.h"
#include "strategy.h"
#include "evaluator.h"
#include "strategies/random_strategy.h"

int main() {
    // InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cart Pole Simulation");


    // SetTargetFPS(50);
    // while (!WindowShouldClose()) {

    //     StepResult res = cartPoleEnv->step(dist(rng));
        
    //     BeginDrawing();
    //     ClearBackground(BLACK);
        
    //     drawTrack();
    //     drawCartPole(cartPoleEnv->getState());
        
    //     EndDrawing();

    //     if (res.truncated || res.terminated) cartPoleEnv->reset();
    // }
    // CloseWindow();
    // delete cartPoleEnv;
    RandomStrategy s;
    Evaluator e(1000, 42);
    printReport(e.evaluate(s));
    return 0;
}