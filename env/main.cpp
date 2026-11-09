#include <random>
#include "cartpole.h"
#include "renderer.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cart Pole Simulation");

    CartPole *cartPoleEnv = new CartPole(42);

    std::mt19937 rng(420);
    std::uniform_int_distribution<int> dist(0, 1);

    SetTargetFPS(50);
    while (!WindowShouldClose()) {

        StepResult res = cartPoleEnv->step(dist(rng));
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        drawTrack();
        drawCartPole(cartPoleEnv->getState());
        
        EndDrawing();

        if (res.truncated || res.terminated) cartPoleEnv->reset();
    }
    CloseWindow();
    delete cartPoleEnv;
    return 0;
}