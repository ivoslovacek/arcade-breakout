#pragma once
#include "data.h"

void *collisionsMain(void *ptr);

void changeBallXDirection(Arguments *args);

void changeBallYDirection(Arguments *args);

void checkBorderCollison(Arguments *args);

void checkPaddleCollision(Arguments *args);