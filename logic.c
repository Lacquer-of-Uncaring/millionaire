#include <string.h>
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


game_t* game_init(){
    char *SAMPLETEXT = "This is an example of my problem, for most lines it works fine, albeit it looks a bit tight. But for any letters that \"hang\" below the line.";
    char *ANS = "ICELAND";
    char *swi = "this is a switch question?";
    char *ans = "lol";

    question test = {SAMPLETEXT, ANS, ANS, ANS, ANS, A_CORRECT};
    question tests = {swi, ans, ans, ans, ans, B_CORRECT};

    game_t* game = malloc(sizeof(game_t));
    for (int i=0; i<15; i++){
        game->questions[i] = test;
        game->switch_questions[i] = tests;
    }
    game->selection = NO_SELECTION;
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
    game->lifeline_50 = 1;
    game->lifeline_25 = 1;
    game->lifeline_switch = 1;
    game->state = RUNNING_STATE;
    game->question_number = 1;
    game->timer = FIRST_COUNTDOWN;
    
    return game;   
} 


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
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
    if (game->question_number < CHECKPOINT_2)
        game->timer = FIRST_COUNTDOWN;
    else if (game->question_number < CHECKPOINT_3)
        game->timer = SECOND_COUNTDOWN;
    else
        game->timer = THIRD_COUNTDOWN;
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
    if (game->state == RUNNING_STATE){    
        if (game->selection < 5){
            if (game->lifeline_50){
                int r1 = delete_wrong_answer(game);
                int r2;
                do{
                    r2 = delete_wrong_answer(game);
                } while (r1 == r2);
                game->lifeline_50 = 0;
                SDL_Delay(1000);
            }
        }   
    }
}

void use_lifeline_25(game_t* game){
    if (game->state == RUNNING_STATE){    
        if (game->selection < 5){
            if (game->lifeline_25){
                delete_wrong_answer(game);
                game->lifeline_25 = 0;
                SDL_Delay(1000);
            }
        }
    }
}

void use_lifeline_switch(game_t* game){
    if (game->lifeline_switch && game->state == RUNNING_STATE && game->selection < 5){
        question *current_q = &game->questions[game->question_number-1];
        question *switch_q = &game->switch_questions[game->question_number-1];
        current_q->text = malloc(strlen(switch_q->text) + 1);
        strcpy(current_q->text,switch_q->text);
        current_q->ans_a = malloc(strlen(switch_q->ans_a) + 1);
        strcpy(current_q->ans_a,switch_q->ans_a);
        current_q->ans_b = malloc(strlen(switch_q->ans_b) + 1);
        strcpy(current_q->ans_b,switch_q->ans_b);
        current_q->ans_c = malloc(strlen(switch_q->ans_c) + 1);
        strcpy(current_q->ans_c,switch_q->ans_c);
        current_q->ans_d = malloc(strlen(switch_q->ans_d) + 1);
        strcpy(current_q->ans_d,switch_q->ans_d);
        current_q->correct = switch_q->correct;
        // reset the question
        game->lifeline_switch = 0;
        game->selection = NO_SELECTION;
        game->A_available = 1;
        game->B_available = 1;
        game->C_available = 1;
        game->D_available = 1;
        if (game->question_number < CHECKPOINT_2)
            game->timer = FIRST_COUNTDOWN;
        else if (game->question_number < CHECKPOINT_3)
            game->timer = SECOND_COUNTDOWN;
        else
            game->timer = THIRD_COUNTDOWN;
    }
}