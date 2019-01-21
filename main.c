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

    char *SAMPLETEXT = "This is an example of my problem, for most lines it works fine, albeit it looks a bit tight. But for any letters that \"hang\" below the line.";
    char *ANS = "ICELAND";
    char *swi = "this is a switch question?";
    char *ans = "lol";

    question test = {SAMPLETEXT, ANS, ANS, ANS, ANS, A_CORRECT};
    question tests = {swi, ans, ans, ans, ans, B_CORRECT};

    game_t game = {
        .questions = {test,test,test,test,test,test,test,test,test,test,test,test,test,test,test},
        .switch_questions = {tests,tests,tests,tests,tests,tests,tests,tests,tests,tests,tests,tests,tests,tests,tests},
        .selection = NO_SELECTION,
        .A_available = 1,
        .B_available = 1,
        .C_available = 1,
        .D_available = 1,
        .lifeline_50 = 1,
        .lifeline_25 = 1,
        .lifeline_switch = 1,
        .state = RUNNING_STATE,
        .question_number = 1,
        .timer = FIRST_COUNTDOWN
    };

    // Initializing some parameters for logic functions
    int init = SDL_GetTicks();
    int x,y;

    // Event handling
    SDL_Event e;
    while (game.state != QUIT_STATE) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                game.state = QUIT_STATE;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode){
                case SDL_SCANCODE_1:
                    use_lifeline_50(&game);
                    break;
                case SDL_SCANCODE_2:
                    use_lifeline_25(&game);
                    break;
                case SDL_SCANCODE_3:
                    use_lifeline_switch(&game);
                    break;

                case SDL_SCANCODE_A:
                    // Can only select in running state and before confirming
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5 && game.A_available)
                            game.selection = A_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_B:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5 && game.B_available)
                            game.selection = B_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_C:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5 && game.C_available)
                            game.selection = C_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_D:
                    if (game.state == RUNNING_STATE){
                        if (game.selection < 5 && game.D_available)
                            game.selection = D_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                case SDL_SCANCODE_SPACE:
                    answer_confirm(&game);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    if (game.state == RUNNING_STATE)
                        if (game.selection < 5)
                            game.selection = NO_SELECTION;
                    break;
                }
                break;
            // Mouse selection 
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x,&y);
                hover_select(&game, x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                answer_confirm(&game);
                break;

            default: {}
            }
        }
        // Done with event handling for the frame

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        decrement_timer(&game, &init);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
        
        check_game_over_state(&game);
        // FPS
        SDL_Delay(1000/60);

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
