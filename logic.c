#include "game.h"
#include "logic.h"


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
