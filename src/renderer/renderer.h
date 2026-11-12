#pragma once
#include "raylib.h"
#include "env/cartpole.h"
#include "strategies/strategy.h"
#include "evaluation/observer.h"

const int ROPE_Y = 500;
const int SCREEN_HEIGHT = 600, SCREEN_WIDTH = 800;
const double SCALE = 100; // pixels per meter

inline void drawCartPole(const State &s) {
    float px = SCREEN_WIDTH / 2 + s.x * SCALE;
    Rectangle cart = {px - 20, ROPE_Y - 10, 40, 20};
    DrawRectangleRec(cart, WHITE);

    Rectangle pole = {px, ROPE_Y, 10, (float)(LENGTH * SCALE)};
    Vector2 origin = {5, (float)(LENGTH * SCALE)};
    DrawRectanglePro(pole, origin, s.theta * RAD2DEG, RED);
}

inline void drawTrack() {
    // white line for ground
    DrawLine(0, ROPE_Y, SCREEN_WIDTH, ROPE_Y, WHITE);

    // left and right limits
    DrawLine(SCREEN_WIDTH/2 - X_THRESHOLD * SCALE, ROPE_Y, SCREEN_WIDTH/2 - X_THRESHOLD * SCALE, ROPE_Y - 100, WHITE);
    DrawLine(SCREEN_WIDTH/2 + X_THRESHOLD * SCALE, ROPE_Y, SCREEN_WIDTH/2 + X_THRESHOLD * SCALE, ROPE_Y - 100, WHITE);

}

inline void watch(Strategy &s, unsigned seed, int fps = 50) {
    CartPole env(seed);
    const std::string windowTitle = "Strategy: " + s.name();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle.c_str());
    SetTargetFPS(fps);

    int episode = 1;
    int steps = 1;

    while (!WindowShouldClose()) {

        StepResult res = env.step(
            s.act(
                env.getState()
            )
        );
        
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("%s   episode %d   step %d", windowTitle.c_str(), episode, steps), 10, 10, 20, RAYWHITE);
        drawTrack();
        drawCartPole(res.state);
        
        EndDrawing();

        if (res.truncated || res.terminated) {
            env.reset();
            steps = 1;
            episode++;
        } else {
            steps++;
        }
    }
    CloseWindow();
}

class WindowObserver : public EpisodeObserver {
    bool draw = true;

public:
    explicit WindowObserver(int fps=50) {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Observer");
        SetTargetFPS(fps);
    }

    void onStep(const State &s, int episode, int step, const std::string &strategy) override {
        if (!draw) return;

        BeginDrawing();
        ClearBackground(BLACK);
        drawTrack();
        drawCartPole(s);
        DrawText(TextFormat("Strategy: %s\n\nEpisode: %d\nStep: %d  ", strategy.c_str(), episode, step), 10, 10, 20, RAYWHITE);
        EndDrawing();

        if (WindowShouldClose()) {
            draw = false;
            CloseWindow();
        }
    }

    ~WindowObserver() {
        if (draw) CloseWindow();
    }
};