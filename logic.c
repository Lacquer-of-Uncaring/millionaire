#include <time.h> 
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"

int box_a_x = 65;
int box_a_y = 325;
int box_b_x = 350;
int box_b_y = 325;
int box_c_x = 65;
int box_c_y = 395;
int box_d_x = 350;
int box_d_y = 395;
int box_w = 215;
int box_h = 55;


int check_answer(game_t *game){
    question current_q = game->questions[game->question_number-1];
    if (current_q.correct == game->selection)
        return 1;
    return 0;
}

void answer_confirm(game_t *game){
    if (game->state == RUNNING_STATE){
        if (game->selection < 5 && game->selection > 0)
            game->selection += 4;
    }
}

void next_question(game_t* game){
    game->question_number += 1;
    game->selection = NO_SELECTION;
    game->state = RUNNING_STATE; 
    game->timer = DEFAULT_COUNTDOWN;
    game->lifeline_used_in_question = 0;
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
}

void check_game_over_state(game_t *game){
    if (game->state == CHECKING_STATE){
        SDL_Delay(1000);
        if ((game->question_number == 15 && check_answer(game)) || !check_answer(game) || game->timer <= 0 )
            game->state = GAME_OVER_STATE;
        else
            next_question(game);
    }

    // Go into checking state if the player confirmed his answer or ran out of time
    if (game->state == RUNNING_STATE){
        if (game->selection > 4){
            SDL_Delay(1000);
            game->state = CHECKING_STATE;
        }
        if (game->timer <= 0){
            SDL_Delay(1000);
            game->state = CHECKING_STATE;
        }
    }
        
}   

void hover_select(game_t* game, int x, int y){
    if (game->state == RUNNING_STATE){
        if (game->selection < 5){
            game->selection = NO_SELECTION;        
            if (x > box_a_x && x < box_a_x + box_w && y > box_a_y && y < box_a_y + box_h && game->A_available)
                game->selection = A_SELECTED;
            if (x > box_b_x && x < box_b_x + box_w && y > box_b_y && y < box_b_y + box_h && game->B_available)
                game->selection = B_SELECTED;
            if (x > box_c_x && x < box_c_x + box_w && y > box_c_y && y < box_c_y + box_h && game->C_available)
                game->selection = C_SELECTED;
            if (x > box_d_x && x < box_d_x + box_w && y > box_d_y && y < box_d_y + box_h && game->D_available)
                game->selection = D_SELECTED;
        }
    }
}

void decrement_timer(game_t* game, int* init){
    int current = SDL_GetTicks();
    // Decrement the countdown timer every second
    if (current - *init > 1000){
        if (game->state == RUNNING_STATE){
            game->timer--;
            *init = SDL_GetTicks();
        }        
    }
}

int random_wrong_answer(game_t* game){
    question current_q = game->questions[game->question_number-1];
    int r;
    do{
        r = rand() % 4;
    } while (r == current_q.correct - 5);
    
    return r;
}

int delete_wrong_answer(game_t* game){
    int r = random_wrong_answer(game);
    switch (r){
        case 0:
            game->A_available = 0;
            break;
        case 1:
            game->B_available = 0;
            break;
        case 2:
            game->C_available = 0;
            break;
        case 3:
            game->D_available = 0;
            break;    
        
        default : {}
    }
    return r;
}

void use_lifeline_50(game_t* game){
    if (game->lifeline_50 && !game->lifeline_used_in_question){
        int r1 = delete_wrong_answer(game);
        int r2;
        do{
            r2 = delete_wrong_answer(game);
        } while (r1 == r2);
        game->lifeline_used_in_question = 1;
        game->lifeline_50 = 0;
        SDL_Delay(500);
    }
}

void use_lifeline_25(game_t* game){
    if (game->lifeline_25 && !game->lifeline_used_in_question){
        delete_wrong_answer(game);
        game->lifeline_used_in_question = 1;
        game->lifeline_25 = 0;
        SDL_Delay(500);
    }
}