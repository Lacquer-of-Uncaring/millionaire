#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"
#include "menu-items.h"
#include "rendering.h"


void game_loop(SDL_Renderer* renderer, game_t* game, menu_t* menu){
	// Initializing some parameters for logic functions
    int init = SDL_GetTicks();
    int x,y;
    int animate = 1;
    int walk_away = 0;

    // Event handling
    SDL_Event e;
    while (game->state != QUIT_STATE) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
            // Exit the program entirely
                game->state = QUIT_STATE;
                menu->state = QUIT;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode){ 
                case SDL_SCANCODE_ESCAPE:
                // Go back to menu 
                    walk_away = 1;
                    game->state = GAME_OVER_STATE;
                    //menu->state = RUNNING;
                    //menu->selection = NO_SELECTION;
                    break;
                case SDL_SCANCODE_1:
                    use_lifeline_50(game);
                    break;
                case SDL_SCANCODE_2:
                    use_lifeline_25(game);
                    break;
                case SDL_SCANCODE_3:
                    use_lifeline_switch(game,&animate);
                    break;

                case SDL_SCANCODE_A:
                    // Can only select in running state and before confirming
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->A_available)
                            game->selection = A_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_B:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->B_available)
                            game->selection = B_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_C:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->C_available)
                            game->selection = C_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_D:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->D_available)
                            game->selection = D_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                case SDL_SCANCODE_SPACE:
                    answer_confirm(game);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    if (game->state == RUNNING_STATE)
                        if (game->selection < 5)
                            game->selection = NO_SELECTION;
                    break;
                }
                break;
            // Mouse selection 
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x,&y);
                hover_select(game, x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                answer_confirm(game);
                break;

            default: {}
            }
        }
        // Done with event handling for the frame

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        decrement_timer(game, &init);
        render_game(renderer, game, &animate, &walk_away);
        SDL_RenderPresent(renderer);
        
        check_game_over_state(game, menu, &animate, &walk_away);
        // FPS
        SDL_Delay(1000/60);
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        SDL_FlushEvent(SDL_MOUSEMOTION);
    }
}

void text_input(SDL_Renderer* renderer, menu_t* menu, char* text){
    int done = 0;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new text onto the end of our text */
                    strcat(text, e.text.text);
                    break;
                case SDL_TEXTEDITING:
                    /*
                    Update the composition text.
                    Update the cursor position.
                    Update the selection length (if any).
                    
                    composition = e.edit.text;
                    cursor = e.edit.start;
                    selection_len = e.edit.length;
                    */
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,text);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

// User logic

node* user_search(node* head, char* id){
    // Base case 
    if (head == NULL) 
        return NULL; 
      
    // If id is in node
    if (!strcmp(head->user->id, id))
        return head; 
  
    // Recur for remaining list 
    return user_search(head->next, id);
}

