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

void check_game_over_state(game_t *game){
    if ((game->question_number == 15 && check_answer(game)) || !check_answer(game) || game->timer <= 0 )
        game->state = GAME_OVER_STATE;
    else{
        game->question_number += 1;
        game->selection = NO_SELECTION;
        game->state = RUNNING_STATE; 
        game->timer = DEFAULT_COUNTDOWN;
    }
}   

void hover_select(game_t* game, int x, int y){
    if (game->state == RUNNING_STATE){
        if (game->selection < 5){
            game->selection = NO_SELECTION;        
            if (x > box_a_x && x < box_a_x + box_w && y > box_a_y && y < box_a_y + box_h)
                game->selection = A_SELECTED;
            if (x > box_b_x && x < box_b_x + box_w && y > box_b_y && y < box_b_y + box_h)
                game->selection = B_SELECTED;
            if (x > box_c_x && x < box_c_x + box_w && y > box_c_y && y < box_c_y + box_h)
                game->selection = C_SELECTED;
            if (x > box_d_x && x < box_d_x + box_w && y > box_d_y && y < box_d_y + box_h)
                game->selection = D_SELECTED;
        }
    }
}