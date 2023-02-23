#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "audio.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 700
#define BRICKS_WOFFSET 20
#define BRICKS_HOFFSET 200
#define BRICKS_IN_ROWS 7
#define BRICKS_IN_COLS 14
#define BRICK_WIDTH 40
#define BRICK_HEIGHT 25
#define BORDER_Y 100
#define BORDER_WIDTH 20
#define PADDLE_WIDTH 60
#define PADDLE_HEIGHT 10
#define BALL_SIZE 8
#define MIN_PADDLE_X 20
#define MAX_PADDLE_X 520
#define MAX_TEXT_LENGHT 101
#define TEXT_COUNT 3
#define MENU_TEXT_COUNT 4
#define INVALID_SCENE -1
#define INIT_SCENE 0
#define WAIT_SCENE 1
#define GAME_SCENE 2
#define END_SCENE 3
#define FREE_SCENE 4
#define INIT_MENU_SCENE 10
#define MENU_SCENE 11
#define FREE_MENU_SCENE 12
#define INIT_SETTINGS_SCENE 13
#define SETTINGS_SCENE 14
#define FREE_SETTINGS_SCENE 15
#define MIN_BALL_SPEED 300
#define MAX_BALL_SPEED 500
#define BALL_SPEED_CONST 0.02
#define SOUND_COUNT 8

typedef struct
{
    SDL_FRect *borders;
    int border_count;
    char **texts;
    SDL_Texture **texts_textures;
    SDL_Rect **text_pos;
    SDL_bool *text_hover;
    size_t rendered_texts;
} Interface;

typedef struct
{
    SDL_Rect parameters;
    int state;
    int texture_index;
} Brick;

typedef struct
{
    SDL_FRect ball;
    float ball_speed;
    int ball_xdirection;
    int ball_ydirection;
    SDL_FRect previous_paddle;
    SDL_FRect paddle;
    Interface interface;
    Brick *bricks;
    int current_bricks;
    SDL_Texture **brick_textures;
    int complete[3];
} Entities;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Audio **sounds;
    TTF_Font *font;
    SDL_Color font_color;
    Entities *entities;
    int score;
    int lives;
    SDL_bool game_stat_change;
    SDL_bool audio_ready;
    unsigned char brick_config;
    unsigned char done;
    unsigned char scene;
    SDL_Point cursor;
} Arguments;

void terminate_program(Arguments *args);

void initBricks(Arguments *args);

void freeBricks(Arguments *args);

void createBricksRect(Arguments *args);

void createBrickTextures(Arguments *args, char *path, char *path2);

void freeBrickTextures(Arguments *args);

void allocGameData(Arguments *args);

void freeGameData(Arguments *args);

void initPaddle(Arguments *args);

void initBall(Arguments *args);

void initBorders(Arguments *args);

void freeBordres(Arguments *args);

int getTextureIndex(int row, int state);

void initFont(Arguments *args);

void freeFont(Arguments *args);

void createGameTexts(Arguments *args);

void freeGameTexts(Arguments *args);

void ballMovement(Arguments *args, float frametime);

void changeBallSpeed(Arguments *args);

void generateBallXDirection(Arguments *args);

void changeGameEndText(Arguments *args);

void loadAllBricks(Arguments *args);

void loadBricksFromFile(Arguments *args, char *path);

void createMenuTexts(Arguments *args);

void freeMenuTexts(Arguments *args);

void createSettingsTexts(Arguments *args);

void freeSettingsTexts(Arguments *args);

void loadSoundsToMemory(Arguments *args);

void freeSoundsFromMemory(Arguments *args);