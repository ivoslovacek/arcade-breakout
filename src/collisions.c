#include "collisions.h"
#include "data.h"
#include "audio.h"

void changeBallXDirection(Arguments *args)
{
    args->entities->ball_xdirection *= -1;
}

void changeBallYDirection(Arguments *args)
{
    args->entities->ball_ydirection *= -1;
}

void checkBorderCollison(Arguments *args)
{
    for (size_t i = 0; i < args->entities->interface.border_count; i++)
    {
        if (SDL_HasIntersectionF(&args->entities->ball, args->entities->interface.borders + i))
        {
            switch (i)
            {
            case 0:
                args->entities->ball.x = args->entities->interface.borders[0].w + 1;
                changeBallXDirection(args);
                playSoundFromMemory(args->sounds[7], SDL_MIX_MAXVOLUME);
                break;

            case 1:
                args->entities->ball.x = args->entities->interface.borders[1].x - args->entities->ball.w - 1;
                changeBallXDirection(args);
                playSoundFromMemory(args->sounds[7], SDL_MIX_MAXVOLUME);
                break;

            case 2:
                args->entities->ball.y = args->entities->interface.borders[2].y + args->entities->interface.borders[2].h + 1;
                changeBallYDirection(args);
                playSoundFromMemory(args->sounds[0], SDL_MIX_MAXVOLUME);
                break;
            }
            changeBallSpeed(args);
        }
    }
}

SDL_bool checkOppositePaddleDirection(SDL_FRect current_paddle, Arguments *args)
{
    if (current_paddle.x < args->entities->previous_paddle.x && args->entities->ball_xdirection == 1)
    {
        return SDL_TRUE;
    }
    else if (current_paddle.x > args->entities->previous_paddle.x && args->entities->ball_xdirection == -1)
    {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

void checkPaddleCollision(Arguments *args)
{

    if (SDL_HasIntersectionF(&args->entities->ball, &args->entities->paddle))
    {

        changeBallYDirection(args);
        args->entities->ball.y = args->entities->paddle.y - args->entities->ball.h - 1;
        changeBallSpeed(args);
        if (checkOppositePaddleDirection(args->entities->paddle, args))
        {
            changeBallXDirection(args);
            if (args->entities->ball_speed > MIN_BALL_SPEED)
                args->entities->ball_speed *= 0.5;
            else
                args->entities->ball_speed = MIN_BALL_SPEED;
        }

        playSoundFromMemory(args->sounds[0], SDL_MIX_MAXVOLUME);
    }

    args->entities->previous_paddle = args->entities->paddle;
}

void checkVoidCollision(Arguments *args)
{
    if (args->entities->ball.y > WINDOW_HEIGHT)
    {
        args->lives--;
        if (args->lives >= 0)
        {
            args->game_stat_change = SDL_TRUE;
            args->scene = WAIT_SCENE;
        }
        else
        {
            args->scene = END_SCENE;
            args->game_stat_change = SDL_TRUE;
            playSound("lost.wav", 64);
        }
    }
}

void checkBrickCollision(Arguments *args)
{
    SDL_FRect tmp, tmp2;
    SDL_bool hit = SDL_FALSE;

    for (size_t i = 0; i < BRICKS_IN_ROWS; i++)
    {
        for (size_t j = 0; j < BRICKS_IN_COLS; j++)
        {
            tmp.x = (float)args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.x;
            tmp.y = (float)args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.y;
            tmp.w = (float)args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.w;
            tmp.h = (float)args->entities->bricks[j + (i * BRICKS_IN_COLS)].parameters.h;

            if (SDL_HasIntersectionF(&args->entities->ball, &tmp) && args->entities->bricks[j + (i * BRICKS_IN_COLS)].state > 0)
            {
                if (args->entities->ball.x - args->entities->ball_xdirection + args->entities->ball.w <= tmp.x || args->entities->ball.x - args->entities->ball_xdirection >= tmp.x + tmp.w)
                {
                    changeBallXDirection(args);
                    if (args->entities->ball.x + args->entities->ball.w >= tmp.x && args->entities->ball.x <= tmp.x + (tmp.w / 2))
                    {
                        args->entities->ball.x = tmp.x - args->entities->ball.w - 1;
                    }
                    else
                    {
                        args->entities->ball.x = tmp.x + tmp.w + 1;
                    }
                }

                if (args->entities->ball.y - args->entities->ball_ydirection + args->entities->ball.h <= tmp.y || args->entities->ball.y - args->entities->ball_ydirection >= tmp.y + tmp.h)
                {
                    changeBallYDirection(args);
                    if (args->entities->ball.y <= tmp.y + tmp.h && args->entities->ball.y >= tmp.y + (tmp.h / 2))
                    {
                        args->entities->ball.y = tmp.y + tmp.h + 1;
                    }
                    else
                    {

                        args->entities->ball.y = tmp.y - args->entities->ball.h - 1;
                    }
                }

                changeBallSpeed(args);
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].state--;
                if (args->entities->bricks[j + (i * BRICKS_IN_COLS)].state <= 0)
                {
                    args->entities->current_bricks--;
                }
                args->entities->bricks[j + (i * BRICKS_IN_COLS)].texture_index = getTextureIndex(i, args->entities->bricks[j + (i * BRICKS_IN_COLS)].state);
                args->score += BRICKS_IN_ROWS - i;
                args->game_stat_change = SDL_TRUE;
                playSoundFromMemory(args->sounds[SOUND_COUNT - 1 - i], SDL_MIX_MAXVOLUME);
            }
        }
    }
}

void gameCollisions(Arguments *args)
{
    while (args->scene == GAME_SCENE)
    {
        checkBorderCollison(args);
        checkPaddleCollision(args);
        checkVoidCollision(args);
        checkBrickCollision(args);

        if (args->entities->current_bricks <= 0)
        {
            playSound("won.wav", 64);
            args->scene = END_SCENE;
        }
    }
}

void optionHover(Arguments *args, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (SDL_PointInRect(&args->cursor, args->entities->interface.text_pos[i]))
        {
            args->entities->interface.text_hover[i] = SDL_TRUE;
        }
        else
        {
            args->entities->interface.text_hover[i] = SDL_FALSE;
        }
    }
}

void menuCollisions(Arguments *args)
{
    optionHover(args, 1, MENU_TEXT_COUNT);
}

void *collisionsMain(void *ptr)
{
    Arguments *args = (Arguments *)ptr;

    SDL_Init(SDL_INIT_AUDIO);
    initAudio();
    loadSoundsToMemory(args);
    args->audio_ready = SDL_TRUE;

    while (!args->done)
    {
        switch (args->scene)
        {
        case GAME_SCENE:
            gameCollisions(args);
            break;

        case SETTINGS_SCENE:
        case MENU_SCENE:
            menuCollisions(args);
            break;

        default:
            break;
        }
    }

    freeSoundsFromMemory(args);
    endAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    args->audio_ready = SDL_TRUE;
}