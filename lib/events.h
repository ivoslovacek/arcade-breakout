#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "data.h"

void gameEvents(Arguments *args);

void endEvents(Arguments *args);

void waitEvents(Arguments *args);

void menuEvents(Arguments *args);

void settingsEvents(Arguments *args);

void *handleEvents(void *data);