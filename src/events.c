#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "events.h"
#include "data.h"

void *handleEvents(void *args)
{
    Arguments *game_data = (Arguments *)args;

    SDL_Init(SDL_INIT_EVENTS);

    while (!game_data->done)
    {
        switch (game_data->scene)
        {
        case WAIT_SCENE:
            waitEvents(game_data);
            break;

        case GAME_SCENE:
            gameEvents(game_data);
            break;

        case END_SCENE:
            endEvents(game_data);
            break;

        case MENU_SCENE:
            menuEvents(game_data);
            break;

        case SETTINGS_SCENE:
            settingsEvents(game_data);
            break;

        default:
            break;
        }
    }

    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void gameEvents(Arguments *args)
{
    SDL_Event event;

    while (args->scene == GAME_SCENE)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                args->done = 1;
                args->scene = FREE_SCENE;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    args->done = 1;
                    args->scene = FREE_SCENE;
                    break;
                }

            case SDL_MOUSEMOTION:
                args->entities->paddle.x = (float)event.motion.x - (args->entities->paddle.w / 2);
                if (args->entities->paddle.x < BORDER_WIDTH)
                    args->entities->paddle.x = BORDER_WIDTH;
                if (args->entities->paddle.x > WINDOW_WIDTH - BORDER_WIDTH - PADDLE_WIDTH)
                    args->entities->paddle.x = WINDOW_WIDTH - BORDER_WIDTH - PADDLE_WIDTH;
                break;

            default:
                break;
            }
        }
    }
}

void waitEvents(Arguments *args)
{
    SDL_Event event;
    initBall(args);

    while (args->scene == WAIT_SCENE)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                args->done = 1;
                args->scene = FREE_SCENE;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    args->done = 1;
                    args->scene = FREE_SCENE;
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                generateBallXDirection(args);
                args->scene = GAME_SCENE;
                break;

            case SDL_MOUSEMOTION:
                args->entities->paddle.x = (float)event.motion.x - (args->entities->paddle.w / 2);

                if (args->entities->paddle.x < BORDER_WIDTH)
                    args->entities->paddle.x = BORDER_WIDTH;
                if (args->entities->paddle.x > WINDOW_WIDTH - BORDER_WIDTH - PADDLE_WIDTH)
                    args->entities->paddle.x = WINDOW_WIDTH - BORDER_WIDTH - PADDLE_WIDTH;

                args->entities->ball.x = ((float)args->entities->paddle.x + ((float)args->entities->paddle.w / 2)) - (args->entities->ball.w / 2);
                break;

            default:
                break;
            }
        }
    }
}

void endEvents(Arguments *args)
{
    SDL_Event event;

    while (args->scene == END_SCENE)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                args->done = 1;
                args->scene = FREE_SCENE;
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_KEYDOWN:
                args->done = 1;
                args->scene = FREE_SCENE;
                break;

            default:
                break;
            }
        }
    }
}

void menuEvents(Arguments *args)
{
    SDL_Event event;

    while (args->scene == MENU_SCENE)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                args->done = 1;
                args->scene = INVALID_SCENE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                for (size_t i = 1; i < MENU_TEXT_COUNT; i++)
                {
                    if (args->entities->interface.text_hover[i])
                    {
                        switch (i)
                        {
                        case 1:
                            args->scene = INIT_SCENE;
                            break;

                        case 2:
                            args->scene = INIT_SETTINGS_SCENE;
                            break;

                        case 3:
                            args->done = 1;
                            args->scene = -1;
                            break;

                        default:
                            break;
                        }
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                args->cursor.x = event.motion.x;
                args->cursor.y = event.motion.y;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    args->done = 1;
                    args->scene = INVALID_SCENE;
                    break;
                }

            default:
                break;
            }
        }
    }
}

void settingsEvents(Arguments *args)
{
    SDL_Event event;

    while (args->scene == SETTINGS_SCENE)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                args->done = 1;
                args->scene = INVALID_SCENE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                for (size_t i = 1; i < MENU_TEXT_COUNT; i++)
                {
                    if (args->entities->interface.text_hover[i])
                    {
                        switch (i)
                        {
                        case 1:
                            args->brick_config = 0;
                            args->scene = INIT_MENU_SCENE;
                            break;

                        case 2:
                            args->brick_config = 1;
                            args->scene = INIT_MENU_SCENE;
                            break;

                        case 3:
                            args->scene = INIT_MENU_SCENE;
                            break;

                        default:
                            break;
                        }
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                args->cursor.x = event.motion.x;
                args->cursor.y = event.motion.y;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    args->done = 1;
                    args->scene = INVALID_SCENE;
                    break;
                }

            default:
                break;
            }
        }
    }
}