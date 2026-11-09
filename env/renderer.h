#pragma once
#include "raylib.h"
#include "cartpole.h"

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