#include "raylib.h"

int main(void)
{
    InitWindow(800, 450, "raylib + xmake demo");
    SetTargetFPS(60);

    Vector2 ballPosition = {400, 225};
    while (!WindowShouldClose())
    {
        // update
        ballPosition.x += (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)) * 4;
        ballPosition.y += (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * 4;

        // draw
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("raylib + xmake demo", 190, 20, 20, DARKGRAY);
            DrawCircleV(ballPosition, 20, MAROON);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
