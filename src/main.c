#include <stdlib.h>
#include <pthread.h>
#include "data.h"
#include "render.h"
#include "events.h"
#include "collisions.h"

int main(int argv, char **argc)
{
    Arguments *args = (Arguments *)malloc(sizeof(Arguments));

    allocGameData(args);

    pthread_t event_thread;
    pthread_t game_thread;
    pthread_t collisions_thread;

    pthread_create(&event_thread, NULL, handleEvents, args);
    pthread_create(&game_thread, NULL, renderMain, args);
    pthread_create(&collisions_thread, NULL, collisionsMain, args);

    pthread_join(event_thread, NULL);
    pthread_join(game_thread, NULL);
    pthread_join(collisions_thread, NULL);

    freeGameData(args);

    free(args);

    return 0;
}