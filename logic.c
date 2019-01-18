#include "game.h"
#include "logic.h"

bool check_answer(game_t *game){
    int Q = game->question_number;
    if ((game->questions[Q-1]).correct == game->selection ){
        return true;
    }
    else{
      return false;
    }
}

void answer_confirmed(game_t *game){
    game->selection += 4;
}

void check_gameover_state(game_t *game){
    if ((game->state == 15 && check_answer(game)) || (!check_answer(game)) /* +timer fucntion*/ ){
        game->state = GAME_OVER_STATE;
    }
    else {
        game->state += 1;
    }
}

void answer_selected(game_t* game, int X, int Y){
    if ((0<X && X<320) && (300<Y && Y<390)){
        game->selection = A_SELECTED;
    }

    else if ((320<X && X<640) && (300<Y && Y<390)){
        game->selection = B_SELECTED;
    }

    else if ((0<X && X<320) && (390<Y && Y<480)){
        game->selection = C_SELECTED;
    }

    else if ((320<X && X<640) && (390<Y && Y<480)){
        game->selection = D_SELECTED;
    }
    else {
        game->selection = NO_SELECTION;
    }
}
