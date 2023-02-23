#include <SDL2/SDL.h>
#include <stdio.h>
#include <pthread.h>
#include "data.h"
#include "render.h"

void renderBricks(Arguments *args)
{
    for (int i = 0; i < BRICKS_IN_COLS * BRICKS_IN_ROWS; i++)
    {
        if (args->entities->bricks[i].state != 0)
        {
            SDL_RenderCopy(args->renderer, args->entities->brick_textures[args->entities->bricks[i].texture_index], NULL, &args->entities->bricks[i].parameters);
        }
    }
}

void renderBorders(Arguments *args)
{
    for (size_t i = 0; i < 3; i++)
    {
        SDL_SetRenderDrawColor(args->renderer, 128, 128, 128, 255);
        SDL_RenderFillRectsF(args->renderer, args->entities->interface.borders, args->entities->interface.border_count);
    }
}

void renderPlayerAndBall(Arguments *args)
{
    SDL_SetRenderDrawColor(args->renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(args->renderer, &args->entities->paddle);
    SDL_RenderFillRectF(args->renderer, &args->entities->ball);
}

void renderTextsGame(Arguments *args)
{
    for (size_t i = 0; i < args->entities->interface.rendered_texts; i++)
    {
        SDL_RenderCopy(args->renderer, args->entities->interface.texts_textures[i], NULL, args->entities->interface.text_pos[i]);
    }
}

void renderGameGraphics(Arguments *args)
{
    SDL_SetRenderDrawColor(args->renderer, 0, 0, 0, 255);
    SDL_RenderClear(args->renderer);

    renderBricks(args);
    renderPlayerAndBall(args);
    renderBorders(args);
    renderTextsGame(args);

    SDL_RenderPresent(args->renderer);
}

void renderMenuGraphics(Arguments *args)
{
    SDL_SetRenderDrawColor(args->renderer, 0, 0, 0, 255);
    SDL_RenderClear(args->renderer);

    renderTextsGame(args);

    SDL_RenderPresent(args->renderer);
}

void updateTextTexture(Arguments *args)
{
    SDL_Surface *surface;

    for (size_t i = 0; i < args->entities->interface.rendered_texts; i++)
    {
        if (i != 2)
            TTF_SizeText(args->font, args->entities->interface.texts[i], &args->entities->interface.text_pos[i]->w, &args->entities->interface.text_pos[i]->h);
        surface = TTF_RenderText_Solid(args->font, args->entities->interface.texts[i], args->font_color);
        args->entities->interface.texts_textures[i] = SDL_CreateTextureFromSurface(args->renderer, surface);
        if (args->entities->interface.texts_textures[i] == NULL)
            terminate_program(args);
        SDL_FreeSurface(surface);
    }
}

void changeTextTexture(Arguments *args)
{

    sprintf(args->entities->interface.texts[0] + 8, "%d", args->score);
    sprintf(args->entities->interface.texts[1] + 8, "%d", args->lives);

    updateTextTexture(args);
}

void *renderMain(void *ptr)
{
    Arguments *args = (Arguments *)ptr;

    createGraphicsEnviroment(args);
    initFont(args);

    Uint64 start;
    Uint64 end;
    float elapsed;

    while (!args->audio_ready)
        ;

    while (!args->done)
    {
        switch (args->scene)
        {
        case INIT_SCENE:
            initBricks(args);
            initPaddle(args);
            initBall(args);
            initBorders(args);
            createGameTexts(args);

            args->scene = WAIT_SCENE;
            break;

        case WAIT_SCENE:
            args->entities->interface.rendered_texts = TEXT_COUNT;
            args->game_stat_change = SDL_TRUE;

            while (args->scene == WAIT_SCENE)
            {
                if (args->game_stat_change)
                {
                    changeTextTexture(args);
                    args->game_stat_change = SDL_FALSE;
                }

                renderGameGraphics(args);
            }

            break;

        case GAME_SCENE:
            args->entities->interface.rendered_texts = 2;
            args->game_stat_change = SDL_TRUE;

            while (args->scene == GAME_SCENE)
            {
                start = SDL_GetPerformanceCounter();

                renderGameGraphics(args);

                end = SDL_GetPerformanceCounter();
                elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

                ballMovement(args, elapsed);

                if (args->game_stat_change)
                {
                    changeTextTexture(args);
                    args->game_stat_change = SDL_FALSE;
                }
            }
            break;

        case END_SCENE:
            args->entities->interface.rendered_texts = TEXT_COUNT;

            changeGameEndText(args);
            args->game_stat_change = SDL_TRUE;

            while (args->scene == END_SCENE)
            {
                if (args->game_stat_change)
                {
                    updateTextTexture(args);

                    args->game_stat_change = SDL_FALSE;
                }

                renderGameGraphics(args);
            }

        case FREE_SCENE:
            freeBricks(args);
            freeBordres(args);
            freeGameTexts(args);

            args->scene = -1;
            args->done = 1;
            break;

        case INIT_MENU_SCENE:
            createMenuTexts(args);

            args->scene = MENU_SCENE;
            break;

        case MENU_SCENE:
            while (args->scene == MENU_SCENE)
            {
                renderMenuGraphics(args);
            }

        case FREE_MENU_SCENE:
            freeMenuTexts(args);

            break;

        case INIT_SETTINGS_SCENE:
            createSettingsTexts(args);
            args->scene = SETTINGS_SCENE;
            break;

        case SETTINGS_SCENE:
            while (args->scene == SETTINGS_SCENE)
            {
                renderMenuGraphics(args);
            }

        case FREE_SETTINGS_SCENE:
            freeSettingsTexts(args);

            break;

        default:
            break;
        }
    }
    args->audio_ready = SDL_FALSE;

    freeFont(args);
    quitGraphicsEnviroment(args);
}

void createGraphicsEnviroment(Arguments *args)
{
    args->window = SDL_CreateWindow(
        "BREAKOUT",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (args->window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        args->done = 1;
        args->scene = -1;
        pthread_exit(NULL);
    }

    args->renderer = SDL_CreateRenderer(args->window, -1, SDL_RENDERER_ACCELERATED);

    if (args->renderer == NULL)
    {
        printf("Could not create a renderer: %s\n", SDL_GetError());
        args->done = 1;
        SDL_DestroyWindow(args->window);
        pthread_exit(NULL);
    }

    SDL_Init(SDL_INIT_VIDEO);
}

void quitGraphicsEnviroment(Arguments *args)
{
    SDL_DestroyRenderer(args->renderer);
    SDL_DestroyWindow(args->window);
    while (!args->audio_ready)
        ;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}