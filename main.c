#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Millionaire",
                                          100, 100,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("error creating window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("error creating renderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

//    game_t game = {
//        .board = { EMPTY, EMPTY, EMPTY,
//                   EMPTY, EMPTY, EMPTY,
//                   EMPTY, EMPTY, EMPTY },
//        .player = PLAYER_X,
//        .state = RUNNING_STATE
//    };
    int quit = 0;
    SDL_Event e;
    while (!quit /*game.state != QUIT_STATE*/) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = 1; //game.state = QUIT_STATE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                //click_on_cell(&game,
                //              e.button.y / CELL_HEIGHT,
                //              e.button.x / CELL_WIDTH);
                break;

            default: {}
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
        render_win_state(renderer);
        //render_game(renderer, &game);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
