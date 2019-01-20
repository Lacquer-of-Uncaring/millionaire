#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Millionaire",
                                          700, 100,
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

    const char *SAMPLETEXT = "This is an example of my problem, for most lines it works fine, albeit it looks a bit tight. But for any letters that \"hang\" below the line.";
    const char *ANS = "ICELAND";

    question test = {SAMPLETEXT, ANS, ANS, ANS, ANS, A_CORRECT};

    game_t game = {
        .questions = {test,test,test,test,test,test,test,test,test,test,test,test,test,test,test},
        .selection = NO_SELECTION,
        .state = RUNNING_STATE,
        .question_number = 1,
        .timer = 10
    };

    int init = SDL_GetTicks();
    int current;

    SDL_Event e;
    while (game.state != QUIT_STATE) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                game.state = QUIT_STATE;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode){
                case SDL_SCANCODE_A:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5)
                            game.selection = A_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_B:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5)
                            game.selection = B_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_C:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5)
                            game.selection = C_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_D:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5)
                            game.selection = D_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                    answer_confirm(&game);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    if (game.state == RUNNING_STATE)
                        if (game.selection < 5)
                            game.selection = NO_SELECTION;
                    break;
                }
                break; 
            case SDL_MOUSEBUTTONDOWN:
                //click_on_cell(&game,
                //              e.button.y / CELL_HEIGHT,
                //              e.button.x / CELL_WIDTH);
                break;

            default: {}
            }
        }
    
        current = SDL_GetTicks();

        if (current-init > 1000){
            game.timer--;
            init = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
        
        if (game.state == CHECKING_STATE){
            SDL_Delay(1000);
            check_game_over_state(&game);
        }
        
        if (game.state == RUNNING_STATE)
            if (game.selection > 4){
                SDL_Delay(1000);
                game.state = CHECKING_STATE;
            }
        

        SDL_Delay(1000/60);

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
