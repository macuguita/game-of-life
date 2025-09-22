#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#ifndef _WIN32
    #include <unistd.h>
#endif
#include "raylib.h"

#ifdef _WIN32
    #define MAX_FILE_PATH 256
#elif __linux__
    #define MAX_FILE_PATH 4096
#elif __APPLE__
    #define MAX_FILE_PATH 1024
#else
    #define MAX_FILE_PATH 4096
#endif

#include "./rainbow_shader.h"

#define GAME_WIDTH 128
#define GAME_HEIGHT 128

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#if 0
int GAME[GAME_HEIGHT][GAME_WIDTH] = {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
};
#endif
int GAME[GAME_HEIGHT][GAME_WIDTH];

void iterate_game(void);
void print_game(void);
void randomize_game(void);
void empty_game(void);
int get_alive_neighbor_count(int x, int y);

int main(void) {

    srand((unsigned int)time(NULL));

    randomize_game();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of life");
    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

    Shader rainbow = LoadShaderFromMemory(0, rainbow_frag);

    SetTargetFPS(30);

    bool paused = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        if (IsKeyPressed(KEY_R)) randomize_game();
        if (IsKeyPressed(KEY_E)) empty_game();
        Vector2 mouse_pos = GetMousePosition();
        static int paintValue = -1;

        float square_width = (float)GetScreenWidth() / GAME_WIDTH;
        float square_height = (float)GetScreenHeight() / GAME_HEIGHT;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int cellX = mouse_pos.x / square_width;
            int cellY = mouse_pos.y / square_height;

            paintValue = GAME[cellY][cellX] ? 0 : 1;
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && paintValue != -1) {
            int cellX = mouse_pos.x / square_width;
            int cellY = mouse_pos.y / square_height;

            GAME[cellY][cellX] = paintValue;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            paintValue = -1;
        }
        BeginTextureMode(target); {
            ClearBackground(BLACK);

            for (int y = 0; y < GAME_HEIGHT; y++) {
                for (int x = 0; x < GAME_WIDTH; x++) {
                    if (GAME[y][x]) {
                        DrawPixel(x, y, WHITE);
                    }
                }
            }

        } EndTextureMode();
        BeginDrawing(); {
            ClearBackground(BLACK);
            BeginShaderMode(rainbow); {
                DrawTexturePro(
                    target.texture,
                    (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                    );
            } EndShaderMode();
        } EndDrawing();
        //print_game();
        if (!paused)
            iterate_game();
    }

    UnloadShader(rainbow);
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}

int get_alive_neighbor_count(int x, int y) {
    int count = 0;

    if (x < 0 || x >= GAME_WIDTH || y < 0 || y >= GAME_HEIGHT) {
        fprintf(stderr, "ERROR: x or y is bigger than GAME_WIDTH or GAME_HEIGHT");
        abort();
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + GAME_WIDTH) % GAME_WIDTH;
            int ny = (y + j + GAME_HEIGHT) % GAME_HEIGHT;

            if (GAME[ny][nx])
                count++;
        }
    }

    return count;
}

void iterate_game() {
    int new_game[GAME_HEIGHT][GAME_WIDTH];

    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            int neighbors_count = get_alive_neighbor_count(x, y);

            if (GAME[y][x]) {
                if (neighbors_count < 2)
                    new_game[y][x] = 0;
                else if (neighbors_count > 3)
                    new_game[y][x] = 0;
                else
                    new_game[y][x] = 1;
            } else {
                if (neighbors_count == 3)
                    new_game[y][x] = 1;
                else
                    new_game[y][x] = 0;
            }
        }
    }
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            GAME[y][x] = new_game[y][x];
        }
    }
}

void print_game() {

    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            if (GAME[y][x])
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }
    printf("\n");
}

void randomize_game() {
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
                GAME[y][x] = rand() % 2;
        }
    }
}

void empty_game() {
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            GAME[y][x] = 0;
        }
    }
}
