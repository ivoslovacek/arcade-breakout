#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "data.h"
#include "audio.h"

int ctov(char c)
{
    if (c == '1' || c == '2')
    {
        return c - 48;
    }
    else
    {
        return 0;
    }
}

int getTextureIndex(int row, int state)
{
    if (state == 1)
    {
        return row;
    }
    else if (state == 2)
    {
        return row + BRICKS_IN_ROWS;
    }
    else
    {
        return 0;
    }
}

void createBricksRect(Arguments *args)
{

    for (int i = 0; i < BRICKS_IN_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_IN_COLS; j++)
        {
            if (args->entities->bricks[j + (i * BRICKS_IN_COLS)].state != 0)
            {
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.x = BRICKS_WOFFSET + (j * BRICK_WIDTH);
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.y = BRICKS_HOFFSET + (i * BRICK_HEIGHT);
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.w = BRICK_WIDTH;
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.h = BRICK_HEIGHT;
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].texture_index = getTextureIndex(i, args->entities->bricks[j + (i * BRICKS_IN_COLS)].state);
            }
        }
    }
}

void loadBricksFromFile(Arguments *args, char *path)
{
    FILE *input;
    input = fopen(path, "r");
    if (input == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CONFIG ERROR", "Couldn't open config file! Default config will be used.", args->window);
        loadAllBricks(args);
        return;
    }

    args->entities->bricks = (Brick *)malloc(sizeof(Brick) * BRICKS_IN_COLS * BRICKS_IN_ROWS);
    if (args->entities->bricks == NULL)
    {
        printf("couldn't allocate memory");
        fclose(input);
        terminate_program(args);
    }

    args->entities->current_bricks = 0;

    for (int i = 0; i < BRICKS_IN_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_IN_COLS; j++)
        {
            args->entities->bricks[j + (i * BRICKS_IN_COLS)].state = ctov(fgetc(input));
            if (args->entities->bricks[j + (i * BRICKS_IN_COLS)].state >= 1)
            {
                args->entities->current_bricks++;
            }
        }

        fgetc(input);
    }

    fclose(input);
}

void loadAllBricks(Arguments *args)
{
    args->entities->bricks = (Brick *)malloc(sizeof(Brick) * BRICKS_IN_COLS * BRICKS_IN_ROWS);
    if (args->entities->bricks == NULL)
    {
        printf("couldn't allocate memory");
        terminate_program(args);
    }

    args->entities->current_bricks = 0;

    for (int i = 0; i < BRICKS_IN_ROWS; i++)
    {
        for (int j = 0; j < BRICKS_IN_COLS; j++)
        {
            args->entities->bricks[j + (i * BRICKS_IN_COLS)].state = 2;
            args->entities->current_bricks++;
        }
    }
}

void createBrickTextures(Arguments *args, char *path, char *path2)
{
    SDL_Surface *tmp = IMG_Load(path);
    if (tmp == NULL)
    {
        printf("couldn't open file");
        terminate_program(args);
    }

    args->entities->brick_textures = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * BRICKS_IN_ROWS * 2);
    if (args->entities->brick_textures == NULL)
    {
        printf("couldn't allocate memory");
        SDL_FreeSurface(tmp);
        terminate_program(args);
    }

    for (size_t i = 0; i < BRICKS_IN_ROWS; i++)
    {
        args->entities->brick_textures[i] = SDL_CreateTextureFromSurface(args->renderer, tmp);
        if (args->entities->brick_textures[i] == NULL)
        {
            printf("couldn't create texture");

            for (size_t j = 0; j < i; j++)
            {
                SDL_DestroyTexture(args->entities->brick_textures[j]);
            }

            free(args->entities->brick_textures);
            SDL_FreeSurface(tmp);
            terminate_program(args);
        }
    }
    SDL_FreeSurface(tmp);

    tmp = IMG_Load(path2);
    if (tmp == NULL)
    {
        for (size_t i = 0; i < BRICKS_IN_ROWS; i++)
        {
            SDL_DestroyTexture(args->entities->brick_textures[i]);
        }
        printf("couldn't open file");
        terminate_program(args);
    }

    for (size_t i = BRICKS_IN_ROWS; i < BRICKS_IN_ROWS * 2; i++)
    {
        args->entities->brick_textures[i] = SDL_CreateTextureFromSurface(args->renderer, tmp);
        if (args->entities->brick_textures[i] == NULL)
        {
            printf("couldn't create texture");

            for (size_t j = 0; j < i; j++)
            {
                SDL_DestroyTexture(args->entities->brick_textures[j]);
            }

            for (size_t j = BRICKS_IN_ROWS; j < i; j++)
            {
                SDL_DestroyTexture(args->entities->brick_textures[j]);
            }

            free(args->entities->brick_textures);
            SDL_FreeSurface(tmp);
            terminate_program(args);
        }
    }

    SDL_FreeSurface(tmp);

    SDL_SetTextureColorMod(args->entities->brick_textures[0], 148, 0, 211);
    SDL_SetTextureColorMod(args->entities->brick_textures[1], 75, 0, 130);
    SDL_SetTextureColorMod(args->entities->brick_textures[2], 0, 0, 255);
    SDL_SetTextureColorMod(args->entities->brick_textures[3], 0, 255, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[4], 255, 255, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[5], 255, 127, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[6], 255, 0, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[7], 148, 0, 211);
    SDL_SetTextureColorMod(args->entities->brick_textures[8], 75, 0, 130);
    SDL_SetTextureColorMod(args->entities->brick_textures[9], 0, 0, 255);
    SDL_SetTextureColorMod(args->entities->brick_textures[10], 0, 255, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[11], 255, 255, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[12], 255, 127, 0);
    SDL_SetTextureColorMod(args->entities->brick_textures[13], 255, 0, 0);
}

void freeBrickTextures(Arguments *args)
{
    for (size_t i = 0; i < BRICKS_IN_ROWS; i++)
    {
        SDL_DestroyTexture(args->entities->brick_textures[i]);
    }

    free(args->entities->brick_textures);
}

void initBricks(Arguments *args)
{
    switch (args->brick_config)
    {
    case 0:
        loadAllBricks(args);
        break;

    case 1:
        loadBricksFromFile(args, "config.txt");
        break;
    }

    createBricksRect(args);
    createBrickTextures(args, "brick.png", "brick2.png");
}

void freeBricks(Arguments *args)
{
    freeBrickTextures(args);
    free(args->entities->bricks);
    args->entities->bricks = NULL;
    args->entities->current_bricks = 0;
}

void terminate_program(Arguments *args)
{
    args->done = 1;
    args->scene = -1;
    pthread_exit(NULL);
}

void allocGameData(Arguments *args)
{
    args->done = 0;
    args->scene = INIT_MENU_SCENE;
    args->score = 0;
    args->lives = 2;
    args->brick_config = 1;
    args->audio_ready = SDL_FALSE;

    args->entities = (Entities *)malloc(sizeof(Entities));
    if (args->entities == NULL)
    {
        printf("couldn't allocate memory");
        terminate_program(args);
    }
}

void freeGameData(Arguments *args)
{
    free(args->entities);
    args->entities = NULL;
}

void initPaddle(Arguments *args)
{
    args->entities->paddle.w = PADDLE_WIDTH;
    args->entities->paddle.h = PADDLE_HEIGHT;
    args->entities->paddle.x = (WINDOW_WIDTH / 2) - (args->entities->paddle.w / 2);
    args->entities->paddle.y = WINDOW_HEIGHT - args->entities->paddle.h - 5;
}

void initBall(Arguments *args)
{
    time_t t;

    srand((unsigned int)time(&t));

    args->entities->ball.w = BALL_SIZE;
    args->entities->ball.h = args->entities->ball.w;
    args->entities->ball.x = args->entities->paddle.x + (args->entities->paddle.w / 2) - (args->entities->ball.w / 2);
    args->entities->ball.y = WINDOW_HEIGHT - args->entities->ball.h - 30 - (rand() % 70);
    args->entities->ball_xdirection = 1;
    args->entities->ball_ydirection = -1;
    args->entities->ball_speed = MIN_BALL_SPEED;
}

void initBorders(Arguments *args)
{
    args->entities->interface.border_count = 3;
    args->entities->interface.borders = (SDL_FRect *)malloc(sizeof(SDL_FRect) * args->entities->interface.border_count);
    if (args->entities->interface.borders == NULL)
    {
        terminate_program(args);
    }
    args->entities->interface.borders[0].w = BORDER_WIDTH;
    args->entities->interface.borders[0].h = WINDOW_HEIGHT - BORDER_Y;
    args->entities->interface.borders[0].x = 0;
    args->entities->interface.borders[0].y = BORDER_Y;
    args->entities->interface.borders[1].w = BORDER_WIDTH;
    args->entities->interface.borders[1].h = WINDOW_HEIGHT - BORDER_Y;
    args->entities->interface.borders[1].x = WINDOW_WIDTH - args->entities->interface.borders[1].w;
    args->entities->interface.borders[1].y = BORDER_Y;
    args->entities->interface.borders[2].w = WINDOW_WIDTH;
    args->entities->interface.borders[2].h = BORDER_WIDTH;
    args->entities->interface.borders[2].x = 0;
    args->entities->interface.borders[2].y = BORDER_Y;
}

void freeBordres(Arguments *args)
{
    free(args->entities->interface.borders);
    args->entities->interface.borders = NULL;
}

void initFont(Arguments *args)
{
    TTF_Init();

    args->font = TTF_OpenFont("arcadeclassic.ttf", 40);

    args->font_color.r = 125;
    args->font_color.g = 125;
    args->font_color.b = 125;
    args->font_color.a = 255;
}

void freeFont(Arguments *args)
{
    TTF_CloseFont(args->font);
    TTF_Quit();
}

void createGameTexts(Arguments *args)
{
    args->entities->interface.texts_textures = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * TEXT_COUNT);
    if (args->entities->interface.texts_textures == NULL)
        terminate_program(args);
    args->entities->interface.texts = (char **)malloc(sizeof(char *) * TEXT_COUNT);
    if (args->entities->interface.texts == NULL)
        terminate_program(args);
    args->entities->interface.text_pos = (SDL_Rect **)malloc(sizeof(SDL_Rect *) * TEXT_COUNT);
    if (args->entities->interface.text_pos == NULL)
        terminate_program(args);

    for (size_t i = 0; i < TEXT_COUNT; i++)
    {
        args->entities->interface.texts[i] = (char *)malloc(sizeof(char) * MAX_TEXT_LENGHT);
        if (args->entities->interface.texts[i] == NULL)
            terminate_program(args);
    }

    strcpy(args->entities->interface.texts[0], "SCORE : ");
    strcat(args->entities->interface.texts[0], "0");

    strcpy(args->entities->interface.texts[1], "LIFES : ");
    strcat(args->entities->interface.texts[1], "2");

    strcpy(args->entities->interface.texts[2], "PRESS ANY MOUSEBUTTON TO START");

    SDL_Surface *surface;

    for (size_t i = 0; i < TEXT_COUNT; i++)
    {
        surface = TTF_RenderText_Solid(args->font, args->entities->interface.texts[i], args->font_color);
        args->entities->interface.texts_textures[i] = SDL_CreateTextureFromSurface(args->renderer, surface);
        if (args->entities->interface.texts_textures[i] == NULL)
            terminate_program(args);
        SDL_FreeSurface(surface);
    }

    for (size_t i = 0; i < TEXT_COUNT; i++)
    {
        args->entities->interface.text_pos[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        if (args->entities->interface.text_pos[i] == NULL)
            terminate_program(args);

        TTF_SizeText(args->font, args->entities->interface.texts[i], &args->entities->interface.text_pos[i]->w, &args->entities->interface.text_pos[i]->h);
    }

    args->entities->interface.text_pos[0]->x = 0;
    args->entities->interface.text_pos[0]->y = (BORDER_Y / 2) - (args->entities->interface.text_pos[0]->h / 2);

    args->entities->interface.text_pos[1]->x = WINDOW_WIDTH - args->entities->interface.text_pos[1]->w;
    args->entities->interface.text_pos[1]->y = (BORDER_Y / 2) - (args->entities->interface.text_pos[1]->h / 2);

    args->entities->interface.text_pos[2]->w = 560;
    args->entities->interface.text_pos[2]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[2]->w / 2);
    args->entities->interface.text_pos[2]->y = BRICKS_HOFFSET + (BRICKS_IN_ROWS * BRICK_HEIGHT) + 100;

    args->entities->interface.rendered_texts = TEXT_COUNT;
}

void freeGameTexts(Arguments *args)
{
    for (size_t i = 0; i < TEXT_COUNT; i++)
    {
        free(args->entities->interface.texts[i]);
        free(args->entities->interface.text_pos[i]);
        SDL_DestroyTexture(args->entities->interface.texts_textures[i]);
    }

    free(args->entities->interface.text_pos);
    free(args->entities->interface.texts);
    free(args->entities->interface.texts_textures);
}

void ballMovement(Arguments *args, float frametime)
{
    args->entities->ball.x += args->entities->ball_speed * args->entities->ball_xdirection * frametime;
    args->entities->ball.y += args->entities->ball_speed * args->entities->ball_ydirection * frametime;
}

void changeBallSpeed(Arguments *args)
{
    if (args->entities->ball_speed < MAX_BALL_SPEED)
        args->entities->ball_speed += BALL_SPEED_CONST * MAX_BALL_SPEED;
    else
        args->entities->ball_speed = MAX_BALL_SPEED;
}

void generateBallXDirection(Arguments *args)
{
    int tmp;
    time_t t;

    srand((unsigned int)time(&t));

    tmp = rand() % 2;

    if (!tmp)
        tmp = -1;

    args->entities->ball_xdirection = tmp;
}

void changeGameEndText(Arguments *args)
{

    if (args->entities->current_bricks <= 0)
    {
        strcpy(args->entities->interface.texts[1], "YOU WON");
        strcpy(args->entities->interface.texts[2], "PRESS ANY BUTTON TO QUIT");
    }
    else
    {
        strcpy(args->entities->interface.texts[1], "YOU LOST");
        strcpy(args->entities->interface.texts[2], "PRESS ANY BUTTON TO QUIT");
    }
}

void createMenuTexts(Arguments *args)
{
    args->entities->interface.texts_textures = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * MENU_TEXT_COUNT);
    if (args->entities->interface.texts_textures == NULL)
        terminate_program(args);
    args->entities->interface.texts = (char **)malloc(sizeof(char *) * MENU_TEXT_COUNT);
    if (args->entities->interface.texts == NULL)
        terminate_program(args);
    args->entities->interface.text_pos = (SDL_Rect **)malloc(sizeof(SDL_Rect *) * MENU_TEXT_COUNT);
    if (args->entities->interface.text_pos == NULL)
        terminate_program(args);

    args->entities->interface.text_hover = (SDL_bool *)calloc(MENU_TEXT_COUNT, sizeof(SDL_bool));
    if (args->entities->interface.text_hover == NULL)
        terminate_program(args);

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        args->entities->interface.texts[i] = (char *)malloc(sizeof(char) * MAX_TEXT_LENGHT);
        if (args->entities->interface.texts[i] == NULL)
            terminate_program(args);
    }

    strcpy(args->entities->interface.texts[0], "BREAKOUT");
    strcpy(args->entities->interface.texts[1], "PLAY");
    strcpy(args->entities->interface.texts[2], "SETTINGS");
    strcpy(args->entities->interface.texts[3], "QUIT");

    SDL_Surface *surface;

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        surface = TTF_RenderText_Solid(args->font, args->entities->interface.texts[i], args->font_color);
        args->entities->interface.texts_textures[i] = SDL_CreateTextureFromSurface(args->renderer, surface);
        if (args->entities->interface.texts_textures[i] == NULL)
            terminate_program(args);
        SDL_FreeSurface(surface);
    }

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        args->entities->interface.text_pos[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        if (args->entities->interface.text_pos[i] == NULL)
            terminate_program(args);

        TTF_SizeText(args->font, args->entities->interface.texts[i], &args->entities->interface.text_pos[i]->w, &args->entities->interface.text_pos[i]->h);
    }

    args->entities->interface.text_pos[0]->w *= 2;
    args->entities->interface.text_pos[0]->h *= 2;
    args->entities->interface.text_pos[0]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[0]->w / 2);
    args->entities->interface.text_pos[0]->y = 20;

    args->entities->interface.text_pos[1]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[1]->w / 2);
    args->entities->interface.text_pos[1]->y = args->entities->interface.text_pos[0]->y + args->entities->interface.text_pos[0]->h + 200;

    args->entities->interface.text_pos[2]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[2]->w / 2);
    args->entities->interface.text_pos[2]->y = args->entities->interface.text_pos[1]->y + args->entities->interface.text_pos[1]->h + 100;

    args->entities->interface.text_pos[3]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[3]->w / 2);
    args->entities->interface.text_pos[3]->y = args->entities->interface.text_pos[2]->y + args->entities->interface.text_pos[2]->h + 100;

    args->entities->interface.rendered_texts = MENU_TEXT_COUNT;
}

void freeMenuTexts(Arguments *args)
{
    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        free(args->entities->interface.texts[i]);
        free(args->entities->interface.text_pos[i]);
        SDL_DestroyTexture(args->entities->interface.texts_textures[i]);
    }

    free(args->entities->interface.text_pos);
    free(args->entities->interface.texts);
    free(args->entities->interface.texts_textures);
    free(args->entities->interface.text_hover);
}

void createSettingsTexts(Arguments *args)
{
    args->entities->interface.texts_textures = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * MENU_TEXT_COUNT);
    if (args->entities->interface.texts_textures == NULL)
        terminate_program(args);
    args->entities->interface.texts = (char **)malloc(sizeof(char *) * MENU_TEXT_COUNT);
    if (args->entities->interface.texts == NULL)
        terminate_program(args);
    args->entities->interface.text_pos = (SDL_Rect **)malloc(sizeof(SDL_Rect *) * MENU_TEXT_COUNT);
    if (args->entities->interface.text_pos == NULL)
        terminate_program(args);

    args->entities->interface.text_hover = (SDL_bool *)calloc(MENU_TEXT_COUNT, sizeof(SDL_bool));
    if (args->entities->interface.text_hover == NULL)
        terminate_program(args);

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        args->entities->interface.texts[i] = (char *)malloc(sizeof(char) * MAX_TEXT_LENGHT);
        if (args->entities->interface.texts[i] == NULL)
            terminate_program(args);
    }

    strcpy(args->entities->interface.texts[0], "CONFIG SETTINGS:");
    strcpy(args->entities->interface.texts[1], "ALL BRICKS");
    strcpy(args->entities->interface.texts[2], "USER CONFIG");
    strcpy(args->entities->interface.texts[3], "BACK");

    SDL_Surface *surface;

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        surface = TTF_RenderText_Solid(args->font, args->entities->interface.texts[i], args->font_color);
        args->entities->interface.texts_textures[i] = SDL_CreateTextureFromSurface(args->renderer, surface);
        if (args->entities->interface.texts_textures[i] == NULL)
            terminate_program(args);
        SDL_FreeSurface(surface);
    }

    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        args->entities->interface.text_pos[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        if (args->entities->interface.text_pos[i] == NULL)
            terminate_program(args);

        TTF_SizeText(args->font, args->entities->interface.texts[i], &args->entities->interface.text_pos[i]->w, &args->entities->interface.text_pos[i]->h);
    }

    args->entities->interface.text_pos[0]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[0]->w / 2);
    args->entities->interface.text_pos[0]->y = 100;

    args->entities->interface.text_pos[1]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[1]->w / 2);
    args->entities->interface.text_pos[1]->y = args->entities->interface.text_pos[0]->y + args->entities->interface.text_pos[0]->h * 2 + 100;

    args->entities->interface.text_pos[2]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[2]->w / 2);
    args->entities->interface.text_pos[2]->y = args->entities->interface.text_pos[1]->y + args->entities->interface.text_pos[1]->h + 100;

    args->entities->interface.text_pos[3]->x = (WINDOW_WIDTH / 2) - (args->entities->interface.text_pos[3]->w / 2);
    args->entities->interface.text_pos[3]->y = args->entities->interface.text_pos[2]->y + args->entities->interface.text_pos[2]->h + 100;

    args->entities->interface.rendered_texts = MENU_TEXT_COUNT;
}

void freeSettingsTexts(Arguments *args)
{
    for (size_t i = 0; i < MENU_TEXT_COUNT; i++)
    {
        free(args->entities->interface.texts[i]);
        free(args->entities->interface.text_pos[i]);
        SDL_DestroyTexture(args->entities->interface.texts_textures[i]);
    }

    free(args->entities->interface.text_pos);
    free(args->entities->interface.texts);
    free(args->entities->interface.texts_textures);
    free(args->entities->interface.text_hover);
}

void loadSoundsToMemory(Arguments *args)
{
    args->sounds = (Audio **)malloc(sizeof(Audio *) * SOUND_COUNT);
    if (args->sounds == NULL)
    {
        printf("couldn't allocate memory");
    }
    char path[6];
    strcpy(path, "1.wav");

    for (size_t i = 0; i < SOUND_COUNT; i++)
    {
        args->sounds[i] = createAudio(path, 0, 64);
        if (args->sounds[i] == NULL)
        {
            printf("couldn't load sounds");
            terminate_program(args);
        }
        path[0]++;
    }
}

void freeSoundsFromMemory(Arguments *args)
{
    for (size_t i = 0; i < SOUND_COUNT; i++)
    {
        free(args->sounds[i]);
    }

    free(args->sounds);
}